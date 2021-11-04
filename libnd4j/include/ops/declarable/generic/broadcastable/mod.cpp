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
//  @author raver119@gmail.com
//

#include <system/op_boilerplate.h>
#if NOT_EXCLUDED(OP_mod)

#include <ops/declarable/CustomOperations.h>
#include <ops/declarable/generic/helpers/BroadcastHelper.h>

namespace sd {
namespace ops {
BROADCASTABLE_OP_IMPL(mod, 0, 0) {
  auto x = INPUT_VARIABLE(0);
  auto y = INPUT_VARIABLE(1);
  auto z = OUTPUT_VARIABLE(0);

  BROADCAST_CHECK_EMPTY(x, y, z);

  auto tZ = BroadcastHelper::broadcastApply(BROADCAST(Mod), x, y, z);
  if (tZ == nullptr)
    return sd::Status::KERNEL_FAILURE;
  else if (tZ != z) {
    OVERWRITE_RESULT(tZ);
  }

  return sd::Status::OK;
}

DECLARE_TYPES(mod) {
  getOpDescriptor()
      ->setAllowedInputTypes(0, DataType::ANY)
      ->setAllowedInputTypes(1, DataType::ANY)
      ->setAllowedOutputTypes(0, DataType::INHERIT);
}

DECLARE_TYPES(mod_bp) { getOpDescriptor()->setAllowedInputTypes(DataType::ANY)->setAllowedOutputTypes({ALL_FLOATS}); }

CUSTOM_OP_IMPL(mod_bp, 3, 2, false, 0, 0) {
  // PLEASE NOTE: we're just passing eps down the line here
  auto x = INPUT_VARIABLE(0);
  auto y = INPUT_VARIABLE(1);
  auto epsNext = INPUT_VARIABLE(2);

  auto gradX = OUTPUT_VARIABLE(0);
  auto gradY = OUTPUT_VARIABLE(1);

  gradY->assign(0.0f);
  gradX->assign(0.0f);

  return sd::Status::OK;
}

DECLARE_SHAPE_FN(mod_bp) {
  auto x = inputShape->at(0);
  auto y = inputShape->at(1);
  auto e = inputShape->at(2);

  // eps always has shape of x
  // grad always has shape of y

  sd::LongType *shapeE;
  sd::LongType *shapeG;

  COPY_SHAPE(x, shapeE);
  COPY_SHAPE(y, shapeG);

  auto shapeList = SHAPELIST(CONSTANT(shapeE), CONSTANT(shapeG));

  return shapeList;
}
}  // namespace ops
}  // namespace sd

#endif
