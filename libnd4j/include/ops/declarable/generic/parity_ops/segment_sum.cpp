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
// Created by george@skymind.io on 2/21/2018.
//

#include <ops/declarable/CustomOperations.h>
#include <ops/declarable/helpers/segment.h>
#if NOT_EXCLUDED(OP_segment_sum)
namespace sd {
namespace ops {
CUSTOM_OP_IMPL(segment_sum, 2, 1, false, 0, 0) {
  auto input = INPUT_VARIABLE(0);
  auto idxSegments = INPUT_VARIABLE(1);
  auto segmentedOutput = OUTPUT_VARIABLE(0);
  REQUIRE_TRUE(idxSegments->isVector(), 0, "segment_sum: segment indexes array should be a vector, but it rank is %i.",
               idxSegments->rankOf());
  REQUIRE_TRUE(idxSegments->lengthOf() == input->sizeAt(0), 0,
               "segment_sum: segment indexes array length should be equal to the input first dimension, but %i != %i.",
               idxSegments->lengthOf(), input->sizeAt(0));

  auto expected = NDArrayFactory::create(input->dataType(), 0.f, block.launchContext());
  auto wrong = NDArrayFactory::create(input->dataType(), 0.f, block.launchContext());

  REQUIRE_TRUE(helpers::segmentIndicesValidate(block.launchContext(), idxSegments, expected, wrong), 0,
               "segment_sum: segment indices should be arranged, but %2.1f > %2.1f", expected.e<float>(0),
               wrong.e<float>(0));

  segmentedOutput->nullify();
  helpers::segmentSumFunctor(block.launchContext(), input, idxSegments, segmentedOutput);

  return sd::Status::OK;
}

DECLARE_SHAPE_FN(segment_sum) {
  auto idxVector = INPUT_VARIABLE(1);

  auto in = inputShape->at(0);
  int outRank = shape::rank(in);
  sd::LongType* outputShape = nullptr;
  int val = (*idxVector).e<int>(idxVector->lengthOf() - 1);

  int numOfClasses = static_cast<int>(val) + 1;

  ALLOCATE(outputShape, block.getWorkspace(), shape::shapeInfoLength(outRank), sd::LongType);

  outputShape[0] = outRank;
  outputShape[1] = numOfClasses;
  for (sd::LongType i = 1; i < outRank; ++i) outputShape[i + 1] = shape::sizeAt(in, i);

  ShapeUtils::updateStridesAndType(outputShape, in, shape::order(in));

  return SHAPELIST(CONSTANT(outputShape));
}

CUSTOM_OP_IMPL(segment_sum_bp, 3, 2, false, 0, 0) {
  return helpers::segmentSumFunctorBP(block.launchContext(), INPUT_VARIABLE(0), INPUT_VARIABLE(1), INPUT_VARIABLE(2),
                                      OUTPUT_NULLIFIED(0));
}
DECLARE_SHAPE_FN(segment_sum_bp) {
  auto in = inputShape->at(0);
  auto inIdx = inputShape->at(1);

  sd::LongType* outShape;
  sd::LongType* outIndex;
  COPY_SHAPE(in, outShape);
  COPY_SHAPE(inIdx, outIndex);
  return SHAPELIST(CONSTANT(outShape), CONSTANT(outIndex));
  //             return SHAPELIST(in, inIdx);
}

DECLARE_TYPES(segment_sum) { getOpDescriptor()->setAllowedInputTypes(sd::DataType::ANY)->setSameMode(true); }
DECLARE_TYPES(segment_sum_bp) {
  getOpDescriptor()
      ->setAllowedInputTypes(sd::DataType::ANY)
      ->setAllowedOutputTypes(0, {ALL_FLOATS})
      ->setAllowedOutputTypes(1, {ALL_INTS})
      ->setSameMode(false);
}
}  // namespace ops

}  // namespace sd
#endif
