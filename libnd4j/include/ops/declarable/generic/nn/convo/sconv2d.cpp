/* ******************************************************************************
 *
 *
 * This program and the accompanying materials are made available under the
 * terms of the Apache License, Version 2.0 which is available at
 * https://www.apache.org/licenses/LICENSE-2.0.
 *
 *  See the NOTICE file distributed with this work for additional
 *  information regarding copyright ownership.
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 * SPDX-License-Identifier: Apache-2.0
 ******************************************************************************/

//
// @author raver119, created on 29/10/17.
// @author Yurii Shyrma, changed on 20.03.2018
//

#include <system/op_boilerplate.h>
#if NOT_EXCLUDED(OP_sconv2d)

#include <ops/declarable/CustomOperations.h>
#include <ops/declarable/helpers/convolutions.h>

#include <memory>

namespace sd {
namespace ops {

CUSTOM_OP_IMPL(sconv2d, 2, 1, false, 0, 9) {
  NDArray *input = INPUT_VARIABLE(0);         // [bS, iH, iW, iC]  (NHWC) or [bS, iC, iH, iW]  (NCHW)
  NDArray *weightsDepth = INPUT_VARIABLE(1);  // [kH, kW, iC, mC], [mC, iC, kH, kW], [mC, kH, kW, iC]
  NDArray *weightsPoint = nullptr;            // [1, 1, iC*mC, oC], [oC, iC*mC, 1, 1], [oC, 1, 1, iC*mC]
  NDArray *bias = nullptr;                    // [oC], if weightsPoint=nullptr then oC = iC*mC

  NDArray *output = OUTPUT_NULLIFIED(0);  // [bS, oH, oW, oC]  (NHWC) or [bS, oC, oH, oW]  (NCHW)

  if (block.width() == 3) {
    if ((INPUT_VARIABLE(2))->rankOf() == 4)
      weightsPoint = INPUT_VARIABLE(2);
    else
      bias = INPUT_VARIABLE(2);
  } else if (block.width() == 4) {
    weightsPoint = INPUT_VARIABLE(2);
    bias = INPUT_VARIABLE(3);
  }

  REQUIRE_TRUE(input->rankOf() == 4, 0, " SCONV2D OP: rank of input array must be equal to 4, but got %i instead !",
               input->rankOf());
  REQUIRE_TRUE(weightsDepth->rankOf() == 4, 0,
               " SCONV2D OP: rank of weightsDepth array must be equal to 4, but got %i instead !",
               weightsDepth->rankOf());
  if (weightsPoint)
    REQUIRE_TRUE(weightsPoint->rankOf() == 4, 0,
                 " SCONV2D OP: rank of weightsPoint array must be equal to 4, but got %i instead !",
                 weightsPoint->rankOf());
  if (bias)
    REQUIRE_TRUE(bias->rankOf() == 1 || bias->rankOf() == 2, 0,
                 " SCONV2D OP: rank of biases array must be equal to 1 or 2, but got %i instead !", bias->rankOf());
  ;

  LongType kH = INT_ARG(0);                                               // filter(kernel) height
  LongType kW = INT_ARG(1);                                               // filter(kernel) width
  LongType sH = INT_ARG(2);                                               // strides height
  LongType sW = INT_ARG(3);                                               // strides width
  LongType pH = INT_ARG(4);                                               // paddings height
  LongType pW = INT_ARG(5);                                               // paddings width
  LongType dH = INT_ARG(6);                                               // dilations height
  LongType dW = INT_ARG(7);                                               // dilations width
  int isSameMode = INT_ARG(8);                                       // 0-VALID, 1-SAME
  int isNCHW = block.getIArguments()->size() > 9 ? !INT_ARG(9) : 1;  // INT_ARG(9): 0-NCHW,  1-NHWC
  int wFormat = block.getIArguments()->size() > 10
                    ? INT_ARG(10)
                    : 0;  // 0 - [kH, kW, iC, mC], 1 - [mC, iC, kH, kW], 2 - [mC, kH, kW, iC]

  LongType bS, iC, iH, iW, mC, oC, oH,
      oW;  // batch size, input channels, input height/width, channels multiplier, output channels, output height/width
  LongType indIOioC, indIiH, indWmC, indWiC, indWkH, indOoH;  // corresponding indexes
  ConvolutionUtils::getSizesAndIndexesConv2d(isNCHW, wFormat, *input, *output, bS, iC, iH, iW, oC, oH, oW, indIOioC,
                                             indIiH, indWiC, indWmC, indWkH, indOoH);
  mC = weightsDepth->sizeAt(indWmC);  // channels multiplier

  std::vector<sd::LongType> expectedWeightsDShape = ConvolutionUtils::expectWeightsShape(wFormat, kH, kW, iC, mC);
  REQUIRE_TRUE(weightsDepth->isSameShape(expectedWeightsDShape), 0,
               " SCONV2D OP: wrong shape of weightsDepth array, expected is %s, but got %s instead !",
               ShapeUtils::shapeAsString(expectedWeightsDShape).c_str(),
               ShapeUtils::shapeAsString(weightsDepth).c_str());
  if (weightsPoint) {
    std::vector<sd::LongType> expectedWeightsPShape = ConvolutionUtils::expectWeightsShape(wFormat, 1, 1, iC * mC, oC);
    REQUIRE_TRUE(weightsPoint->isSameShape(expectedWeightsPShape), 0,
                 " SCONV2D OP: wrong shape of weightsPoint array, expected is %s, but got %s instead !",
                 ShapeUtils::shapeAsString(expectedWeightsPShape).c_str(),
                 ShapeUtils::shapeAsString(weightsPoint).c_str());
  }
  if (bias)
    REQUIRE_TRUE(oC == bias->lengthOf(), 0,
                 " SCONV2D OP: length of bias array must be equal to outChannels, but got %i instead",
                 bias->lengthOf());

  if (iC == 1) {
    sd_debug("SCONV2D OP: for input_channels = 1 this op is equivalent to standard conv2d\n", "");
    ConvolutionUtils::conv2d(block, input, weightsDepth, bias, output, kH, kW, sH, sW, pH, pW, dH, dW, isSameMode,
                             isNCHW, wFormat);
    return sd::Status::OK;
  }

  ConvolutionUtils::sconv2d(block, input, weightsDepth, weightsPoint, bias, output, kH, kW, sH, sW, pH, pW, dH, dW,
                            isSameMode, isNCHW, wFormat);

  return sd::Status::OK;
}

DECLARE_TYPES(sconv2d) {
  getOpDescriptor()->setAllowedInputTypes(sd::DataType::ANY)->setAllowedOutputTypes({ALL_FLOATS});
}

DECLARE_SHAPE_FN(sconv2d) {
  auto inputShapeInfo = inputShape->at(0);          // [bS, iH, iW, iC]  (NHWC) or [bS, iC, iH, iW]  (NCHW)
  auto weightsDShapeInfo = inputShape->at(1);       // [kH, kW, iC, mC], [mC, iC, kH, kW], [mC, kH, kW, iC]
  sd::LongType const *weightsPShapeInfo = nullptr;  // [1, 1, iC*mC, oC], [oC, iC*mC, 1, 1], [oC, 1, 1, iC*mC]
  sd::LongType const *biasShapeInfo = nullptr;      // [oC], oC = iC*mC if weightsPoint=nullptr

  if (block.width() == 3)
    if (inputShape->at(2)[0] == 4)
      weightsPShapeInfo = inputShape->at(2);
    else
      biasShapeInfo = inputShape->at(2);
  else if (block.width() == 4) {
    weightsPShapeInfo = inputShape->at(2);
    biasShapeInfo = inputShape->at(3);
  }

  const LongType rank = 4;
  REQUIRE_TRUE(inputShapeInfo[0] == rank, 0,
               "SCONV2D OP: rank of input array must be equal to %i, but got %i instead !", rank, inputShapeInfo[0]);
  REQUIRE_TRUE(weightsDShapeInfo[0] == rank, 0,
               "SCONV2D OP: rank of weightsDepth array must be equal to %i, but got %i instead !", rank,
               weightsDShapeInfo[0]);
  if (weightsPShapeInfo)
    REQUIRE_TRUE(weightsPShapeInfo[0] == rank, 0,
                 "SCONV2D OP: rank of weightsPoint array must be equal to %i, but got %i instead !", rank,
                 weightsPShapeInfo[0]);
  if (biasShapeInfo)
    REQUIRE_TRUE(biasShapeInfo[0] <= 2, 0, "SCONV2D OP: rank of biases array must be <= 2, but got %i instead !",
                 biasShapeInfo[0]);


  LongType kH = INT_ARG(0);                                               // filter(kernel) height
  LongType kW = INT_ARG(1);                                               // filter(kernel) width
  LongType sH = INT_ARG(2);                                               // strides height
  LongType sW = INT_ARG(3);                                               // strides width
  LongType pH = INT_ARG(4);                                               // paddings height
  LongType pW = INT_ARG(5);                                               // paddings width
  LongType dH = INT_ARG(6);                                               // dilations height
  LongType dW = INT_ARG(7);                                               // dilations width
  int isSameMode = INT_ARG(8);                                       // 0-VALID, 1-SAME
  int isNCHW = block.getIArguments()->size() > 9 ? !INT_ARG(9) : 1;  // INT_ARG(9): 1-NHWC, 0-NCHW
  int wFormat = block.getIArguments()->size() > 10
                    ? INT_ARG(10)
                    : 0;  // 0 - [kH, kW, iC, mC], 1 - [mC, iC, kH, kW], 2 - [mC, kH, kW, iC]

  LongType indIOioC, indIiH, indWmC(0 == wFormat ? 3 : 0);
  if (!isNCHW) {
    indIOioC = 3;
    indIiH = 1;
  } else {
    indIOioC = 1;
    indIiH = 2;
  }

  const LongType bS = inputShapeInfo[1];                                            // batch size
  const LongType iH = inputShapeInfo[indIiH + 1];                                   // input height
  const LongType iW = inputShapeInfo[indIiH + 2];                                   // input width
  const LongType iC = inputShapeInfo[indIOioC + 1];                                 // input channels
  const LongType mC = weightsDShapeInfo[indWmC + 1];                                // channel multiplier
  const LongType oC = weightsPShapeInfo ? weightsPShapeInfo[indWmC + 1] : iC * mC;  // output channels (oC or iC*mC)

  std::vector<sd::LongType> expectedWeightsDShape = ConvolutionUtils::expectWeightsShape(wFormat, kH, kW, iC, mC);
  REQUIRE_TRUE(ShapeUtils::areShapesEqual(weightsDShapeInfo, expectedWeightsDShape), 0,
               "SCONV2D OP: wrong shape of depth weights array, expected is %s, but got %s instead !",
               ShapeUtils::shapeAsString(expectedWeightsDShape).c_str(),
               ShapeUtils::shapeAsString(weightsDShapeInfo).c_str());
  if (weightsPShapeInfo) {
    std::vector<sd::LongType> expectedWeightsPShape = ConvolutionUtils::expectWeightsShape(wFormat, 1, 1, iC * mC, oC);
    REQUIRE_TRUE(ShapeUtils::areShapesEqual(weightsPShapeInfo, expectedWeightsPShape), 0,
                 "SCONV2D OP: wrong shape of point array, expected is %s, but got %s instead !",
                 ShapeUtils::shapeAsString(expectedWeightsPShape).c_str(),
                 ShapeUtils::shapeAsString(weightsPShapeInfo).c_str());
  }
  if (biasShapeInfo)
    REQUIRE_TRUE(
        biasShapeInfo[0] <= 2 && oC == shape::length(biasShapeInfo), 0,
        "SCONV2D OP: wrong shape of array with biases, expected rank, length: <=2, %i, but got %i, %i instead !", oC,
        biasShapeInfo[0], shape::length(biasShapeInfo));

  LongType oH, oW;  // output height, width
  ConvolutionUtils::calcOutSizePool2D(oH, oW, kH, kW, sH, sW, pH, pW, dH, dW, iH, iW, isSameMode);

  sd::LongType *outputShapeInfo = nullptr;
  ALLOCATE(outputShapeInfo, block.getWorkspace(), shape::shapeInfoLength(inputShapeInfo), sd::LongType);

  outputShapeInfo[0] = 4;
  outputShapeInfo[1] = bS;

  if (isNCHW) {
    outputShapeInfo[2] = oC;
    outputShapeInfo[3] = oH;
    outputShapeInfo[4] = oW;
  } else {
    outputShapeInfo[2] = oH;
    outputShapeInfo[3] = oW;
    outputShapeInfo[4] = oC;
  }

  ShapeUtils::updateStridesAndType(outputShapeInfo, weightsDShapeInfo, shape::order(inputShapeInfo));

  return SHAPELIST(CONSTANT(outputShapeInfo));
}

DECLARE_TYPES(sconv2d_bp) {
  getOpDescriptor()->setAllowedInputTypes(sd::DataType::ANY)->setAllowedOutputTypes({ALL_FLOATS});
}

////////////////////////////////////////////////////////////////////////
CUSTOM_OP_IMPL(sconv2d_bp, 3, 2, false, 0, 9) {
  NDArray *input = INPUT_VARIABLE(0);         // [bS, iH, iW, iC]  (NHWC) or [bS, iC, iH, iW]  (NCHW)
  NDArray *gradO = INPUT_VARIABLE(1);         // [bS, oH, oW, oC]  (NHWC) or [bS, oC, oH, oW] (NCHW), epsilon_next
  NDArray *weightsDepth = INPUT_VARIABLE(2);  // [kH, kW, iC, mC], [mC, iC, kH, kW], [mC, kH, kW, iC]
  NDArray *weightsPoint = nullptr;            // [1, 1, iC*mC, oC], [oC, iC*mC, 1, 1], [oC, 1, 1, iC*mC]
  NDArray *bias = nullptr;                    // [oC], oC = iC*mC if weightsPoint=nullptr

  NDArray *gradI = OUTPUT_NULLIFIED(0);   // [bS, iH, iW, iC]  (NHWC) or [bS, iC, iH, iW] (NCHW), epsilon
  NDArray *gradWD = OUTPUT_NULLIFIED(1);  // [kH, kW, iC, mC], [mC, iC, kH, kW], [mC, kH, kW, iC]
  NDArray *gradWP = nullptr;              // [1, 1, iC*mC, oC], [oC, iC*mC, 1, 1], [oC, 1, 1, iC*mC]
  NDArray *gradB = nullptr;               // [oC]

  if (block.width() == 4) {
    if ((INPUT_VARIABLE(3))->rankOf() == 4) {
      weightsPoint = INPUT_VARIABLE(3);
      gradWP = OUTPUT_NULLIFIED(2);
    } else {
      bias = INPUT_VARIABLE(3);
      gradB = OUTPUT_NULLIFIED(2);
    }
  } else if (block.width() == 5) {
    weightsPoint = INPUT_VARIABLE(3);
    bias = INPUT_VARIABLE(4);
    gradWP = OUTPUT_NULLIFIED(2);
    gradB = OUTPUT_NULLIFIED(3);
  }

  REQUIRE_TRUE(input->rankOf() == 4, 0, " SCONV2D_BP OP: rank of input array must be equal to 4, but got %i instead !",
               input->rankOf());
  REQUIRE_TRUE(gradO->rankOf() == 4, 0,
               " SCONV2D_BP OP: rank of output gradients (next epsilon) array must be equal to 4, but got %i instead !",
               gradO->rankOf());
  REQUIRE_TRUE(weightsDepth->rankOf() == 4, 0,
               " SCONV2D_BP OP: rank of weightsDepth array must be equal to 4 !, but got %i instead !",
               weightsDepth->rankOf());
  if (weightsPoint) {
    REQUIRE_TRUE(weightsPoint->rankOf() == 4, 0,
                 " SCONV2D_BP OP: rank of weightsPoint array must be equal to 4, but got %i instead !",
                 weightsPoint->rankOf());
    REQUIRE_TRUE(gradWP->rankOf() == 4, 0,
                 " SCONV2D_BP OP: rank of weightsPoint gradients array must be equal to 4, but got %i instead !",
                 gradWP->rankOf());
  }
  if (bias) {
    REQUIRE_TRUE(bias->rankOf() == 1 || bias->rankOf() == 2, 0,
                 " SCONV2D_BP OP: rank of biases array must be equal to 1 or 2, but got %i instead !", bias->rankOf());
    REQUIRE_TRUE(gradB->rankOf() == 1 || gradB->rankOf() == 2, 0,
                 " SCONV2D_BP OP: rank of  biases gradientsarray must be equal to 1 or 2, but got %i instead !",
                 gradB->rankOf());
  }

  LongType kH = INT_ARG(0);                                               // filter(kernel) height
  LongType kW = INT_ARG(1);                                               // filter(kernel) width
  LongType sH = INT_ARG(2);                                               // strides height
  LongType sW = INT_ARG(3);                                               // strides width
  LongType pH = INT_ARG(4);                                               // paddings height
  LongType pW = INT_ARG(5);                                               // paddings width
  LongType dH = INT_ARG(6);                                               // dilations height
  LongType dW = INT_ARG(7);                                               // dilations width
  int isSameMode = INT_ARG(8);                                       // 0-VALID, 1-SAME
  int isNCHW = block.getIArguments()->size() > 9 ? !INT_ARG(9) : 1;  // INT_ARG(9): 0-NCHW,  1-NHWC
  int wFormat = block.getIArguments()->size() > 10
                    ? INT_ARG(10)
                    : 0;  // 0 - [kH, kW, iC, mC], 1 - [mC, iC, kH, kW], 2 - [mC, kH, kW, iC]

  LongType bS, iC, iH, iW, mC, oC, oH,
      oW;  // batch size, input channels, input height/width, channels multiplier, output channels, output height/width
  LongType indIOioC, indIiH, indWmC, indWiC, indWkH, indOoH;  // corresponding indexes
  ConvolutionUtils::getSizesAndIndexesConv2d(isNCHW, wFormat, *input, *gradO, bS, iC, iH, iW, oC, oH, oW, indIOioC,
                                             indIiH, indWiC, indWmC, indWkH, indOoH);
  mC = weightsDepth->sizeAt(indWmC);  // channels multiplier

  std::vector<sd::LongType> expectedWeightsDShape = ConvolutionUtils::expectWeightsShape(wFormat, kH, kW, iC, mC);
  REQUIRE_TRUE(weightsDepth->isSameShape(expectedWeightsDShape), 0,
               " SCONV2D_BP OP: wrong shape of weightsDepth array, expected is %s, but got %s instead !",
               ShapeUtils::shapeAsString(expectedWeightsDShape).c_str(),
               ShapeUtils::shapeAsString(weightsDepth).c_str());
  REQUIRE_TRUE(gradWD->isSameShape(expectedWeightsDShape), 0,
               " SCONV2D_BP OP: wrong shape of gradWD array, expected is %s, but got %s instead !",
               ShapeUtils::shapeAsString(expectedWeightsDShape).c_str(), ShapeUtils::shapeAsString(gradWD).c_str());
  if (weightsPoint) {
    std::vector<sd::LongType> expectedWeightsPShape = ConvolutionUtils::expectWeightsShape(wFormat, 1, 1, iC * mC, oC);
    REQUIRE_TRUE(weightsPoint->isSameShape(expectedWeightsPShape), 0,
                 " SCONV2D_BP OP: wrong shape of weightsPoint array, expected is %s, but got %s instead !",
                 ShapeUtils::shapeAsString(expectedWeightsPShape).c_str(),
                 ShapeUtils::shapeAsString(weightsPoint).c_str());
    REQUIRE_TRUE(gradWP->isSameShape(expectedWeightsPShape), 0,
                 " SCONV2D_BP OP: wrong shape of gradWP array, expected is %s, but got %s instead !",
                 ShapeUtils::shapeAsString(expectedWeightsPShape).c_str(), ShapeUtils::shapeAsString(gradWP).c_str());
  }
  if (bias) {
    REQUIRE_TRUE(oC == bias->lengthOf(), 0,
                 " SCONV2D_BP OP: length of bias array must be equal to outChannels, but got %i instead",
                 bias->lengthOf());
    REQUIRE_TRUE(oC == gradB->lengthOf(), 0,
                 " SCONV2D_BP OP: length of biases gradients array must be equal to outChannels, but got %i instead",
                 gradB->lengthOf());
  }



  // ----- if weightsPoint is present, perform pointwise backprop first and calculate gradWP at this step ----- //
  if (weightsPoint) {
    auto resultFFShape =
        isNCHW ? std::vector<sd::LongType>({bS, mC * iC, oH, oW}) : std::vector<sd::LongType>({bS, oH, oW, mC * iC});
    auto resultFF = NDArrayFactory::create_(input->ordering(), resultFFShape, input->dataType(), block.launchContext());
    ConvolutionUtils::sconv2d(block, input, weightsDepth, nullptr, nullptr, resultFF, kH, kW, sH, sW, pH, pW, dH, dW,
                              isSameMode, isNCHW, wFormat);

    auto gradIDepthShape =
        ShapeUtils::composeShapeUsingDimsAndIdx({bS, iC * mC, oH, oW, 0, indIOioC, indIiH, indIiH + 1});
    auto gradIDepth =
        NDArrayFactory::create_(resultFF->ordering(), gradIDepthShape, resultFF->dataType(),
                                block.launchContext());  // [bS, oH, oW, iC*mC]  (NHWC) or [bS, iC*mC, oH, oW] (NCHW)

    ConvolutionUtils::conv2dBP(block, resultFF, weightsPoint, bias, gradO, gradIDepth, gradWP, gradB, 1, 1, 1, 1, 0, 0,
                               1, 1, isSameMode, isNCHW, wFormat);  // in this case oH=iH and oW=iW

    gradO = gradIDepth;
    bias = gradB = nullptr;  // if pointwise backprop was done then don't calculate gradB at depthwise_conv2d_bp step

    delete resultFF;
  }

  // ----- apply depthwise_conv2d_bp ----- //
  ConvolutionUtils::depthwiseConv2dBP(block, input, weightsDepth, bias, gradO, gradI, gradWD, gradB, kH, kW, sH, sW, pH,
                                      pW, dH, dW, isSameMode, isNCHW, wFormat);

  if (weightsPoint) delete gradO;

  return sd::Status::OK;
}

DECLARE_SHAPE_FN(sconv2d_bp) {
  auto inputShapeInfo = inputShape->at(0);          // [bS, iH, iW, iC]  (NHWC) or [bS, iC, iH, iW]  (NCHW)
  auto gradOShapeInfo = inputShape->at(1);          // [bS, oH, oW, oC]  (NHWC) or [bS, oC, oH, oW] (NCHW), epsilon_next
  auto weightsDShapeInfo = inputShape->at(2);       // [kH, kW, iC, mC], [mC, iC, kH, kW], [mC, kH, kW, iC]
  sd::LongType const *weightsPShapeInfo = nullptr;  // [1, 1, iC*mC, oC], [oC, iC*mC, 1, 1], [oC, 1, 1, iC*mC]
  sd::LongType const *biasShapeInfo = nullptr;      // [oC], oC = iC*mC if weightsPoint=nullptr

  if (block.width() == 4) {
    if (inputShape->at(3)[0] == 4)
      weightsPShapeInfo = inputShape->at(3);
    else
      biasShapeInfo = inputShape->at(3);
  } else if (block.width() == 5) {
    weightsPShapeInfo = inputShape->at(3);
    biasShapeInfo = inputShape->at(4);
  }

  const LongType rank = 4;
  REQUIRE_TRUE(inputShapeInfo[0] == rank, 0,
               " SCONV2D_BP OP: rank of input array must be equal to %i, but got %i instead !", rank,
               inputShapeInfo[0]);
  REQUIRE_TRUE(
      gradOShapeInfo[0] == rank, 0,
      " SCONV2D_BP OP: rank of output gradients (next epsilon) array must be equal to %i, but got %i instead !", rank,
      gradOShapeInfo[0]);
  REQUIRE_TRUE(weightsDShapeInfo[0] == rank, 0,
               " SCONV2D_BP OP: rank of weightsDepth array must be equal to %i, but got %i instead !", rank,
               weightsDShapeInfo[0]);
  if (weightsPShapeInfo)
    REQUIRE_TRUE(weightsPShapeInfo[0] == rank, 0,
                 " SCONV2D_BP OP: rank of weightsPoint array must be equal to %i, but got %i instead !", rank,
                 weightsPShapeInfo[0]);
  if (biasShapeInfo)
    REQUIRE_TRUE(biasShapeInfo[0] == 1 || biasShapeInfo[0] == 2, 0,
                 " SCONV2D_BP OP: rank of biases array must be 1 or 2, but got %i instead !", biasShapeInfo[0]);


  LongType kH = INT_ARG(0);                                               // filter(kernel) height
  LongType kW = INT_ARG(1);                                               // filter(kernel) width
  LongType sH = INT_ARG(2);                                               // strides height
  LongType sW = INT_ARG(3);                                               // strides width
  LongType pH = INT_ARG(4);                                               // paddings height
  LongType pW = INT_ARG(5);                                               // paddings width
  LongType dH = INT_ARG(6);                                               // dilations height
  LongType dW = INT_ARG(7);                                               // dilations width
  int isSameMode = INT_ARG(8);                                       // 0-VALID, 1-SAME
  int isNCHW = block.getIArguments()->size() > 9 ? !INT_ARG(9) : 1;  // INT_ARG(9): 0-NCHW,  1-NHWC
  int wFormat = block.getIArguments()->size() > 10
                    ? INT_ARG(10)
                    : 0;  // 0 - [kH, kW, iC, mC], 1 - [mC, iC, kH, kW], 2 - [mC, kH, kW, iC]

  int indIOioC, indIiH, indWmC(0 == wFormat ? 3 : 0);
  if (!isNCHW) {
    indIOioC = 3;
    indIiH = 1;
  } else {
    indIOioC = 1;
    indIiH = 2;
  }

  const LongType bS = inputShapeInfo[1];                                            // batch size
  const LongType iH = inputShapeInfo[indIiH + 1];                                   // input height
  const LongType iW = inputShapeInfo[indIiH + 2];                                   // input width
  const LongType iC = inputShapeInfo[indIOioC + 1];                                 // input channels
  const LongType mC = weightsDShapeInfo[indWmC + 1];                                // channel multiplier
  const LongType oC = weightsPShapeInfo ? weightsPShapeInfo[indWmC + 1] : iC * mC;  // output channels (oC or iC*mC)

  LongType trueoH, trueoW;  // true output height, width
  ConvolutionUtils::calcOutSizePool2D(trueoH, trueoW, kH, kW, sH, sW, pH, pW, dH, dW, iH, iW, isSameMode);

  std::vector<sd::LongType> expectedGradOShapeInfo =
      ShapeUtils::composeShapeUsingDimsAndIdx({bS, oC, trueoH, trueoW, 0, indIOioC, indIiH, indIiH + 1});
  REQUIRE_TRUE(
      ShapeUtils::areShapesEqual(gradOShapeInfo, expectedGradOShapeInfo), 0,
      "SCONV2D_BP OP: wrong shape of output gradients (next epsilon) array, expected is %s, but got %s instead !",
      ShapeUtils::shapeAsString(expectedGradOShapeInfo).c_str(), ShapeUtils::shapeAsString(gradOShapeInfo).c_str());
  std::vector<sd::LongType> expectedWeightsDShape = ConvolutionUtils::expectWeightsShape(wFormat, kH, kW, iC, mC);
  REQUIRE_TRUE(ShapeUtils::areShapesEqual(weightsDShapeInfo, expectedWeightsDShape), 0,
               "SCONV2D_BP OP: wrong shape of depth weights array, expected is %s, but got %s instead !",
               ShapeUtils::shapeAsString(expectedWeightsDShape).c_str(),
               ShapeUtils::shapeAsString(weightsDShapeInfo).c_str());
  if (weightsPShapeInfo) {
    std::vector<sd::LongType> expectedWeightsPShape = ConvolutionUtils::expectWeightsShape(wFormat, 1, 1, iC * mC, oC);
    REQUIRE_TRUE(ShapeUtils::areShapesEqual(weightsPShapeInfo, expectedWeightsPShape), 0,
                 "SCONV2D_BP OP: wrong shape of point array, expected is %s, but got %s instead !",
                 ShapeUtils::shapeAsString(expectedWeightsPShape).c_str(),
                 ShapeUtils::shapeAsString(weightsPShapeInfo).c_str());
  }
  if (biasShapeInfo)
    REQUIRE_TRUE(
        (biasShapeInfo[0] == 1 || biasShapeInfo[0] == 2) && oC == shape::length(biasShapeInfo), 0,
        "SCONV2D_BP OP: wrong shape of array with biases, expected rank, length: <=2, %i, but got %i, %i instead !", oC,
        biasShapeInfo[0], shape::length(biasShapeInfo));

  auto gradIshapeInfo =
      ShapeBuilders::copyShapeInfoAndType(inputShapeInfo, gradOShapeInfo, false, block.getWorkspace());
  auto gradWDshapeInfo =
      ShapeBuilders::copyShapeInfoAndType(weightsDShapeInfo, gradOShapeInfo, false, block.getWorkspace());

  sd::LongType *gradWPshapeInfo(nullptr), *gradBshapeInfo(nullptr);

  if (weightsPShapeInfo && biasShapeInfo) {
    gradWPshapeInfo =
        ShapeBuilders::copyShapeInfoAndType(weightsPShapeInfo, gradOShapeInfo, false, block.getWorkspace());
    gradBshapeInfo = ShapeBuilders::copyShapeInfoAndType(biasShapeInfo, gradOShapeInfo, false, block.getWorkspace());
    return SHAPELIST(CONSTANT(gradIshapeInfo), CONSTANT(gradWDshapeInfo), CONSTANT(gradWPshapeInfo),
                     CONSTANT(gradBshapeInfo));
  }

  if (weightsPShapeInfo && !biasShapeInfo) {
    gradWPshapeInfo =
        ShapeBuilders::copyShapeInfoAndType(weightsPShapeInfo, gradOShapeInfo, false, block.getWorkspace());
    return SHAPELIST(CONSTANT(gradIshapeInfo), CONSTANT(gradWDshapeInfo), CONSTANT(gradWPshapeInfo));
  }

  if (!weightsPShapeInfo && biasShapeInfo) {
    gradBshapeInfo = ShapeBuilders::copyShapeInfoAndType(biasShapeInfo, gradOShapeInfo, false, block.getWorkspace());
    return SHAPELIST(CONSTANT(gradIshapeInfo), CONSTANT(gradWDshapeInfo), CONSTANT(gradBshapeInfo));
  }

  return SHAPELIST(CONSTANT(gradIshapeInfo), CONSTANT(gradWDshapeInfo));
}

}  // namespace ops
}  // namespace sd

#endif
