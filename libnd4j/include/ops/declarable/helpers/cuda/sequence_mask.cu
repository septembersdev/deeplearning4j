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
//  @author GS <sgazeos@gmail.com>
//
#include <ops/declarable/helpers/sequence_mask.h>
#include <execution/cuda/LaunchDims.h>
namespace sd {
namespace ops {
namespace helpers {

template <typename I, typename B>
static SD_KERNEL void sequenceMaskKernel(const void* inputBuf, const sd::LongType* inputShape, void* outputBuf,
                                         const sd::LongType* outputShape, int maxIndex) {
  __shared__ const I* input;
  __shared__ B* output;
  __shared__ sd::LongType inputLen, outputLen;
  if (threadIdx.x == 0) {
    input = reinterpret_cast<const I*>(inputBuf);
    output = reinterpret_cast<B*>(outputBuf);
    inputLen = shape::length(inputShape);
    outputLen = shape::length(outputShape);
  }
  __syncthreads();

  for (auto i = blockIdx.x; i < maxIndex; i += gridDim.x)
    for (auto k = threadIdx.x; k < inputLen; k += blockDim.x)
      if (i < input[shape::getIndexOffset(k, inputShape)])
        output[shape::getIndexOffset(k * maxIndex + i, outputShape)] = B(true);
}

template <typename I, typename B>
static void sequenceMask_(LaunchContext* context, NDArray* input, NDArray* output, int maxIndex) {
  //TODO hello
  dim3 launchDims = getSequenceMaskLaunchDims(maxIndex,*input);
  NDArray::prepareSpecialUse({output}, {input});
  auto stream = context->getCudaStream();
  sequenceMaskKernel<I, B><<<launchDims.x, launchDims.y, launchDims.z, *stream>>>(
      input->specialBuffer(), input->specialShapeInfo(), output->specialBuffer(), output->specialShapeInfo(), maxIndex);
  NDArray::registerSpecialUse({output}, {input});
}

void sequenceMask(sd::LaunchContext* context, NDArray* input, NDArray* output, int maxIndex) {
  BUILD_DOUBLE_SELECTOR(input->dataType(), output->dataType(), sequenceMask_, (context, input, output, maxIndex),
                        SD_INTEGER_TYPES, SD_COMMON_TYPES_EXTENDED);
}

BUILD_DOUBLE_TEMPLATE(template void sequenceMask_,
                      (sd::LaunchContext * context, NDArray* input, NDArray* output, int maxIndex), SD_INTEGER_TYPES,
                      SD_COMMON_TYPES_EXTENDED);
}  // namespace helpers
}  // namespace ops
}  // namespace sd
