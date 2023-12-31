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
// Created by raver119 on 02.09.17.
//
#include <helpers/helper_hash.h>

#include "testlayers.h"

class HashUtilsTests : public NDArrayTests {};

TEST_F(HashUtilsTests, TestEquality1) {
  std::string str("Conv2D");

  sd::LongType hash1 = sd::ops::HashHelper::getInstance().getLongHash(str);
  ASSERT_EQ(-1637140380760460323L, hash1);
}

TEST_F(HashUtilsTests, TestEquality2) {
  std::string str("switch");

  sd::LongType hash1 = sd::ops::HashHelper::getInstance().getLongHash(str);
  ASSERT_EQ(-1988317239813741487L, hash1);
}
