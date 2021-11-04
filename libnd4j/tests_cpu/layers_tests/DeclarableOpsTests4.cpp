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
#include <array/NDArray.h>
#include <array/NDArrayList.h>
#include <helpers/helper_hash.h>
#include <ops/declarable/CustomOperations.h>

#include "testlayers.h"

using namespace sd;
using namespace sd::graph;

class DeclarableOpsTests4 : public testing::Test {
 public:
  DeclarableOpsTests4() {
    printf("\n");
    fflush(stdout);

    sd::ops::adjust_hue op0;
    sd::ops::adjust_saturation op1;
  }
};

template <typename T>
class TypedDeclarableOpsTests4 : public testing::Test {
 public:
  TypedDeclarableOpsTests4() {
    printf("\n");
    fflush(stdout);

    sd::ops::adjust_hue op0;
    sd::ops::adjust_saturation op1;
  }
};

typedef ::testing::Types<double, float> TestingTypes;
TYPED_TEST_CASE(TypedDeclarableOpsTests4, TestingTypes);

//////////////////////////////////////////////////////////////////////
TYPED_TEST(TypedDeclarableOpsTests4, avgpool2d_1) {
  auto x = NDArrayFactory::create<TypeParam>('c', {2, 4, 4, 2});
  auto exp = NDArrayFactory::create<TypeParam>(
      'c', {2, 2, 2, 2},
      {6.f, 7.f, 10.f, 11.f, 22.f, 23.f, 26.f, 27.f, 38.f, 39.f, 42.f, 43.f, 54.f, 55.f, 58.f, 59.f});

  x.linspace(1);

  sd::ops::avgpool2d op;
  auto result = op.evaluate({&x}, {}, {2, 2, 2, 2, 0, 0, 1, 1, 1, 1, 1});

  ASSERT_EQ(sd::Status::OK, result.status());

  auto z = result.at(0);

  ASSERT_TRUE(exp.isSameShape(z));
  ASSERT_TRUE(exp.equalsTo(z));
}

//////////////////////////////////////////////////////////////////////
TYPED_TEST(TypedDeclarableOpsTests4, avgpool2d_2) {
  auto x = NDArrayFactory::create<TypeParam>('c', {2, 4, 4, 2});
  auto exp = NDArrayFactory::create<TypeParam>(
      'c', {2, 2, 2, 2},
      {6.f, 7.f, 10.f, 11.f, 22.f, 23.f, 26.f, 27.f, 38.f, 39.f, 42.f, 43.f, 54.f, 55.f, 58.f, 59.f});

  x.linspace(1);

  sd::ops::avgpool2d op;
  auto result = op.evaluate({&x}, {}, {2, 2, 2, 2, 0, 0, 1, 1, 0, 1, 1});

  ASSERT_EQ(sd::Status::OK, result.status());

  auto z = result.at(0);

  ASSERT_TRUE(exp.isSameShape(z));
  ASSERT_TRUE(exp.equalsTo(z));
}

//////////////////////////////////////////////////////////////////////
TYPED_TEST(TypedDeclarableOpsTests4, avgpool2d_3) {
  auto x = NDArrayFactory::create<TypeParam>('c', {2, 5, 5, 2});
  auto exp = NDArrayFactory::create<TypeParam>(
      'c', {2, 3, 3, 2},
      {
          7.f,  8.f,  11.f, 12.f, 14.f, 15.f, 27.f, 28.f, 31.f, 32.f, 34.f, 35.f, 42.f, 43.f, 46.f, 47.f, 49.f, 50.f,
          57.f, 58.f, 61.f, 62.f, 64.f, 65.f, 77.f, 78.f, 81.f, 82.f, 84.f, 85.f, 92.f, 93.f, 96.f, 97.f, 99.f, 100.f,
      });

  x.linspace(1);

  sd::ops::avgpool2d op;
  auto result = op.evaluate({&x}, {}, {2, 2, 2, 2, 0, 0, 1, 1, 1, 0, 1});

  ASSERT_EQ(sd::Status::OK, result.status());

  auto z = result.at(0);

  ASSERT_TRUE(exp.isSameShape(z));
  ASSERT_TRUE(exp.equalsTo(z));
}

// auto padding{top,right, bottom, left} matching arm_compute
std::tuple<int, int, int, int> getSpecialAutoPadding(int rank) {
  auto extra_pad_x = rank < 1 ? 0 : 32;
  auto pad_x = rank < 1 ? 0 : 4;
  auto pad_y = rank < 2 ? 0 : 4;
  return std::tuple<int, int, int, int>{pad_y, pad_x + extra_pad_x, pad_y, pad_x};
}

TYPED_TEST(TypedDeclarableOpsTests4, avgpool2d_padded_buffer) {
  int top, right, bottom, left;
  std::tie(top, right, bottom, left) = getSpecialAutoPadding(4);

  auto input = NDArrayFactory::create('c', {2, 5, 5, 2}, DataTypeUtils::fromT<TypeParam>(),
                                      {0, 0, top + bottom, left + right}, {0, 0, top, left});
  auto output = NDArrayFactory::create('c', {2, 3, 3, 2}, DataTypeUtils::fromT<TypeParam>(),
                                       {0, 0, top + bottom, left + right}, {0, 0, top, left});
  auto exp = NDArrayFactory::create<TypeParam>(
      'c', {2, 3, 3, 2},
      {
          7.f,  8.f,  11.f, 12.f, 14.f, 15.f, 27.f, 28.f, 31.f, 32.f, 34.f, 35.f, 42.f, 43.f, 46.f, 47.f, 49.f, 50.f,
          57.f, 58.f, 61.f, 62.f, 64.f, 65.f, 77.f, 78.f, 81.f, 82.f, 84.f, 85.f, 92.f, 93.f, 96.f, 97.f, 99.f, 100.f,
      });

  input.linspace(1);

  sd::ops::avgpool2d op;
  auto status = op.execute({&input}, {&output}, {}, {2, 2, 2, 2, 0, 0, 1, 1, 1, 0, 1});

  ASSERT_EQ(sd::Status::OK, status);
  ASSERT_TRUE(exp.isSameShape(output));
  ASSERT_TRUE(exp.equalsTo(output));
}

//////////////////////////////////////////////////////////////////////
TYPED_TEST(TypedDeclarableOpsTests4, avgpool2d_4) {
  auto x = NDArrayFactory::create<TypeParam>('c', {2, 5, 5, 2});
  auto exp = NDArrayFactory::create<TypeParam>(
      'c', {2, 2, 2, 2},
      {7.f, 8.f, 11.f, 12.f, 27.f, 28.f, 31.f, 32.f, 57.f, 58.f, 61.f, 62.f, 77.f, 78.f, 81.f, 82.f});

  x.linspace(1);

  sd::ops::avgpool2d op;
  auto result = op.evaluate({&x}, {}, {2, 2, 2, 2, 0, 0, 1, 1, 0, 1, 1});

  ASSERT_EQ(sd::Status::OK, result.status());

  auto z = result.at(0);

  ASSERT_TRUE(exp.isSameShape(z));
  ASSERT_TRUE(exp.equalsTo(z));
}

//////////////////////////////////////////////////////////////////////
TYPED_TEST(TypedDeclarableOpsTests4, avgpool2d_5) {
  auto x = NDArrayFactory::create<TypeParam>('c', {2, 2, 5, 5});
  auto exp = NDArrayFactory::create<TypeParam>(
      'c', {2, 2, 3, 3}, {1.f,   2.5f, 4.5f, 8.5f,  10.f,  12.f,  18.5f, 20.f,  22.f,  26.f,  27.5f, 29.5f,
                          33.5f, 35.f, 37.f, 43.5f, 45.f,  47.f,  51.f,  52.5f, 54.5f, 58.5f, 60.f,  62.f,
                          68.5f, 70.f, 72.f, 76.f,  77.5f, 79.5f, 83.5f, 85.f,  87.f,  93.5f, 95.f,  97.f});

  x.linspace(1);

  sd::ops::avgpool2d op;
  auto result = op.evaluate({&x}, {}, {2, 2, 2, 2, 1, 1, 1, 1, 0, 0, 0});

  ASSERT_EQ(sd::Status::OK, result.status());

  auto z = result.at(0);

  ASSERT_TRUE(exp.isSameShape(z));
  ASSERT_TRUE(exp.equalsTo(z));
}

//////////////////////////////////////////////////////////////////////
TYPED_TEST(TypedDeclarableOpsTests4, avgpool2d_6) {
  auto x = NDArrayFactory::create<TypeParam>('c', {2, 2, 5, 5});
  auto exp = NDArrayFactory::create<TypeParam>(
      'c', {2, 2, 3, 3}, {0.25f,  1.25f, 2.25f, 4.25f,  10.f,   12.f,   9.25f,  20.f,   22.f,   6.5f,   13.75f, 14.75,
                          16.75f, 35.f,  37.f,  21.75f, 45.f,   47.f,   12.75f, 26.25f, 27.25f, 29.25f, 60.f,   62.f,
                          34.25f, 70.f,  72.f,  19.f,   38.75f, 39.75f, 41.75f, 85.f,   87.f,   46.75f, 95.f,   97.f});

  x.linspace(1);

  sd::ops::avgpool2d op;
  auto result = op.evaluate({&x}, {}, {2, 2, 2, 2, 1, 1, 1, 1, 0, 1, 0});

  ASSERT_EQ(sd::Status::OK, result.status());

  auto z = result.at(0);

  ASSERT_TRUE(exp.isSameShape(z));
  ASSERT_TRUE(exp.equalsTo(z));
}

//////////////////////////////////////////////////////////////////////
TYPED_TEST(TypedDeclarableOpsTests4, avgpool2d_7) {
  auto x = NDArrayFactory::create<TypeParam>('c', {2, 2, 5, 5});
  auto exp = NDArrayFactory::create<TypeParam>(
      'c', {2, 2, 3, 3}, {4.f,   6.f,   7.5f,  14.f,  16.f,  17.5f, 21.5f, 23.5f, 25.f,  29.f,  31.f,  32.5f,
                          39.f,  41.f,  42.5f, 46.5f, 48.5f, 50.f,  54.f,  56.f,  57.5f, 64.f,  66.f,  67.5f,
                          71.5f, 73.5f, 75.f,  79.f,  81.f,  82.5f, 89.f,  91.f,  92.5f, 96.5f, 98.5f, 100.f});

  x.linspace(1);

  sd::ops::avgpool2d op;
  auto result = op.evaluate({&x}, {}, {2, 2, 2, 2, 0, 0, 1, 1, 1, 0, 0});

  ASSERT_EQ(sd::Status::OK, result.status());

  auto z = result.at(0);

  ASSERT_TRUE(exp.isSameShape(z));
  ASSERT_TRUE(exp.equalsTo(z));
}

//////////////////////////////////////////////////////////////////////
TYPED_TEST(TypedDeclarableOpsTests4, avgpool2d_8) {
  auto x = NDArrayFactory::create<TypeParam>('c', {1, 1, 3, 3});
  auto exp = NDArrayFactory::create<TypeParam>('c', {1, 1, 2, 2}, {3.f, 4.f, 6.f, 7.f});

  x.linspace(1);

  sd::ops::avgpool2d op;
  auto result = op.evaluate({&x}, {}, {2, 2, 1, 1, 0, 0, 1, 1, 0, 0, 0});

  ASSERT_EQ(sd::Status::OK, result.status());

  auto z = result.at(0);

  ASSERT_TRUE(exp.isSameShape(z));
  ASSERT_TRUE(exp.equalsTo(z));
}

//////////////////////////////////////////////////////////////////////
TYPED_TEST(TypedDeclarableOpsTests4, avgpool2d_9) {
  auto x = NDArrayFactory::create<TypeParam>('c', {1, 1, 3, 3});
  auto exp = NDArrayFactory::create<TypeParam>('c', {1, 1, 3, 3}, {3.f, 4.f, 4.5f, 6.f, 7.f, 7.5f, 7.5f, 8.5f, 9.f});

  x.linspace(1);

  sd::ops::avgpool2d op;
  auto result = op.evaluate({&x}, {}, {2, 2, 1, 1, 0, 0, 1, 1, 1, 0, 0});

  ASSERT_EQ(sd::Status::OK, result.status());

  auto z = result.at(0);

  // z->printShapeInfo("z shape:");
  // z->printBuffer("z buffer:");

  ASSERT_TRUE(exp.isSameShape(z));
  ASSERT_TRUE(exp.equalsTo(z));
}

//////////////////////////////////////////////////////////////////////
TYPED_TEST(TypedDeclarableOpsTests4, avgpool2d_10) {
  auto input = NDArrayFactory::create<TypeParam>(
      'c', {4, 10, 10, 3},
      {9.37125111f,  2.20166993f,  2.91434479f,  5.43639755f,  -2.10573769f, 4.08528662f,  5.86908436f,  -4.46203756f,
       2.21057916f,  5.35849190f,  0.01394637f,  4.40566349f,  7.07982206f,  -0.09633455f, 2.42429352f,  3.97301817f,
       -1.89553940f, 1.99690318f,  6.33141708f,  0.55401880f,  1.70707977f,  5.55204201f,  -0.03513752f, 1.60011971f,
       2.62700319f,  -2.74582434f, 3.06697464f,  1.06277943f,  -1.16075921f, -0.78095782f, 9.72352791f,  -1.22686064f,
       1.99644792f,  7.35571337f,  1.40607321f,  0.11390255f,  9.53334427f,  2.28303599f,  -1.66728830f, 6.16678810f,
       -0.04532295f, -1.97708666f, 9.74906158f,  1.46223176f,  -1.46734393f, 4.30761862f,  -1.23790228f, 1.24823606f,
       6.13938427f,  -3.83689475f, -1.19625473f, 7.91535568f,  6.05868721f,  -3.22946382f, 8.81633949f,  -0.19967777f,
       0.66053957f,  2.30919123f,  0.74543846f,  -0.39347672f, 11.11058044f, 0.53720862f,  1.52645731f,  5.70012379f,
       -1.15213466f, 1.16451406f,  7.00526333f,  1.57362783f,  -2.44384766f, 5.54213285f,  -1.98828590f, -0.70483637f,
       7.88281822f,  -3.59875536f, 0.80745387f,  13.41578484f, -1.55507684f, -0.65855008f, 9.32583523f,  -0.14544789f,
       0.73436141f,  3.61176538f,  -1.71268058f, -2.58490300f, 9.09280205f,  -3.27405524f, -2.04569697f, 4.44761324f,
       -0.62955856f, -2.61917663f, 8.04890442f,  0.54579324f,  0.85929775f,  9.82259560f,  -1.93825579f, 0.77703512f,
       4.67090321f,  -4.79267597f, -2.38906908f, 9.31265545f,  0.96026313f,  -1.14109385f, 11.54231834f, -0.01417295f,
       -0.39500344f, 8.49191666f,  0.55300158f,  2.79490185f,  6.92466164f,  1.72254205f,  2.82222271f,  8.83112717f,
       2.95033407f,  2.18054962f,  6.73509789f,  -2.22272944f, 0.51127720f,  -1.04563558f, 2.15747333f,  -2.30959272f,
       9.55441570f,  1.50396204f,  1.77370787f,  7.38146257f,  -1.79076433f, 3.20961165f,  7.18864202f,  2.91217351f,
       0.43018937f,  7.11078024f,  -1.17386127f, -0.16817921f, 6.12327290f,  -2.82205725f, 3.30696845f,  13.51291752f,
       -1.30856836f, -2.38332748f, 11.09487438f, -1.47190213f, -0.53050828f, 4.38285351f,  -5.07309771f, 1.50714362f,
       5.72274446f,  -2.85825086f, -0.89673209f, 3.73791552f,  -0.67708802f, -4.13149452f, -0.00671843f, -0.26566532f,
       0.32961160f,  7.14501762f,  -1.41608179f, -4.96590328f, 12.26205540f, -0.65158135f, -0.88641000f, 6.95777559f,
       -0.79058206f, -0.10260171f, 7.87169170f,  1.35921454f,  1.11759663f,  5.46187401f,  -2.57214499f, 2.48484039f,
       4.04043484f,  -2.07137156f, -1.42709637f, 9.25487137f,  -0.12605135f, -2.66949964f, 2.89412403f,  0.74451172f,
       -2.96250391f, 3.99258423f,  0.27084303f,  0.32213116f,  5.42332172f,  -0.44414216f, 1.70881832f,  6.69346905f,
       0.53058422f,  -4.73146200f, 4.22051668f,  2.24834967f,  0.66996074f,  4.30173683f,  0.11849818f,  -4.07520294f,
       8.27318478f,  -2.54398274f, -2.86705542f, 10.11775303f, -0.99382895f, 0.65881538f,  7.93556786f,  -1.27934420f,
       -1.69343162f, 9.68042564f,  -1.02609646f, -1.18189347f, 5.75370646f,  -1.67888868f, -4.48871994f, 4.79537392f,
       -0.79212248f, -0.19855022f, 6.15060997f,  -0.01081491f, 3.64454579f,  10.82562447f, 1.58859253f,  -2.65847278f,
       8.60093212f,  -1.59196103f, 0.07635692f,  11.76175690f, -1.17453325f, 0.10122013f,  6.86458445f,  -2.18891335f,
       -2.74004745f, 8.07066154f,  0.71818852f,  -2.03035975f, 6.31053686f,  0.51509416f,  1.39789927f,  9.43515587f,
       2.04256630f,  0.13985133f,  4.65010691f,  2.40911126f,  -0.36255789f, -3.06867862f, -0.45225358f, -1.56778407f,
       6.05917358f,  -1.09891272f, 1.77184200f,  6.46248102f,  0.96042323f,  -0.24346280f, 4.63436460f,  -4.69907761f,
       1.25187206f,  11.46173859f, -2.21917558f, 1.28007793f,  6.92173195f,  2.11268163f,  -3.47389889f, 5.08722782f,
       -3.03950930f, -4.17154264f, 11.30568314f, 0.80361372f,  2.53214502f,  7.18707085f,  -4.49114513f, 2.85449266f,
       10.14906883f, -0.31974933f, -0.84472644f, -0.52459574f, 0.12921631f,  -1.81390119f, 2.76170087f,  1.03982210f,
       2.91744232f,  -0.29048753f, 5.87453508f,  -1.53684759f, 1.85800636f,  -0.91404629f, 1.28954852f,  5.11354685f,
       -2.47475505f, -1.33179152f, 2.58552408f,  1.37316465f,  -3.32339454f, 1.54122913f,  3.24953628f,  -0.29758382f,
       2.82391763f,  -1.51142192f, -1.22699404f, 6.75745535f,  0.65452754f,  -3.29385471f, 2.06008053f,  2.53172946f,
       -4.23532820f, -1.53909743f, -0.07010663f, -1.42173731f, 7.29031610f,  -0.18448229f, 4.59496164f,  6.73027277f,
       0.73441899f,  0.14426160f,  4.14915276f,  -2.97010231f, 6.05851364f,  4.95218086f,  -2.39145470f, 2.40494704f,
       2.10288811f,  0.53503096f,  1.44511235f,  6.66344261f,  -3.05803776f, 7.21418667f,  3.30303526f,  -0.24163735f,
       3.47409391f,  3.64520788f,  2.15189481f,  -3.11243272f, 3.62310791f,  0.37379482f,  0.40865007f,  -0.83132005f,
       -4.78246069f, 2.07030797f,  6.51765442f,  3.16178989f,  5.06180477f,  3.78434467f,  -0.96689719f, 0.35965276f,
       5.89967585f,  1.40294051f,  1.11952639f,  10.59778214f, 0.26739889f,  -1.61297631f, 6.24801159f,  -0.93914318f,
       -0.57812452f, 9.92604542f,  -0.73025000f, -3.38530874f, 2.45646000f,  -2.47949195f, 0.51638460f,  10.65636063f,
       1.97816694f,  -3.00407791f, 2.66914415f,  -0.81951088f, -0.23316640f, 2.40737987f,  -2.70007610f, 1.51531935f,
       4.08860207f,  -0.27552786f, -1.31721711f, 7.11568260f,  -3.33498216f, -4.02545023f, 7.22675610f,  -0.81690705f,
       -2.52689576f, 1.04016697f,  -0.79291463f, -0.34875512f, 10.00498390f, -4.24167728f, 1.46162593f,  11.82569408f,
       -1.70359993f, -0.30161047f, 16.44085884f, -0.82253462f, -0.09435523f, 6.13080597f,  -0.20259480f, 0.68308711f,
       6.15663004f,  -6.61776876f, 0.33295766f,  2.55449438f,  -0.17819691f, -1.14892209f, 5.56776142f,  1.99279118f,
       1.33035934f,  4.45823956f,  3.34916544f,  -2.59905386f, 6.16164446f,  -2.03881931f, -2.45273542f, 12.46793365f,
       -2.22743297f, 2.83738565f,  8.48628139f,  -1.39347959f, -1.30867767f, 11.08041477f, -4.00363779f, 2.09183025f,
       11.30395889f, -2.20504737f, 1.37426853f,  8.98735619f,  1.04676604f,  -0.72757077f, 8.28050232f,  -6.70741081f,
       -0.65798020f, 5.68592072f,  -0.60760021f, 0.35854483f,  6.26852131f,  1.94100165f,  1.32112014f,  0.80987954f,
       -1.74617672f, -0.25434083f, 7.16045523f,  1.58884013f,  -2.64847064f, 13.14820385f, 1.21393633f,  -2.47258949f,
       9.41650105f,  -0.79384226f, 2.48954105f,  10.95629311f, 0.47723705f,  4.02126694f,  8.02593136f,  -2.20726371f,
       -1.18794477f, 1.50836647f,  0.93118095f,  -1.73513174f, 8.85493565f,  -2.99670315f, -0.79055870f, 2.39473820f,
       2.05046916f,  -2.38055134f, 11.82299423f, 0.15609655f,  0.68744308f,  5.66401434f,  -0.69281673f, 2.09855556f,
       7.74626589f,  -0.34283102f, 1.00542057f,  9.95838642f,  0.80161905f,  2.33455157f,  9.80057335f,  -0.93561798f,
       2.56991577f,  8.29711342f,  0.94213426f,  0.44209945f,  11.70259857f, 0.92710167f,  2.60957146f,  0.24971688f,
       -0.86529571f, 3.78628922f,  6.80884457f,  -0.68178189f, 2.21103406f,  3.18895817f,  0.60283208f,  -2.92716241f,
       6.72060776f,  -1.06625068f, 2.56543374f,  9.97404480f,  3.58080721f,  -0.94936347f, 10.16736984f, -1.38464379f,
       1.18191063f,  6.66179037f,  -3.56115270f, 0.32329530f,  10.90870762f, 2.20638227f,  0.19653285f,  7.34650040f,
       -3.63859272f, -1.03027737f, 5.98829985f,  -3.66606474f, -3.89746714f, 8.63469028f,  1.22569811f,  1.63240814f,
       3.74385309f,  0.58243257f,  -0.56981975f, 3.69260955f,  1.00979900f,  -1.44030499f, 8.57058144f,  -1.10648811f,
       1.20474911f,  5.43133020f,  -2.14822555f, -0.07928789f, 11.25825310f, 0.19645604f,  -5.49546146f, 10.41917038f,
       -0.68178523f, -2.99639869f, 6.50054455f,  0.46488351f,  -5.42328453f, 9.09500027f,  -2.82107449f, 0.05601966f,
       15.34610748f, -0.06820253f, 3.86699796f,  10.73316956f, -3.04795432f, -0.14702171f, 5.64813185f,  1.44028485f,
       -2.47596145f, 0.07280898f,  -3.03187990f, -1.35183525f, 9.35835648f,  2.72966957f,  1.88199532f,  10.36187744f,
       -0.22834805f, -3.26738238f, 6.92025137f,  -2.34061313f, 4.77379704f,  5.28559113f,  -2.96323752f, -1.76186585f,
       5.94436455f,  0.38647744f,  -5.73869514f, 6.76849556f,  1.40892124f,  -1.19068217f, 5.37919092f,  -6.65328646f,
       3.62782669f,  12.34744644f, 2.44762444f,  -4.19242620f, 6.14906216f,  0.08121119f,  0.61355996f,  2.69666457f,
       -1.88962626f, -0.55314136f, 1.84937525f,  1.56048691f,  1.17460012f,  3.75674725f,  1.06198275f,  -5.74625874f,
       5.41645575f,  -1.28946674f, -1.51689398f, 4.32400894f,  -0.05222082f, -4.83948946f, 1.80747867f,  1.63144708f,
       -2.73887825f, 1.63975775f,  -2.02163982f, -0.16210437f, 2.93518686f,  1.14427686f,  -2.83246303f, 4.79283667f,
       2.69697428f,  -3.12678456f, -1.19225168f, -2.37022972f, -3.09429741f, 1.94225383f,  -1.13747168f, -2.55048585f,
       5.40242243f,  1.12777328f,  3.43713188f,  3.62658787f,  -2.16878843f, 0.30164462f,  2.97407579f,  -0.07275413f,
       -1.31149673f, 4.70066261f,  -2.01323795f, 4.85255766f,  4.59128904f,  1.68084168f,  1.60336494f,  6.58138466f,
       -1.04759812f, 2.69906545f,  3.55769277f,  -0.74327278f, 2.65819693f,  5.39528131f,  2.11248922f,  -1.06446671f,
       5.24546766f,  -2.43146014f, 4.58907509f,  0.06521678f,  -2.24503994f, 2.45722699f,  6.94863081f,  0.35258654f,
       2.83396196f,  9.92525196f,  -1.12225175f, -0.34365177f, 7.19116688f,  -4.39813757f, 0.46517885f,  13.22028065f,
       -2.57483673f, -6.37226963f, 7.58046293f,  -2.74600363f, 0.42231262f,  8.04881668f,  0.17289802f,  -0.53447008f,
       16.55157471f, -5.63614368f, 0.39288223f,  3.37079263f,  1.26484549f,  -0.12820500f, 8.46440125f,  -4.39304399f,
       2.97676420f,  0.65650189f,  0.83158541f,  -1.11556435f, 6.32885838f,  -0.36087769f, 2.80724382f,  9.90292645f,
       1.15936041f,  0.20947981f,  6.91249275f,  -2.67404819f, 2.93782163f,  6.65656614f,  -2.30828357f, 2.98214006f,
       6.80611229f,  -4.93821478f, -7.66555262f, 7.59763002f,  -0.54159302f, 3.87403512f,  12.42607784f, 2.59284401f,
       -0.23375344f, 8.95293331f,  -0.71807784f, 0.61873478f,  8.66713524f,  1.24289191f,  -2.37835455f, 2.08071637f,
       -0.88315344f, -3.41891551f, 6.85245323f,  1.73007369f,  1.02169311f,  7.69170332f,  -2.85411978f, 2.69790673f,
       8.12906551f,  -1.19351399f, -2.26442742f, 12.26104450f, -0.75579089f, -1.73274946f, 10.68729019f, 2.20655656f,
       -0.90522075f, 12.42165184f, -1.67929137f, 2.44851565f,  9.31565762f,  -0.06645700f, 1.52762020f,  6.18427515f,
       -1.68882596f, 3.70261097f,  3.02252960f,  -3.44125366f, -1.31575799f, 2.84617424f,  -0.96849400f, -4.52356243f,
       9.95027161f,  0.19966406f,  -0.78874779f, 8.18595028f,  -4.08300209f, 1.75126517f,  0.96418417f,  -4.04913044f,
       -0.95200396f, 12.03637886f, -0.03041124f, 0.41642749f,  8.88267422f,  -3.24985337f, -2.24919462f, 7.32566118f,
       0.16964148f,  -2.74123430f, 7.05264473f,  -3.30191112f, 0.17163286f,  4.81851053f,  -1.64463484f, -0.85933101f,
       7.29276276f,  2.34066939f,  -2.14860010f, 3.46148157f,  -0.01782012f, 1.51504040f,  4.79304934f,  1.85281146f,
       -1.70663762f, 6.93470192f,  -4.15440845f, -1.25983095f, 10.52491760f, 0.42930329f,  -1.85146868f, 11.70042324f,
       -0.41704914f, 3.83796859f,  9.21148491f,  -2.79719448f, 0.79470479f,  6.26926661f,  -5.85230207f, 3.95105338f,
       7.84790897f,  -1.38680744f, -1.78099084f, 11.95235348f, -2.99841452f, -1.34507811f, 6.15714645f,  -1.07552516f,
       -2.81228638f, 1.66234732f,  -4.55166149f, -1.92601109f, 8.64634514f,  -0.48158705f, 3.31595659f,  7.67371941f,
       2.56964207f,  0.12107098f,  4.56467867f,  -0.93541539f, 1.39432955f,  11.99714088f, 1.05353570f,  -2.13099813f,
       3.67617917f,  3.45895386f,  1.37365830f,  8.74344158f,  -4.17585802f, 1.43908918f,  6.28764772f,  3.97346330f,
       -0.69144285f, 9.07983303f,  -0.41635889f, -0.14965028f, 8.85469818f,  1.11306190f,  2.59440994f,  5.38982344f,
       -1.07948279f, 1.37252975f,  10.26984596f, -0.09318046f, 2.73104119f,  12.45902252f, -1.55446684f, -2.76124811f,
       12.19395065f, -0.51846564f, 1.02764034f,  11.42673588f, -0.95940983f, -0.04781032f, 8.78379822f,  -4.88957930f,
       0.32534006f,  11.97696400f, -3.35108662f, 1.95104563f,  4.46915388f,  -2.32061648f, 3.45230985f,  8.29983711f,
       2.81034684f,  -2.35529327f, 6.07801294f,  -0.98105043f, -0.05359888f, 2.52291036f,  -0.01986909f, -2.35321999f,
       10.51954269f, 2.11145401f,  3.53506470f,  7.29093266f,  0.03721160f,  -1.13496494f, 7.43886709f,  -5.84201956f,
       2.50796294f,  12.14647675f, 2.77490377f,  -2.18896222f, 6.05641937f,  5.32617044f,  1.04221284f,  10.79106712f,
       -2.95749092f, -2.75414610f, 11.30037117f, -3.40654182f, -2.24673963f, 7.49126101f,  0.70811015f,  -6.18003702f,
       13.83951187f, -1.01204085f, 1.36298490f,  -1.04451632f, 2.42435336f,  -0.02346706f, -0.85528886f, 1.04731262f,
       0.22192979f,  4.15708160f,  0.34933877f,  0.04814529f,  2.24107265f,  0.49676740f,  -1.47752666f, 0.45040059f,
       -0.70471478f, -1.19759345f, 0.21711677f,  0.88461423f,  -2.76830935f, 5.52066898f,  1.97664857f,  -1.75381601f,
       3.45877838f,  1.52617192f,  -1.61350942f, 0.85337949f,  1.97610760f,  -3.40310287f, 3.40319014f,  -3.38691044f,
       -0.71319139f, 1.65463758f,  -0.60680127f, -1.80700517f, 8.02592373f,  2.59627104f,  2.65895891f,  5.93043184f,
       -4.48425817f, 3.92670918f,  4.19496679f,  -2.28286791f, 6.41634607f,  5.72330523f,  1.16269672f,  -0.28753027f,
       2.46342492f,  0.36693189f,  0.26712441f,  6.37652683f,  -2.50139046f, 2.43923736f,  5.56310415f,  0.98065847f,
       1.04267502f,  4.16403675f,  -0.04966142f, 4.40897894f,  3.72905660f,  -3.46129870f, 3.59962773f,  1.34830284f,
       -1.76661730f, 0.47943926f,  5.29946661f,  -1.12711561f, 1.26970029f,  15.17655945f, -1.50971997f, 5.81345224f,
       8.48562050f,  -4.36049604f, 2.48144460f,  8.23780441f,  -3.46030426f, -0.84656560f, 5.94946814f,  1.12747943f,
       -2.65683913f, 8.69085693f,  1.31309867f,  -2.79958344f, 8.76840591f,  -1.56444156f, 1.62710834f,  2.41177034f,
       -0.72804940f, 5.70619011f,  4.67169666f,  -0.86167198f, -1.83803177f, 2.96346045f,  2.82692933f,  -2.81557131f,
       7.11113358f,  -1.90071094f, 2.54244423f,  11.19284058f, -0.06298946f, -1.71517313f, 12.98388577f, 0.84510714f,
       3.00816894f,  2.57200313f,  0.03899818f,  -1.49330592f, 9.60099125f,  -3.59513044f, -1.30045319f, 7.09241819f,
       -0.65233821f, -2.33627677f, 8.81366920f,  0.84154201f,  1.03312039f,  9.85289097f,  0.19351870f,  1.78496623f,
       7.34631205f,  -2.16530800f, -0.65016162f, 2.46842360f,  0.24016285f,  -1.24308395f, 4.78175163f,  -0.97682536f,
       2.20942235f,  6.68382788f,  3.76786447f,  -1.44454038f, 6.26453733f,  -3.23575711f, -2.30137897f, 9.53092670f,
       -5.55222607f, 3.25999236f,  9.37559509f,  1.86339056f,  -0.23551451f, 10.23400211f, 3.93031883f,  -0.52629089f,
       7.85724449f,  -2.91549587f, 4.46612740f,  5.66530371f,  -2.70820427f, 4.81359577f,  10.31247330f, 1.92230141f,
       2.53931546f,  0.74986327f,  1.70303428f,  0.48063779f,  5.31099129f,  -0.78976244f, 3.75864220f,  4.23051405f,
       2.34042454f,  -7.98193836f, 9.83987141f,  -1.46722627f, 3.54497814f,  10.36455154f, -4.51249075f, 0.77715248f,
       7.78694630f,  -4.59989023f, -2.49585629f, 9.90296268f,  1.38535416f,  1.17441154f,  10.10452843f, -0.98628229f,
       0.60194463f,  9.12639141f,  -3.90754628f, 2.88526392f,  7.24123430f,  -0.15283313f, -0.75728363f, -1.15116858f,
       -2.53791571f, 0.77229571f,  6.44114161f,  0.02646767f,  4.95463037f,  7.21066380f,  1.79384065f,  0.73250306f,
       8.04447937f,  0.32576546f,  -0.79447043f, 10.12717724f, 2.33392906f,  1.30716443f,  12.36073112f, -0.36694977f,
       -1.20438910f, 7.03105593f,  0.59557682f,  0.69267452f,  10.18113136f, 2.49944925f,  -0.42229167f, 8.83143330f,
       -1.18805945f, -2.87509322f, 4.53596449f,  4.09732771f,  -3.39088297f, -1.02536607f, 0.82119560f,  -3.47302604f,
       9.29991817f,  0.21001509f,  4.97036457f,  9.50018406f,  1.04420102f,  1.96560478f,  10.74769592f, -6.22709799f,
       3.11690164f,  5.06759691f,  -1.23724771f, -3.05831861f, 8.12925529f,  -1.93435478f, -1.10151744f, 9.32263088f,
       -0.04249470f, -5.98547363f, 10.49398136f, 0.26400441f,  -0.78915191f, 13.28219604f, 2.99276900f,  0.74853164f,
       2.49364305f,  -3.43529654f, 4.05278301f,  2.13498688f,  -2.35444307f, -0.79900265f, 4.66968822f,  -0.31095147f,
       3.60674143f,  12.37222099f, -0.07855003f, -3.30292702f, 12.15215874f, 0.60886210f,  2.87075138f,  7.75271845f,
       0.38044083f,  3.34402204f,  6.40583277f,  -0.87888050f, 0.67438459f,  6.91080809f,  1.98332930f,  -0.08303714f,
       8.08630371f,  -0.16772588f, -2.74058914f, 7.17253590f,  -2.69122696f, 1.48173678f,  8.99470139f,  -1.43302310f,
       -0.88651133f, 2.66944790f,  -0.29186964f, 2.00838661f,  5.09587479f,  -0.76676071f, -2.88322186f, 8.31110573f,
       -0.14550979f, -1.37726915f, 10.28355122f, -1.60575438f, -0.04118848f, 9.97510815f,  0.14440438f,  -3.24632120f,
       9.00034523f,  4.14319563f,  -1.31023729f, 7.16950464f,  -0.70428526f, 2.01559544f,  7.26155043f,  2.40816474f,
       2.09847403f,  7.31264496f,  -0.75401551f, 2.13392544f,  7.03648758f,  1.04036045f,  -1.15636516f, 1.09634531f,
       -0.06340861f, -0.58107805f, -0.65623116f, 1.18972754f,  -0.80717683f, 1.40118241f,  -0.61932516f, -3.60596156f,
       1.59904599f,  -2.23774099f, -1.13721037f, 3.89620137f,  -0.09115922f, -7.51356888f, 2.36975193f,  -1.42520905f,
       -2.34173775f, 3.33830214f,  -2.74016523f, -3.04115510f, 6.00119495f,  -1.36084354f, -2.45065260f, 4.56992292f,
       -3.02825928f, -3.74182844f, 5.11069250f,  -0.91531068f, -2.31385994f, 1.83399653f,  3.39370203f,  -3.60886002f});
  auto exp = NDArrayFactory::create<TypeParam>(
      'c', {4, 4, 4, 3},
      {7.97172260f,  0.06878620f,  2.27749538f,  7.29276514f,  -0.14074677f, 0.65480286f,  5.70313978f,  -0.06546132f,
       0.35443667f,  3.70382833f,  -0.84020567f, 0.63826996f,  8.60301399f,  -0.38236514f, 1.55177069f,  7.37542057f,
       -0.99374938f, -0.29971302f, 8.84352493f,  -0.67121059f, 0.43132120f,  4.78175592f,  -1.25070143f, -1.91523600f,
       6.03855371f,  -0.00292124f, -1.11214364f, 7.90158176f,  -0.57949901f, -0.96735370f, 7.81192017f,  -0.53255427f,
       -0.48009714f, 3.16953635f,  0.08353355f,  -1.54299748f, 3.74821687f,  1.69396687f,  0.72724354f,  5.42915201f,
       -1.13686812f, -0.71793109f, 5.78376389f,  -0.72239977f, -0.60055625f, 2.53636408f,  0.56777251f,  -2.07892323f,
       6.08064651f,  0.68620735f,  2.54017019f,  5.65828180f,  -0.68255502f, 1.47283304f,  6.10842514f,  -0.39655915f,
       0.28380761f,  1.96707797f,  -1.98206317f, 0.94027776f,  4.71811438f,  0.32104525f,  -0.92409706f, 8.34588146f,
       -1.05581069f, -0.55217457f, 9.58440876f,  -0.96549922f, 0.45820439f,  5.65453672f,  -2.50953507f, -0.71441835f,
       8.03059578f,  -0.21281289f, 0.92125505f,  9.26900673f,  -0.35963219f, -0.70039093f, 8.59924412f,  -1.22358346f,
       0.81318003f,  3.85920119f,  -0.01305223f, -1.09234154f, 6.33158875f,  1.28094780f,  -1.48926139f, 4.94969177f,
       -0.77126902f, -1.97033751f, 5.64381838f,  -0.16285487f, -1.31277227f, 2.39893222f,  -1.32902908f, -1.39609122f,
       6.47572327f,  -0.45267010f, 1.55727172f,  6.70965624f,  -1.68735468f, -0.05672536f, 7.25092363f,  -0.64613032f,
       0.67050058f,  3.60789680f,  -2.05948973f, 2.22687531f,  8.15202713f,  -0.70148355f, 1.28314006f,  8.14842319f,
       -1.88807654f, -1.04808438f, 8.45500565f,  -0.76425624f, 0.94542569f,  4.56179953f,  -0.28786001f, -2.04502511f,
       8.46278095f,  -0.31019822f, 0.07339200f,  9.34214592f,  -0.61948007f, 0.52481830f,  8.32515621f,  -1.52418160f,
       0.49678251f,  5.11082315f,  -1.09908783f, -0.52969611f, 5.27806664f,  0.88632923f,  0.66754371f,  4.75839233f,
       0.48928693f,  -0.68036932f, 6.56925392f,  -0.02949905f, -2.99189186f, 4.46320581f,  -0.64534980f, -0.29516968f,
       8.60809517f,  -1.13120568f, 3.41720533f,  5.84243155f,  -1.24109328f, 0.89566326f,  5.99578333f,  -0.42496428f,
       2.07076764f,  3.17812920f,  -0.81566459f, -0.14363396f, 6.55184317f,  0.39633346f,  -0.43852386f, 8.70214558f,
       -2.24613595f, 0.30708700f,  8.73882294f,  -0.53545928f, 1.54409575f,  4.49452257f,  -0.16509305f, 0.19028664f,
       8.24897003f,  0.44750381f,  2.15448594f,  8.97640514f,  -0.77728152f, 0.57272542f,  9.03467560f,  0.47173575f,
       -1.10807717f, 3.30056310f,  -0.43268481f, -0.41470885f, 3.53798294f,  -0.08546703f, -2.16840744f, 6.18733406f,
       -0.17871059f, -2.59837723f, 5.94218683f,  -1.02990067f, -0.49760687f, 3.76938033f,  0.86383581f,  -1.91504073f});

  sd::ops::avgpool2d op;
  auto result = op.evaluate({&input}, {3, 3, 3, 3, 0, 0, 1, 1, 1, 0, 1});
  ASSERT_EQ(sd::Status::OK, result.status());

  auto z = result.at(0);

  // z->printIndexedBuffer("z");
  // exp.printIndexedBuffer("e");

  ASSERT_TRUE(exp.isSameShape(z));
  ASSERT_TRUE(exp.equalsTo(z));
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests4, avgpool2d_11) {
  int inOutH = 5;   // 35;
  int inOutW = 5;   // 35;
  int inOutC = 10;  // 192;

  auto x = NDArrayFactory::create<double>('c', {1, inOutH, inOutW, inOutC});
  x.linspace(1.0);

  sd::ops::avgpool2d op;
  auto result = op.evaluate({&x}, {3, 3, 1, 1, 0, 0, 1, 1, 1, 0, 1});
  ASSERT_EQ(sd::Status::OK, result.status());

  auto z = result.at(0);

  int totalPadHeight = (inOutH - 1) * 1 + 3 - inOutH;
  int padTop = totalPadHeight / 2;
  int padBottom = totalPadHeight - totalPadHeight / 2;

  int k = 3;

  auto m = NDArrayFactory::create<double>('c', {1, inOutH, inOutW, inOutC});
  auto c = NDArrayFactory::create<double>('c', {1, inOutH, inOutW, inOutC});

  for (int h = 0; h < inOutH; h++) {
    for (int w = 0; w < inOutW; w++) {
      int hFrom = h - padTop;
      int wFrom = w - padBottom;

      int hTo = hFrom + k;
      int wTo = wFrom + k;

      hFrom = sd::math::sd_max<int>(0, hFrom);
      wFrom = sd::math::sd_max<int>(0, wFrom);

      hTo = sd::math::sd_min<int>(inOutH, hTo);
      wTo = sd::math::sd_min<int>(inOutW, wTo);

      int idxOut[4];
      int idxIn[4];
      for (int ch = 0; ch < inOutC; ch++) {
        idxOut[1] = h;
        idxOut[2] = w;
        idxOut[3] = ch;
        idxIn[3] = ch;

        for (int kh = hFrom; kh < hTo; kh++) {
          for (int kw = wFrom; kw < wTo; kw++) {
            idxIn[1] = kh;
            idxIn[2] = kw;

            auto inVal = x.e<double>(0, kh, kw, ch);
            m.p(0, h, w, ch, inVal + m.e<double>(0, h, w, ch));
            c.p(0, h, w, ch, 1 + c.e<int>(0, h, w, ch));
          }
        }
      }
    }
  }
  m /= c;

  ASSERT_EQ(m, *z);
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests4, avgpool2d_12) {
  int bS = 4, iH = 10, iW = 10, iC = 3, kH = 3, kW = 3, sH = 3, sW = 3, pH = 0, pW = 0, dH = 1, dW = 1;
  int oH = 4, oW = 4;
  int paddingMode = 1;  // 1-SAME,  0-VALID
  int dataFormat = 1;   // 1-NHWC, 0-NDHW

  auto input = NDArrayFactory::create<double>('c', {bS, iH, iW, iC});
  auto expected = NDArrayFactory::create<double>(
      'c', {bS, oH, oW, iC},
      {17.5,   18.5,   19.5,   25.,   26.,   27.,   34.,   35.,    36.,    41.5,   42.5,   43.5,   92.5,   93.5,
       94.5,   100.,   101.,   102.,  109.,  110.,  111.,  116.5,  117.5,  118.5,  182.5,  183.5,  184.5,  190.,
       191.,   192.,   199.,   200.,  201.,  206.5, 207.5, 208.5,  257.5,  258.5,  259.5,  265.,   266.,   267.,
       274.,   275.,   276.,   281.5, 282.5, 283.5, 317.5, 318.5,  319.5,  325.,   326.,   327.,   334.,   335.,
       336.,   341.5,  342.5,  343.5, 392.5, 393.5, 394.5, 400.,   401.,   402.,   409.,   410.,   411.,   416.5,
       417.5,  418.5,  482.5,  483.5, 484.5, 490.,  491.,  492.,   499.,   500.,   501.,   506.5,  507.5,  508.5,
       557.5,  558.5,  559.5,  565.,  566.,  567.,  574.,  575.,   576.,   581.5,  582.5,  583.5,  617.5,  618.5,
       619.5,  625.,   626.,   627.,  634.,  635.,  636.,  641.5,  642.5,  643.5,  692.5,  693.5,  694.5,  700.,
       701.,   702.,   709.,   710.,  711.,  716.5, 717.5, 718.5,  782.5,  783.5,  784.5,  790.,   791.,   792.,
       799.,   800.,   801.,   806.5, 807.5, 808.5, 857.5, 858.5,  859.5,  865.,   866.,   867.,   874.,   875.,
       876.,   881.5,  882.5,  883.5, 917.5, 918.5, 919.5, 925.,   926.,   927.,   934.,   935.,   936.,   941.5,
       942.5,  943.5,  992.5,  993.5, 994.5, 1000., 1001., 1002.,  1009.,  1010.,  1011.,  1016.5, 1017.5, 1018.5,
       1082.5, 1083.5, 1084.5, 1090., 1091., 1092., 1099., 1100.,  1101.,  1106.5, 1107.5, 1108.5, 1157.5, 1158.5,
       1159.5, 1165.,  1166.,  1167., 1174., 1175., 1176., 1181.5, 1182.5, 1183.5});
  input.linspace(1.);

  sd::ops::avgpool2d op;
  auto results = op.evaluate({&input}, {kH, kW, sH, sW, pH, pW, dH, dW, paddingMode, 0, dataFormat});
  auto output = results.at(0);

  ASSERT_EQ(sd::Status::OK, results.status());

  // output->printIndexedBuffer("output");
  // expected.printIndexedBuffer("expected");

  ASSERT_TRUE(expected.isSameShape(output));
  ASSERT_TRUE(expected.equalsTo(output));
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests4, avgpool2d_13) {
  const int bS = 2;   // batch size
  const int iD = 1;   // input depth (number of picture channels, for example rgb=3)
  const int iH = 28;  // picture height in pixels
  const int iW = 28;  // picture width in pixels
  const int kH = 5;   // kernel height in pixels
  const int kW = 5;   // kernel width in pixels
  const int sH = 1;   // stride step in horizontal direction
  const int sW = 1;   // stride step in vertical direction
  const int pH = 0;   // padding height
  const int pW = 0;   // padding width
  const int dH = 2;   // dilation height
  const int dW = 2;   // dilation width
  const int oH = (iH - kH - (kH - 1) * (dH - 1) + 2 * pH) / sH + 1;  // output height
  const int oW = (iW - kW - (kW - 1) * (dW - 1) + 2 * pW) / sW + 1;  // output width

  auto x = NDArrayFactory::create_<float>('c', {bS, iD, iH, iW});
  auto exp = NDArrayFactory::create<float>('c', {bS, iD, oH, oW});
  // auto z('c',{bS,iD,oH,oW});

  std::unique_ptr<VariableSpace> variableSpace(new VariableSpace());
  variableSpace->putVariable(-1, x);
  // variableSpace->putVariable(1, &z);
  std::unique_ptr<Context> block(new Context(1, variableSpace.get(), false));
  block->fillInputs({-1});
  std::vector<int>* argI = block->getIArguments();
  *argI = {kH, kW, sH, sW, pH, pW,
           dW, dH, 0,  0,  0};  // 0,1 - kernel Height/Width; 2,3 - stride Height/Width; 4,5 - pad Height/Width; 6,7 -
                                // dilation Height/Width; 8 - same mode;

  sd::ops::avgpool2d pooling;
  sd::Status status = pooling.execute(block.get());
  ASSERT_EQ(sd::Status::OK, status);

  auto result = variableSpace->getVariable(block->getNodeId())->getNDArray();
  ASSERT_TRUE(exp.isSameShape(result));
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests4, avgpool2d_14) {
  const int bS = 2;
  const int iD = 1;
  const int iH = 28;
  const int iW = 28;
  const int kH = 5;
  const int kW = 5;
  const int sH = 1;
  const int sW = 1;
  const int pH = 0;
  const int pW = 0;
  const int dH = 1;
  const int dW = 1;
  const int oH = (iH - kH - (kH - 1) * (dH - 1) + 2 * pH) / sH + 1;  // output height
  const int oW = (iW - kW - (kW - 1) * (dW - 1) + 2 * pW) / sW + 1;  // output width

  auto x = NDArrayFactory::create_<float>('c', {bS, iD, iH, iW});
  auto exp = NDArrayFactory::create<float>('c', {bS, iD, oH, oW});
  // auto z('c',{bS,iD,oH,oW});

  auto variableSpace = new VariableSpace();
  variableSpace->putVariable(-1, x);
  // variableSpace->putVariable(1, &z);

  auto block = new Context(1, variableSpace, false);
  block->fillInputs({-1});
  std::vector<int>* argI = block->getIArguments();
  *argI = {kH, kW, sH, sW, pH, pW,
           dW, dH, 0,  0,  0};  // 0,1 - kernel Height/Width; 2,3 - stride Height/Width; 4,5 - pad Height/Width; 6,7 -
                                // dilation Height/Width; 8 - same mode;

  sd::ops::avgpool2d pooling;
  sd::Status status = pooling.execute(block);
  ASSERT_EQ(sd::Status::OK, status);

  auto result = variableSpace->getVariable(block->getNodeId())->getNDArray();
  // result->printShapeInfo();
  ASSERT_TRUE(exp.isSameShape(result));

  delete variableSpace;
  delete block;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests4, Avgpool2d_test15) {
  const int bS = 2;
  const int iD = 1;
  const int iH = 28;
  const int iW = 28;
  const int kH = 5;
  const int kW = 5;
  const int sH = 1;
  const int sW = 1;
  const int pH = 0;
  const int pW = 0;
  const int dH = 1;
  const int dW = 1;
  const int oH = (int)sd::math::sd_ceil<float, int>(iH * 1.f / sH);
  const int oW = (int)sd::math::sd_ceil<float, int>(iW * 1.f / sW);

  auto x = NDArrayFactory::create_<float>('c', {bS, iD, iH, iW});
  auto exp = NDArrayFactory::create<float>('c', {bS, iD, oH, oW});
  // auto z('c',{bS,iD,oH,oW});

  auto variableSpace = new VariableSpace();
  variableSpace->putVariable(-1, x);
  // variableSpace->putVariable(1, &z);

  auto block = new Context(1, variableSpace, false);
  block->fillInputs({-1});
  std::vector<int>* argI = block->getIArguments();
  *argI = {kH, kW, sH, sW, pH, pW,
           dW, dH, 1,  0,  0};  // 0,1 - kernel Height/Width; 2,3 - stride Height/Width; 4,5 - pad Height/Width; 6,7 -
                                // dilation Height/Width; 8 - same mode;

  sd::ops::avgpool2d pooling;
  sd::Status status = pooling.execute(block);
  ASSERT_EQ(sd::Status::OK, status);

  auto result = variableSpace->getVariable(block->getNodeId())->getNDArray();
  // result->printShapeInfo();
  ASSERT_TRUE(exp.isSameShape(result));

  delete variableSpace;
  delete block;
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests4, avgpool2d_16) {
  int bS = 2, iH = 4, iW = 4, iC = 2, kH = 2, kW = 2, sH = 2, sW = 2, pH = 0, pW = 0, dH = 1, dW = 1;
  int oH = 2, oW = 2;
  int paddingMode = 1;  // 1-SAME,  0-VALID
  int dataFormat = 1;   // 1-NHWC, 0-NDHW

  NDArray input('c', {bS, iH, iW, iC}, sd::DataType::FLOAT32);
  NDArray output('f', {bS, oH, oW, iC}, sd::DataType::FLOAT32);
  NDArray expected('c', {bS, oH, oW, iC},
                   {6.f, 7.f, 10.f, 11.f, 22.f, 23.f, 26.f, 27.f, 38.f, 39.f, 42.f, 43.f, 54.f, 55.f, 58.f, 59.f},
                   sd::DataType::FLOAT32);

  input.linspace(1.);

  sd::ops::avgpool2d op;
  auto status = op.execute({&input}, {&output}, {}, {kH, kW, sH, sW, pH, pW, dH, dW, paddingMode, 0, dataFormat}, {});

  ASSERT_EQ(sd::Status::OK, status);

  // output.printBuffer();
  // expected.printIndexedBuffer("expected");

  ASSERT_TRUE(expected.equalsTo(output));
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests4, biasadd_1) {
  auto x = NDArrayFactory::create<double>('c', {2, 3, 3, 2});
  auto bias = NDArrayFactory::create<double>('c', {2}, {1, 2});
  auto exp = NDArrayFactory::create<double>(
      'c', {2, 3, 3, 2}, {1.f, 2.f, 1.f, 2.f, 1.f, 2.f, 1.f, 2.f, 1.f, 2.f, 1.f, 2.f, 1.f, 2.f, 1.f, 2.f, 1.f, 2.f,
                          1.f, 2.f, 1.f, 2.f, 1.f, 2.f, 1.f, 2.f, 1.f, 2.f, 1.f, 2.f, 1.f, 2.f, 1.f, 2.f, 1.f, 2.f});

  sd::ops::biasadd op;
  auto result = op.evaluate({&x, &bias}, {}, {}, {});

  ASSERT_EQ(sd::Status::OK, result.status());

  auto z = result.at(0);

  ASSERT_TRUE(exp.isSameShape(z));
  ASSERT_TRUE(exp.equalsTo(z));
}

TEST_F(DeclarableOpsTests4, biasadd_2) {
  auto x = NDArrayFactory::create<double>('c', {2, 2, 3, 3});
  auto bias = NDArrayFactory::create<double>('c', {2}, {1, 2});
  auto exp = NDArrayFactory::create<double>('c', {2, 2, 3, 3}, {1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                                                                1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2});

  sd::ops::biasadd op;
  auto result = op.evaluate({&x, &bias}, {}, {}, {true});

  ASSERT_EQ(sd::Status::OK, result.status());

  auto z = result.at(0);

  ASSERT_TRUE(exp.isSameShape(z));
  ASSERT_TRUE(exp.equalsTo(z));
}

TEST_F(DeclarableOpsTests4, biasadd_3) {
  auto x = NDArrayFactory::create<double>('c', {2, 3});
  auto row = NDArrayFactory::create<double>('c', {3}, {1, 2, 3});
  auto exp = NDArrayFactory::create<double>('c', {2, 3}, {1, 2, 3, 1, 2, 3});

  sd::ops::biasadd op;
  auto result = op.evaluate({&x, &row}, {}, {}, {true});

  ASSERT_EQ(sd::Status::OK, result.status());

  auto z = result.at(0);

  ASSERT_TRUE(exp.isSameShape(z));
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests4, biasadd_bp_1) {
  NDArray x('c', {2, 2, 2, 3}, {1., 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24},
            sd::DataType::FLOAT32);
  NDArray gradO('c', {2, 2, 2, 3}, sd::DataType::FLOAT32);
  NDArray bias('c', {3}, {-1., -2, -3}, sd::DataType::FLOAT32);

  NDArray expGradB('c', {3}, {9.2, 10., 10.8}, sd::DataType::FLOAT32);

  gradO.linspace(0.1, 0.1);

  sd::ops::biasadd_bp op;
  auto result = op.evaluate({&x, &bias, &gradO}, {}, {}, {false});  // NHWC

  ASSERT_EQ(sd::Status::OK, result.status());

  auto gradI = result.at(0);
  auto gradB = result.at(1);

  ASSERT_TRUE(gradI->isSameShape(gradO));
  ASSERT_TRUE(gradI->equalsTo(gradO));

  ASSERT_TRUE(gradB->isSameShape(expGradB));
  ASSERT_TRUE(gradB->equalsTo(expGradB));
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests4, biasadd_bp_2) {
  NDArray x('c', {2, 3, 2, 2}, {1., 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24},
            sd::DataType::FLOAT32);
  NDArray gradO('c', {2, 3, 2, 2}, sd::DataType::FLOAT32);
  NDArray bias('c', {3}, {-1., -2, -3}, sd::DataType::FLOAT32);

  NDArray expGradB('c', {3}, {6.8, 10., 13.2}, sd::DataType::FLOAT32);

  gradO.linspace(0.1, 0.1);

  sd::ops::biasadd_bp op;
  auto result = op.evaluate({&x, &bias, &gradO}, {}, {}, {true});  // NCHW

  ASSERT_EQ(sd::Status::OK, result.status());

  auto gradI = result.at(0);
  auto gradB = result.at(1);

  ASSERT_TRUE(gradI->isSameShape(gradO));
  ASSERT_TRUE(gradI->equalsTo(gradO));

  ASSERT_TRUE(gradB->isSameShape(expGradB));
  ASSERT_TRUE(gradB->equalsTo(expGradB));
}

TEST_F(DeclarableOpsTests4, biasadd_4) {
  if (!Environment::getInstance().isExperimentalBuild()) return;

  auto x = NDArrayFactory::create<double>('c', {2, 3});
  auto y = NDArrayFactory::create<float>('c', {3}, {1.f, 2.f, 3.f});
  auto z = NDArrayFactory::create<float>('c', {2, 3});
  auto exp = NDArrayFactory::create<float>('c', {2, 3}, {1.f, 2.f, 3.f, 1.f, 2.f, 3.f});

  sd::ops::biasadd op;
  auto status = op.execute({&x, &y}, {&z}, {}, {}, {true});
  ASSERT_EQ(sd::Status::OK, status);

  ASSERT_EQ(exp, z);
}

TEST_F(DeclarableOpsTests4, Test_Fill_1) {
  auto x = NDArrayFactory::create<int>('c', {1, 3}, {3, 2, 4});
  auto v = NDArrayFactory::create<double>(2.);
  auto exp = NDArrayFactory::create<double>('c', {3, 2, 4});
  exp.assign(2.0f);

  sd::ops::fill op;
  auto result = op.evaluate({&x, &v});

  ASSERT_EQ(sd::Status::OK, result.status());

  auto z = result.at(0);

  ASSERT_TRUE(exp.isSameShape(z));
  ASSERT_TRUE(exp.equalsTo(z));
}

TEST_F(DeclarableOpsTests4, Test_FirasSparce_1) {
  auto x = NDArrayFactory::create<double>('c', {1, 81});
  auto exp = NDArrayFactory::create<double>('c', {1, 2}, {0, 1});

  x.p(51, 1);
  x.p(52, 0);
  x.p(60, 1);
  x.p(61, 0);
  sd::ops::firas_sparse op;
  auto result = op.evaluate({&x}, {0, 1});

  ASSERT_EQ(sd::Status::OK, result.status());

  auto z = result.at(0);
  //    z->printIndexedBuffer("FIRAS");
  //    z->printShapeInfo("OUTSHAPE");
  //    ASSERT_TRUE(exp.isSameShape(z));
  ASSERT_TRUE(exp.equalsTo(z));
}

TEST_F(DeclarableOpsTests4, Test_FlattenTests_1) {
  auto x = NDArrayFactory::create<double>('c', {3, 3, 3, 3});
  auto exp = NDArrayFactory::create<double>('c', {81});

  x.linspace(1);
  exp.linspace(1);
  sd::ops::flatten op;
  auto result = op.evaluate({&x}, {}, {'c'});

  ASSERT_EQ(sd::Status::OK, result.status());

  auto z = result.at(0);
  //    z->printIndexedBuffer("Flatten1");
  //    z->printShapeInfo("Flatten1 shape");
  ASSERT_TRUE(exp.equalsTo(z));
}

TEST_F(DeclarableOpsTests4, Test_FlattenTests_2) {
  auto x = NDArrayFactory::create<double>('c', {3, 3, 3, 3});
  auto y = NDArrayFactory::create<double>('c', {3, 3});
  auto exp = NDArrayFactory::create<double>('c', {90});

  x.linspace(1);
  y.linspace(82);
  exp.linspace(1);
  sd::ops::flatten op;
  auto result = op.evaluate({&x, &y}, {}, {'c'});
  ASSERT_EQ(sd::Status::OK, result.status());

  auto z = result.at(0);
  //    z->printIndexedBuffer("Flatten2");
  //    z->printShapeInfo("Flatten2 shape");

  ASSERT_TRUE(exp.equalsTo(z));
}

TEST_F(DeclarableOpsTests4, Test_FlattenTests_3) {
  NDArray x('c', {2, 2}, {1, 2, 3, 4}, sd::DataType::INT32);
  NDArray y('f', {2, 2}, sd::DataType::INT32);
  NDArray exp('c', {8}, {1, 2, 3, 4, 1, 2, 3, 4}, sd::DataType::INT32);

  y.assign(x);

  sd::ops::flatten op;
  auto result = op.evaluate({&x, &y}, {}, {'c'});
  ASSERT_EQ(sd::Status::OK, result.status());

  auto z = result.at(0);

  ASSERT_TRUE(exp.equalsTo(z));
}

TEST_F(DeclarableOpsTests4, Test_FlattenTests_4) {
  NDArray x('c', {2, 2}, {1, 2, 3, 4}, sd::DataType::INT32);
  NDArray y('f', {2, 2}, sd::DataType::INT32);
  NDArray exp('c', {8}, {1, 3, 2, 4, 1, 3, 2, 4}, sd::DataType::INT32);

  y.assign(x);

  sd::ops::flatten op;
  auto result = op.evaluate({&x, &y}, {}, {'f'});
  ASSERT_EQ(sd::Status::OK, result.status());

  auto z = result.at(0);

  ASSERT_TRUE(exp.equalsTo(z));
}

TEST_F(DeclarableOpsTests4, Test_FloorTests_1) {
  auto x = NDArrayFactory::create<double>('c', {3, 3}, {1.5, 2.3, 3.4, 4.3, 5.9, 6.1, 7.2, 8.9, 9.7});
  auto exp = NDArrayFactory::create<double>('c', {3, 3});

  exp.linspace(1);
  sd::ops::Floor op;
  auto result = op.evaluate({&x});

  ASSERT_EQ(sd::Status::OK, result.status());

  auto z = result.at(0);
  //    z->printIndexedBuffer("Flatten1");
  //    z->printShapeInfo("Flatten1 shape");
  ASSERT_TRUE(exp.equalsTo(z));
}

TEST_F(DeclarableOpsTests4, Test_Split_1) {
  auto x = NDArrayFactory::create<double>('c', {5, 30});
  auto sizes = NDArrayFactory::create<int>('c', {1, 3}, {4, 15, 11});

  std::vector<sd::LongType> list0({0, 0, 0, 4});
  std::vector<sd::LongType> list1({0, 0, 4, 19});
  std::vector<sd::LongType> list2({0, 0, 19, 30});

  auto sub0 = x(list0, true);
  auto sub1 = x(list1, true);
  auto sub2 = x(list2, true);

  sub0.assign(0.0);
  sub1.assign(1.0);
  sub2.assign(2.0);

  sd::ops::split_v op;
  auto result = op.evaluate({&x, &sizes}, {}, {1});

  ASSERT_EQ(sd::Status::OK, result.status());

  ASSERT_EQ(3, result.size());

  auto z0 = result.at(0);
  auto z1 = result.at(1);
  auto z2 = result.at(2);

  ASSERT_TRUE(sub0.isSameShape(z0));
  ASSERT_TRUE(sub1.isSameShape(z1));
  ASSERT_TRUE(sub2.isSameShape(z2));

  ASSERT_TRUE(sub0.equalsTo(z0));
  ASSERT_TRUE(sub1.equalsTo(z1));
  ASSERT_TRUE(sub2.equalsTo(z2));
}

// special test for TF mode, when axis goes first
TEST_F(DeclarableOpsTests4, Test_Split_2) {
  auto x = NDArrayFactory::create<double>('c', {5, 12});
  auto axis = NDArrayFactory::create<double>('c', {1, 1}, {1.f});

  std::vector<sd::LongType> list0 = {0, 0, 0, 3};
  std::vector<sd::LongType> list1 = {0, 0, 3, 6};
  std::vector<sd::LongType> list2 = {0, 0, 6, 9};
  std::vector<sd::LongType> list3 = {0, 0, 9, 12};

  auto sub0 = x(list0, true);
  auto sub1 = x(list1, true);
  auto sub2 = x(list2, true);
  auto sub3 = x(list3, true);

  sub0.assign(0.0f);
  sub1.assign(1.0f);
  sub2.assign(2.0f);
  sub3.assign(3.0f);

  sd::ops::split op;
  auto result = op.evaluate({&axis, &x}, {}, {4});
  ASSERT_EQ(sd::Status::OK, result.status());

  auto z0 = result.at(0);
  auto z1 = result.at(1);
  auto z2 = result.at(2);
  auto z3 = result.at(3);

  ASSERT_TRUE(sub0.isSameShape(z0));
  ASSERT_TRUE(sub1.isSameShape(z1));
  ASSERT_TRUE(sub2.isSameShape(z2));
  ASSERT_TRUE(sub3.isSameShape(z3));

  ASSERT_TRUE(sub0.equalsTo(z0));
  ASSERT_TRUE(sub1.equalsTo(z1));
  ASSERT_TRUE(sub2.equalsTo(z2));
  ASSERT_TRUE(sub3.equalsTo(z3));
}

// special test for TF mode, when axis goes first
TEST_F(DeclarableOpsTests4, Test_Split_3) {
  auto x = NDArrayFactory::create<double>('c', {6, 12});
  auto axis = NDArrayFactory::create<double>('c', {1, 1}, {0.f});

  std::vector<sd::LongType> list0 = {0, 2, 0, 0};
  std::vector<sd::LongType> list1 = {2, 4, 0, 0};
  std::vector<sd::LongType> list2 = {4, 6, 0, 0};

  auto sub0 = x(list0, true);
  auto sub1 = x(list1, true);
  auto sub2 = x(list2, true);

  sub0.assign(0.0f);
  sub1.assign(1.0f);
  sub2.assign(2.0f);

  sd::ops::split op;
  auto result = op.evaluate({&axis, &x}, {}, {3});
  ASSERT_EQ(sd::Status::OK, result.status());

  auto z0 = result.at(0);
  auto z1 = result.at(1);
  auto z2 = result.at(2);

  ASSERT_TRUE(sub0.isSameShape(z0));
  ASSERT_TRUE(sub1.isSameShape(z1));
  ASSERT_TRUE(sub2.isSameShape(z2));

  ASSERT_TRUE(sub0.equalsTo(z0));
  ASSERT_TRUE(sub1.equalsTo(z1));
  ASSERT_TRUE(sub2.equalsTo(z2));
}

///////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests4, split_test4) {
  auto input = NDArrayFactory::create<double>('c', {10}, {1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 10.f});
  auto axis = NDArrayFactory::create<double>(-1);
  auto exp1 = NDArrayFactory::create<double>('c', {5}, {1.f, 2.f, 3.f, 4.f, 5.f});
  auto exp2 = NDArrayFactory::create<double>('c', {5}, {6.f, 7.f, 8.f, 9.f, 10.f});

  sd::ops::split op;
  auto results = op.evaluate({&input, &axis}, {}, {2}, {});

  ASSERT_EQ(sd::Status::OK, results.status());

  auto out1 = results.at(0);
  auto out2 = results.at(1);

  ASSERT_TRUE(exp1.isSameShape(out1));
  ASSERT_TRUE(exp2.isSameShape(out2));
  ASSERT_TRUE(exp1.equalsTo(out1));
  ASSERT_TRUE(exp2.equalsTo(out2));
}

///////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests4, split_test5) {
  auto input = NDArrayFactory::create<double>('c', {3, 8},
                                              {1.f,  2.f,  3.f,  4.f,  5.f,  6.f,  7.f,  8.f,  9.f,  10.f, 11.f, 12.f,
                                               13.f, 14.f, 15.f, 16.f, 17.f, 18.f, 19.f, 20.f, 21.f, 22.f, 23.f, 24.f});
  auto exp1 =
      NDArrayFactory::create<double>('c', {3, 4}, {1.f, 2.f, 3.f, 4.f, 9.f, 10.f, 11.f, 12.f, 17.f, 18.f, 19.f, 20.f});
  auto exp2 =
      NDArrayFactory::create<double>('c', {3, 4}, {5.f, 6.f, 7.f, 8.f, 13.f, 14.f, 15.f, 16.f, 21.f, 22.f, 23.f, 24.f});

  sd::ops::split op;
  auto results = op.evaluate({&input}, {}, {2, -1}, {});

  ASSERT_EQ(sd::Status::OK, results.status());

  auto out1 = results.at(0);
  auto out2 = results.at(1);

  ASSERT_TRUE(exp1.isSameShape(out1));
  ASSERT_TRUE(exp2.isSameShape(out2));
  ASSERT_TRUE(exp1.equalsTo(out1));
  ASSERT_TRUE(exp2.equalsTo(out2));
}

///////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests4, split_test6) {
  NDArray input('c', {0, 4}, sd::DataType::FLOAT32);
  std::vector<sd::LongType> expShape = {0, 1};

  const int numSplits = 4;
  const int axis = 1;

  sd::ops::split op;
  auto results = op.evaluate({&input}, {}, {numSplits, axis}, {});

  ASSERT_EQ(sd::Status::OK, results.status());

  for (int i = 0; i < numSplits; ++i) ASSERT_TRUE(results.at(i)->isSameShape(expShape));
}

///////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests4, split_test7) {
  NDArray input('c', {0, 4}, sd::DataType::FLOAT32);
  std::vector<sd::LongType> expShape = {0, 4};

  const int numSplits = 4;
  const int axis = 0;

  sd::ops::split op;
  auto results = op.evaluate({&input}, {}, {numSplits, axis}, {});

  ASSERT_EQ(sd::Status::OK, results.status());

  for (int i = 0; i < numSplits; ++i) ASSERT_TRUE(results.at(i)->isSameShape(expShape));
}

TEST_F(DeclarableOpsTests4, Test_Squeeze_args_1) {
  auto x = NDArrayFactory::create<double>('c', {2, 1, 1, 1, 2}, {1, 2, 3, 4});
  auto exp = NDArrayFactory::create<double>('c', {2, 1, 2}, {1, 2, 3, 4});

  sd::ops::squeeze op;
  auto result = op.evaluate({&x}, {}, {1, 3});
  ASSERT_EQ(sd::Status::OK, result.status());

  auto z = result.at(0);

  ASSERT_TRUE(exp.isSameShape(z));
  ASSERT_TRUE(exp.equalsTo(z));
}

TEST_F(DeclarableOpsTests4, Test_Squeeze_args_2) {
  auto x = NDArrayFactory::create<double>('c', {2, 1, 1, 1, 2}, {1, 2, 3, 4});
  auto y = NDArrayFactory::create<double>('c', {2}, {1.f, 3.f});
  auto exp = NDArrayFactory::create<double>('c', {2, 1, 2}, {1, 2, 3, 4});

  sd::ops::squeeze op;
  auto result = op.evaluate({&x, &y}, {}, {});
  ASSERT_EQ(sd::Status::OK, result.status());

  auto z = result.at(0);

  ASSERT_TRUE(exp.isSameShape(z));
  ASSERT_TRUE(exp.equalsTo(z));
}

TEST_F(DeclarableOpsTests4, Test_Squeeze_args_3) {
  auto x = NDArrayFactory::create<double>('c', {2, 1, 1, 1, 2}, {1, 2, 3, 4});
  auto exp = NDArrayFactory::create<double>('c', {2, 1, 2}, {1, 2, 3, 4});

  sd::ops::squeeze op;
  auto result = op.evaluate({&x}, {}, {-2, -3});
  ASSERT_EQ(sd::Status::OK, result.status());

  auto z = result.at(0);

  ASSERT_TRUE(exp.isSameShape(z));
  ASSERT_TRUE(exp.equalsTo(z));
}

TEST_F(DeclarableOpsTests4, Test_SpaceToDepth_1) {
  auto x = NDArrayFactory::create<double>('c', {1, 2, 2, 3}, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12});
  auto exp = NDArrayFactory::create<double>('c', {1, 1, 1, 12}, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12});

  sd::ops::space_to_depth op;
  auto result = op.evaluate({&x}, {}, {2, 1});
  ASSERT_EQ(sd::Status::OK, result.status());

  auto z = result.at(0);

  ASSERT_TRUE(exp.isSameShape(z));
  ASSERT_TRUE(exp.equalsTo(z));
}

TEST_F(DeclarableOpsTests4, Test_SpaceToDepth_2) {
  auto x = NDArrayFactory::create<double>('c', {1, 3, 2, 2}, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12});
  auto exp = NDArrayFactory::create<double>('c', {1, 12, 1, 1}, {1, 5, 9, 2, 6, 10, 3, 7, 11, 4, 8, 12});

  sd::ops::space_to_depth op;
  auto result = op.evaluate({&x}, {}, {2, 0});
  ASSERT_EQ(sd::Status::OK, result.status());

  auto z = result.at(0);

  ASSERT_TRUE(exp.isSameShape(z));
  ASSERT_TRUE(exp.equalsTo(z));
}

TEST_F(DeclarableOpsTests4, Test_DepthToSpace_1) {
  auto x = NDArrayFactory::create<double>('c', {1, 1, 1, 12}, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12});
  auto exp = NDArrayFactory::create<double>('c', {1, 2, 2, 3}, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12});

  sd::ops::depth_to_space op;
  auto result = op.evaluate({&x}, {}, {2, 1});
  ASSERT_EQ(sd::Status::OK, result.status());

  auto z = result.at(0);

  ASSERT_TRUE(exp.isSameShape(z));
  ASSERT_TRUE(exp.equalsTo(z));
}

TEST_F(DeclarableOpsTests4, Test_DepthToSpace_2) {
  auto x = NDArrayFactory::create<double>('c', {1, 12, 1, 1}, {1, 5, 9, 2, 6, 10, 3, 7, 11, 4, 8, 12});
  auto exp = NDArrayFactory::create<double>('c', {1, 3, 2, 2}, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12});

  sd::ops::depth_to_space op;
  auto result = op.evaluate({&x}, {}, {2, 0});
  ASSERT_EQ(sd::Status::OK, result.status());

  auto z = result.at(0);

  ASSERT_TRUE(exp.isSameShape(z));
  ASSERT_TRUE(exp.equalsTo(z));
}

TEST_F(DeclarableOpsTests4, Test_DepthToSpace_3) {
  auto x = NDArrayFactory::create<double>('c', {4, 4, 16, 16});
  auto exp = NDArrayFactory::create<double>('c', {4, 16, 64, 1});

  sd::ops::depth_to_space op;
  auto result = op.evaluate({&x}, {}, {4, 1});
  ASSERT_EQ(sd::Status::OK, result.status());

  auto z = result.at(0);

  ASSERT_TRUE(exp.isSameShape(z));
}

TEST_F(DeclarableOpsTests4, Test_Cross_1) {
  auto a = NDArrayFactory::create<double>('c', {3}, {1, 2, 3});
  auto b = NDArrayFactory::create<double>('c', {3}, {6, 7, 8});
  auto exp = NDArrayFactory::create<double>('c', {3}, {-5, 10, -5});

  sd::ops::cross op;
  auto result = op.evaluate({&a, &b});
  ASSERT_EQ(sd::Status::OK, result.status());

  auto z = result.at(0);

  ASSERT_TRUE(exp.isSameShape(z));
  ASSERT_TRUE(exp.equalsTo(z));
}

TEST_F(DeclarableOpsTests4, Test_Cross_2) {
  auto a = NDArrayFactory::create<double>('c', {2, 3}, {1, 2, 3, 1, 2, 3});
  auto b = NDArrayFactory::create<double>('c', {2, 3}, {6, 7, 8, 6, 7, 8});
  auto exp = NDArrayFactory::create<double>('c', {2, 3}, {-5, 10, -5, -5, 10, -5});

  sd::ops::cross op;
  auto result = op.evaluate({&a, &b});
  ASSERT_EQ(sd::Status::OK, result.status());

  auto z = result.at(0);

  ASSERT_TRUE(exp.isSameShape(z));
  ASSERT_TRUE(exp.equalsTo(z));
}

TEST_F(DeclarableOpsTests4, Test_Cross_3) {
  auto a = NDArrayFactory::create<double>('c', {3, 3}, {1, 2, 3, 4, 5, 6, 7, 8, 9});
  auto b = NDArrayFactory::create<double>('c', {3, 3}, {2, 3, 4, 7, 6, 5, 6, 3, 2});
  auto exp = NDArrayFactory::create<double>('c', {3, 3}, {-1, 2, -1, -11, 22, -11, -11, 40, -27});

  sd::ops::cross op;
  auto result = op.evaluate({&a, &b});
  ASSERT_EQ(sd::Status::OK, result.status());

  auto z = result.at(0);

  ASSERT_TRUE(exp.isSameShape(z));
  ASSERT_TRUE(exp.equalsTo(z));
}

TEST_F(DeclarableOpsTests4, Test_Add_119) {
  auto a = NDArrayFactory::create<double>('c', {1, 4}, {1, 2, 3, 4});
  auto b = NDArrayFactory::create<double>('c', {4}, {1, 2, 3, 4});
  auto exp = NDArrayFactory::create<double>('c', {1, 4}, {2, 4, 6, 8});

  sd::ops::add op;
  auto result = op.evaluate({&a, &b});

  ASSERT_EQ(sd::Status::OK, result.status());

  auto z = result.at(0);

  ASSERT_EQ(2, z->rankOf());

  ASSERT_TRUE(exp.isSameShape(z));
  ASSERT_TRUE(exp.equalsTo(z));
}

TEST_F(DeclarableOpsTests4, Test_TileToShape_1) {
  auto x = NDArrayFactory::create<double>('c', {2, 1, 3});
  auto exp =
      NDArrayFactory::create<double>('c', {2, 4, 3}, {1.f, 2.f, 3.f, 1.f, 2.f, 3.f, 1.f, 2.f, 3.f, 1.f, 2.f, 3.f,
                                                      4.f, 5.f, 6.f, 4.f, 5.f, 6.f, 4.f, 5.f, 6.f, 4.f, 5.f, 6.f});
  x.linspace(1.f);

  sd::ops::tile_to_shape op;
  auto result = op.evaluate({&x}, {}, {2, 4, 3});

  ASSERT_EQ(sd::Status::OK, result.status());

  auto z = result.at(0);

  ASSERT_TRUE(exp.isSameShape(z));
  ASSERT_TRUE(exp.equalsTo(z));
}

TEST_F(DeclarableOpsTests4, Test_StridedSlice_Alex_1) {
  auto x = NDArrayFactory::create<double>('c', {3, 4, 5});
  x.linspace(1);
  auto exp = NDArrayFactory::create<double>('c', {1, 3, 4, 5});
  exp.linspace(1);

  sd::ops::strided_slice op;
  auto result = op.evaluate({&x}, {}, {0, 0, 0, 1, 0, -999, 0, 0, 0, -999, 3, 4, 5, -999, 1, 1, 1});

  ASSERT_EQ(sd::Status::OK, result.status());

  auto z = result.at(0);

  ASSERT_TRUE(exp.isSameShape(z));
  ASSERT_TRUE(exp.equalsTo(z));
}

TEST_F(DeclarableOpsTests4, Test_StridedSlice_Alex_2) {
  auto x = NDArrayFactory::create<double>('c', {3, 4, 5});
  auto begin = NDArrayFactory::create<double>('c', {4}, {-999, 0, 0, 0});
  auto end = NDArrayFactory::create<double>('c', {4}, {-999, 3, 4, 5});
  auto stride = NDArrayFactory::create<double>('c', {4}, {-999, 1, 1, 1});
  x.linspace(1);
  auto exp = NDArrayFactory::create<double>('c', {1, 3, 4, 5});
  exp.linspace(1);

  sd::ops::strided_slice op;
  auto result = op.evaluate({&x, &begin, &end, &stride}, {}, {0, 0, 0, 1, 0});

  ASSERT_EQ(sd::Status::OK, result.status());

  ASSERT_EQ(sd::Status::OK, result.status());

  auto z = result.at(0);

  ASSERT_TRUE(exp.isSameShape(z));
  ASSERT_TRUE(exp.equalsTo(z));
}

TEST_F(DeclarableOpsTests4, Test_StridedSlice_Alex_3) {
  int axis = 0;
  auto x = NDArrayFactory::create<double>('c', {1}, {10});
  auto begin = NDArrayFactory::create<int>('c', {1}, {axis});
  auto end = NDArrayFactory::create<int>('c', {1}, {axis});
  auto stride = NDArrayFactory::create<int>('c', {1}, {1});
  // x.linspace(1);
  // auto exp = NDArrayFactory::create<double>('c', {1,3,4,5});
  // exp.linspace(1);

  sd::ops::strided_slice op;
  auto result = op.evaluate({&x, &begin, &end, &stride}, {}, {1, 0, 0, 0, 0});

  ASSERT_EQ(sd::Status::OK, result.status());

  auto z = result.at(0);
  ASSERT_TRUE(z->isEmpty());
}
TEST_F(DeclarableOpsTests4, Test_StridedSlice_Alex_4) {
  auto x = NDArrayFactory::create<double>('c', {1, 3}, {1, 2, 3});
  auto begin = NDArrayFactory::create<double>('c', {2}, {0, 0});
  auto end = NDArrayFactory::create<double>('c', {2}, {0, 1});
  auto stride = NDArrayFactory::create<double>('c', {2}, {1, 1});
  //    x.linspace(1);
  auto exp = NDArrayFactory::create<double>('c', {1}, {1});
  // exp.linspace(1);

  sd::ops::strided_slice op;
  auto result = op.evaluate({&x, &begin, &end, &stride}, {}, {1, 0, 1, 0, 2});

  ASSERT_EQ(sd::Status::OK, result.status());

  auto z = result.at(0);
  ASSERT_TRUE(z->lengthOf() == 1);
  ASSERT_TRUE(exp.equalsTo(z));
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests4, parallel_stack_test1) {
  auto x1 = NDArrayFactory::create<double>('c', {2, 2, 2});
  auto x2 = NDArrayFactory::create<double>('c', {2, 2, 2});
  auto x3 = NDArrayFactory::create<double>('c', {2, 2, 2});
  x1.linspace(1);
  x2.linspace(9);
  x3.linspace(17);

  auto expected = NDArrayFactory::create<double>('c', {3, 2, 2, 2});
  expected.linspace(1);

  sd::ops::parallel_stack op;
  auto results = op.evaluate({&x1, &x2, &x3});
  auto output = results.at(0);

  ASSERT_EQ(sd::Status::OK, results.status());
  ASSERT_TRUE(expected.isSameShape(output));
  ASSERT_TRUE(expected.equalsTo(output));
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests4, parallel_stack_test2) {
  auto x1 = NDArrayFactory::create<double>('c', {1, 2}, {1, 2});
  auto x2 = NDArrayFactory::create<double>('c', {1, 2}, {3, 4});
  auto x3 = NDArrayFactory::create<double>('c', {1, 2}, {5, 6});

  auto expected = NDArrayFactory::create<double>('c', {3, 1, 2}, {1, 2, 3, 4, 5, 6});

  sd::ops::parallel_stack op;
  auto results = op.evaluate({&x1, &x2, &x3});
  auto output = results.at(0);

  ASSERT_EQ(sd::Status::OK, results.status());
  ASSERT_TRUE(expected.isSameShape(output));
  ASSERT_TRUE(expected.equalsTo(output));
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests4, parallel_stack_test3) {
  auto x1 = NDArrayFactory::create<double>('c', {2, 1}, {1, 2});
  auto x2 = NDArrayFactory::create<double>('c', {2, 1}, {3, 4});
  auto x3 = NDArrayFactory::create<double>('c', {2, 1}, {5, 6});

  auto expected = NDArrayFactory::create<double>('c', {3, 2, 1}, {1, 2, 3, 4, 5, 6});

  sd::ops::parallel_stack op;
  auto results = op.evaluate({&x1, &x2, &x3});
  auto output = results.at(0);

  ASSERT_EQ(sd::Status::OK, results.status());
  ASSERT_TRUE(expected.isSameShape(output));
  ASSERT_TRUE(expected.equalsTo(output));
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests4, parallel_stack_test4) {
  auto x1 = NDArrayFactory::create<double>('c', {2}, {1, 2});
  auto x2 = NDArrayFactory::create<double>('c', {2}, {3, 4});
  auto x3 = NDArrayFactory::create<double>('c', {2}, {5, 6});

  auto expected = NDArrayFactory::create<double>('c', {3, 2}, {1, 2, 3, 4, 5, 6});

  sd::ops::parallel_stack op;
  auto results = op.evaluate({&x1, &x2, &x3});
  auto output = results.at(0);

  ASSERT_EQ(sd::Status::OK, results.status());
  ASSERT_TRUE(expected.isSameShape(output));
  ASSERT_TRUE(expected.equalsTo(output));
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests4, parallel_stack_test5) {
  auto x1 = NDArrayFactory::create<double>('c', {1}, {1});
  auto x2 = NDArrayFactory::create<double>('c', {1}, {3});
  auto x3 = NDArrayFactory::create<double>('c', {1}, {5});

  auto expected = NDArrayFactory::create<double>('c', {3, 1}, {1, 3, 5});

  sd::ops::parallel_stack op;
  auto results = op.evaluate({&x1, &x2, &x3});
  auto output = results.at(0);

  ASSERT_EQ(sd::Status::OK, results.status());
  ASSERT_TRUE(expected.isSameShape(output));
  ASSERT_TRUE(expected.equalsTo(output));
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests4, parallel_stack_test6) {
  auto x1 = NDArrayFactory::create<double>(1.);
  auto x2 = NDArrayFactory::create<double>(3.);
  auto x3 = NDArrayFactory::create<double>(5.);

  auto expected = NDArrayFactory::create<double>('c', {3}, {1, 3, 5});

  sd::ops::parallel_stack op;
  auto results = op.evaluate({&x1, &x2, &x3});
  auto output = results.at(0);

  ASSERT_EQ(sd::Status::OK, results.status());
  ASSERT_TRUE(expected.isSameShape(output));
  ASSERT_TRUE(expected.equalsTo(output));
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests4, parallel_stack_test7) {
  auto x1 = NDArrayFactory::create<double>(1.);
  auto expected = NDArrayFactory::create<double>('c', {1}, {1.});

  sd::ops::parallel_stack op;
  auto results = op.evaluate({&x1});
  auto output = results.at(0);

  ASSERT_EQ(sd::Status::OK, results.status());
  ASSERT_TRUE(expected.isSameShape(output));
  ASSERT_TRUE(expected.equalsTo(output));
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests4, meshgrid_test1) {
  auto in0 = NDArrayFactory::create<double>('c', {2}, {1, 2});
  auto in1 = NDArrayFactory::create<double>('c', {3}, {10, 20, 30});
  auto in2 = NDArrayFactory::create<double>('c', {4}, {100, 200, 300, 400});
  auto exp0 = NDArrayFactory::create<double>('c', {2, 3, 4},
                                             {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2});
  auto exp1 = NDArrayFactory::create<double>(
      'c', {2, 3, 4}, {10, 10, 10, 10, 20, 20, 20, 20, 30, 30, 30, 30, 10, 10, 10, 10, 20, 20, 20, 20, 30, 30, 30, 30});
  auto exp2 =
      NDArrayFactory::create<double>('c', {2, 3, 4}, {100, 200, 300, 400, 100, 200, 300, 400, 100, 200, 300, 400,
                                                      100, 200, 300, 400, 100, 200, 300, 400, 100, 200, 300, 400});

  sd::ops::meshgrid op;
  auto results = op.evaluate({&in0, &in1, &in2}, {}, {0});
  auto out0 = results.at(0);
  auto out1 = results.at(1);
  auto out2 = results.at(2);

  // out0->printIndexedBuffer();

  ASSERT_EQ(sd::Status::OK, results.status());
  ASSERT_TRUE(exp0.isSameShape(out0));
  ASSERT_TRUE(exp0.equalsTo(out0));
  ASSERT_TRUE(exp1.isSameShape(out1));
  ASSERT_TRUE(exp1.equalsTo(out1));
  ASSERT_TRUE(exp2.isSameShape(out2));
  ASSERT_TRUE(exp2.equalsTo(out2));
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests4, meshgrid_test2) {
  auto in0 = NDArrayFactory::create<double>('c', {2}, {1, 2});
  auto in1 = NDArrayFactory::create<double>('c', {3}, {10, 20, 30});
  auto in2 = NDArrayFactory::create<double>('c', {4}, {100, 200, 300, 400});
  auto exp0 = NDArrayFactory::create<double>('c', {3, 2, 4},
                                             {1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2});
  auto exp1 = NDArrayFactory::create<double>(
      'c', {3, 2, 4}, {10, 10, 10, 10, 10, 10, 10, 10, 20, 20, 20, 20, 20, 20, 20, 20, 30, 30, 30, 30, 30, 30, 30, 30});
  auto exp2 =
      NDArrayFactory::create<double>('c', {3, 2, 4}, {100, 200, 300, 400, 100, 200, 300, 400, 100, 200, 300, 400,
                                                      100, 200, 300, 400, 100, 200, 300, 400, 100, 200, 300, 400});

  sd::ops::meshgrid op;
  auto results = op.evaluate({&in0, &in1, &in2});
  auto out0 = results.at(0);
  auto out1 = results.at(1);
  auto out2 = results.at(2);

  ASSERT_EQ(sd::Status::OK, results.status());
  ASSERT_TRUE(exp0.isSameShape(out0));
  ASSERT_TRUE(exp0.equalsTo(out0));
  ASSERT_TRUE(exp1.isSameShape(out1));
  ASSERT_TRUE(exp1.equalsTo(out1));
  ASSERT_TRUE(exp2.isSameShape(out2));
  ASSERT_TRUE(exp2.equalsTo(out2));
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests4, meshgrid_test3) {
  auto in0 = NDArrayFactory::create<double>('c', {2}, {1, 2});
  auto in1 = NDArrayFactory::create<double>('c', {1, 3}, {10, 20, 30});
  auto in2 = NDArrayFactory::create<double>('c', {2, 2}, {100, 200, 300, 400});
  auto exp0 = NDArrayFactory::create<double>('c', {3, 2, 4},
                                             {1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2});
  auto exp1 = NDArrayFactory::create<double>(
      'c', {3, 2, 4}, {10, 10, 10, 10, 10, 10, 10, 10, 20, 20, 20, 20, 20, 20, 20, 20, 30, 30, 30, 30, 30, 30, 30, 30});
  auto exp2 =
      NDArrayFactory::create<double>('c', {3, 2, 4}, {100, 200, 300, 400, 100, 200, 300, 400, 100, 200, 300, 400,
                                                      100, 200, 300, 400, 100, 200, 300, 400, 100, 200, 300, 400});

  sd::ops::meshgrid op;
  auto results = op.evaluate({&in0, &in1, &in2});
  auto out0 = results.at(0);
  auto out1 = results.at(1);
  auto out2 = results.at(2);

  ASSERT_EQ(sd::Status::OK, results.status());
  ASSERT_TRUE(exp0.isSameShape(out0));
  ASSERT_TRUE(exp0.equalsTo(out0));
  ASSERT_TRUE(exp1.isSameShape(out1));
  ASSERT_TRUE(exp1.equalsTo(out1));
  ASSERT_TRUE(exp2.isSameShape(out2));
  ASSERT_TRUE(exp2.equalsTo(out2));
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests4, meshgrid_test4) {
  auto in0 = NDArrayFactory::create<double>('c', {1, 2}, {1, 2});
  auto in1 = NDArrayFactory::create<double>('c', {3, 1}, {10, 20, 30});
  auto in2 = NDArrayFactory::create<double>('c', {1, 4, 1}, {100, 200, 300, 400});
  auto exp0 = NDArrayFactory::create<double>('c', {2, 3, 4},
                                             {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2});
  auto exp1 = NDArrayFactory::create<double>(
      'c', {2, 3, 4}, {10, 10, 10, 10, 20, 20, 20, 20, 30, 30, 30, 30, 10, 10, 10, 10, 20, 20, 20, 20, 30, 30, 30, 30});
  auto exp2 =
      NDArrayFactory::create<double>('c', {2, 3, 4}, {100, 200, 300, 400, 100, 200, 300, 400, 100, 200, 300, 400,
                                                      100, 200, 300, 400, 100, 200, 300, 400, 100, 200, 300, 400});

  sd::ops::meshgrid op;
  auto results = op.evaluate({&in0, &in1, &in2}, {}, {0});
  auto out0 = results.at(0);
  auto out1 = results.at(1);
  auto out2 = results.at(2);

  ASSERT_EQ(sd::Status::OK, results.status());
  ASSERT_TRUE(exp0.isSameShape(out0));
  ASSERT_TRUE(exp0.equalsTo(out0));
  ASSERT_TRUE(exp1.isSameShape(out1));
  ASSERT_TRUE(exp1.equalsTo(out1));
  ASSERT_TRUE(exp2.isSameShape(out2));
  ASSERT_TRUE(exp2.equalsTo(out2));
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests4, meshgrid_test5) {
  auto in0 = NDArrayFactory::create<double>(1);
  auto in1 = NDArrayFactory::create<double>(2);
  auto in2 = NDArrayFactory::create<double>(3);
  auto exp0 = NDArrayFactory::create<double>('c', {1, 1, 1}, {1});
  auto exp1 = NDArrayFactory::create<double>('c', {1, 1, 1}, {2});
  auto exp2 = NDArrayFactory::create<double>('c', {1, 1, 1}, {3});

  sd::ops::meshgrid op;
  auto results = op.evaluate({&in0, &in1, &in2}, {}, {0});
  auto out0 = results.at(0);
  auto out1 = results.at(1);
  auto out2 = results.at(2);

  ASSERT_EQ(sd::Status::OK, results.status());
  ASSERT_TRUE(exp0.isSameShape(out0));
  ASSERT_TRUE(exp0.equalsTo(out0));
  ASSERT_TRUE(exp1.isSameShape(out1));
  ASSERT_TRUE(exp1.equalsTo(out1));
  ASSERT_TRUE(exp2.isSameShape(out2));
  ASSERT_TRUE(exp2.equalsTo(out2));
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests4, meshgrid_test6) {
  auto in0 = NDArrayFactory::create<double>('c', {2, 2}, {1, 2, 3, 4});
  auto in1 = NDArrayFactory::create<double>(5);
  auto in2 = NDArrayFactory::create<double>(6);
  auto exp0 = NDArrayFactory::create<double>('c', {4, 1, 1}, {1, 2, 3, 4});
  auto exp1 = NDArrayFactory::create<double>('c', {4, 1, 1}, {5, 5, 5, 5});
  auto exp2 = NDArrayFactory::create<double>('c', {4, 1, 1}, {6, 6, 6, 6});

  sd::ops::meshgrid op;
  auto results = op.evaluate({&in0, &in1, &in2}, {}, {0});
  auto out0 = results.at(0);
  auto out1 = results.at(1);
  auto out2 = results.at(2);

  ASSERT_EQ(sd::Status::OK, results.status());
  ASSERT_TRUE(exp0.isSameShape(out0));
  ASSERT_TRUE(exp0.equalsTo(out0));
  ASSERT_TRUE(exp1.isSameShape(out1));
  ASSERT_TRUE(exp1.equalsTo(out1));
  ASSERT_TRUE(exp2.isSameShape(out2));
  ASSERT_TRUE(exp2.equalsTo(out2));
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests4, meshgrid_test7) {
  auto in0 = NDArrayFactory::create<double>('c', {2, 2}, {1, 2, 3, 4});
  auto in1 = NDArrayFactory::create<double>(5);
  auto in2 = NDArrayFactory::create<double>(6);
  auto exp0 = NDArrayFactory::create<double>('c', {1, 4, 1}, {1, 2, 3, 4});
  auto exp1 = NDArrayFactory::create<double>('c', {1, 4, 1}, {5, 5, 5, 5});
  auto exp2 = NDArrayFactory::create<double>('c', {1, 4, 1}, {6, 6, 6, 6});

  sd::ops::meshgrid op;
  auto results = op.evaluate({&in0, &in1, &in2}, {}, {1});
  auto out0 = results.at(0);
  auto out1 = results.at(1);
  auto out2 = results.at(2);

  ASSERT_EQ(sd::Status::OK, results.status());
  ASSERT_TRUE(exp0.isSameShape(out0));
  ASSERT_TRUE(exp0.equalsTo(out0));
  ASSERT_TRUE(exp1.isSameShape(out1));
  ASSERT_TRUE(exp1.equalsTo(out1));
  ASSERT_TRUE(exp2.isSameShape(out2));
  ASSERT_TRUE(exp2.equalsTo(out2));
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests4, meshgrid_test8) {
  auto in0 = NDArrayFactory::create<double>(5);
  auto exp0 = NDArrayFactory::create<double>('c', {1}, {5});

  sd::ops::meshgrid op;
  auto results = op.evaluate({&in0}, {}, {0});
  auto out0 = results.at(0);

  ASSERT_EQ(sd::Status::OK, results.status());
  ASSERT_TRUE(exp0.isSameShape(out0));
  ASSERT_TRUE(exp0.equalsTo(out0));
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests4, meshgrid_test9) {
  auto in0 = NDArrayFactory::create<double>(5);
  auto exp0 = NDArrayFactory::create<double>('c', {1}, {5});

  sd::ops::meshgrid op;
  auto results = op.evaluate({&in0}, {}, {1});
  auto out0 = results.at(0);

  ASSERT_EQ(sd::Status::OK, results.status());
  ASSERT_TRUE(exp0.isSameShape(out0));
  ASSERT_TRUE(exp0.equalsTo(out0));
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests4, WeightedCrossEntropyWithLogits_1) {
  auto input = NDArrayFactory::create<double>('c', {2, 3}, {11.f, 13.f, 4.f, 15.f, 6.f, 3.f});
  auto targets = NDArrayFactory::create<double>('c', {2, 3}, {15.5f, 15.7f, 5.f, 15.f, 5.f, 6.f});
  auto weight = NDArrayFactory::create<double>(0.7f);
  auto expected =
      NDArrayFactory::create<double>('c', {2, 3}, {-159.50006, -191.1, -16.009075, -210., -24.001238, -15.03887});

  // Targets {15.5f, 15.7f,  5.f , 15.f,   5.f,   6.f};
  //----------
  // Inputs {11.f, 13.f,  4.f, 15.f,  6.f,  3.f};
  //----------
  // Weights [0.7]
  // Result {-159.50006,  -191.1,       -16.009075, -210., -24.001238,  -15.03887}

  sd::ops::weighted_cross_entropy_with_logits op;
  auto results = op.evaluate({&targets, &input, &weight});
  auto output = results.at(0);

  // output->printIndexedBuffer();

  ASSERT_EQ(sd::Status::OK, results.status());
  ASSERT_TRUE(expected.isSameShape(output));
  ASSERT_TRUE(expected.equalsTo(output));
}

////////////////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests4, WeightedCrossEntropyWithLogits_2) {
  auto input = NDArrayFactory::create<double>('c', {2, 3}, {11.f, 13.f, 4.f, 15.f, 6.f, 3.f});
  auto targets = NDArrayFactory::create<double>('c', {2, 3}, {15.5f, 15.7f, 5.f, 15.f, 5.f, 6.f});
  auto weights = NDArrayFactory::create<double>({0.5f, 0.7f, 1.0f});
  auto expected =
      NDArrayFactory::create<double>('c', {2, 3}, {-159.5001f, -191.1f, -15.98185f, -210.f, -24.001238f, -14.951412f});

  sd::ops::weighted_cross_entropy_with_logits op;
  auto results = op.evaluate({&targets, &input, &weights});
  auto output = results.at(0);

  ASSERT_EQ(sd::Status::OK, results.status());
  ASSERT_TRUE(expected.isSameShape(output));
  ASSERT_TRUE(expected.equalsTo(output));
}

///////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests4, lstm_test1) {
  const int time = 5;
  const int batchSize = 3;
  const int inSize = 3;
  const int numProj = 3;
  const int numUnits = 3;

  auto x = NDArrayFactory::create<double>('c', {time, batchSize, inSize});
  auto h0 = NDArrayFactory::create<double>('c', {batchSize, numProj});
  auto c0 = NDArrayFactory::create<double>('c', {batchSize, numUnits});
  auto Wx = NDArrayFactory::create<double>('c', {inSize, 4 * numUnits});
  auto Wh = NDArrayFactory::create<double>('c', {numProj, 4 * numUnits});
  auto Wc = NDArrayFactory::create<double>('c', {3 * numUnits});
  auto Wp = NDArrayFactory::create<double>('c', {numUnits, numProj});
  auto b = NDArrayFactory::create<double>('c', {4 * numUnits});

  x.linspace(0.5, 0.5);
  h0 = 1.;
  c0 = 2.;
  Wx = 0.003;
  Wh = 0.006;
  Wc = 0.;
  Wp = 0.;
  b = 0.5;

  auto expH = NDArrayFactory::create<double>(
      'c', {time, batchSize, numProj},
      {0.57574, 0.57574, 0.57574, 0.58006, 0.58006, 0.58006, 0.58434, 0.58434, 0.58434, 0.55114, 0.55114, 0.55114,
       0.55732, 0.55732, 0.55732, 0.56338, 0.56338, 0.56338, 0.53763, 0.53763, 0.53763, 0.54534, 0.54534, 0.54534,
       0.55287, 0.55287, 0.55287, 0.53626, 0.53626, 0.53626, 0.54487, 0.54487, 0.54487, 0.55327, 0.55327, 0.55327,
       0.54484, 0.54484, 0.54484, 0.55379, 0.55379, 0.55379, 0.5625,  0.5625,  0.5625});

  auto expClast = NDArrayFactory::create<double>(
      'c', {1, batchSize, numProj},
      {1.1589154, 1.1589154, 1.1589154, 1.1892855, 1.1892855, 1.1892855, 1.219861, 1.219861, 1.219861});

  sd::ops::lstm op;
  auto results = op.evaluate({&x, &h0, &c0, &Wx, &Wh, &Wc, &Wp, &b}, {0., 0., 0.}, {0, 0});

  ASSERT_EQ(sd::Status::OK, results.status());

  auto* h = results.at(0);
  auto* c = results.at(1);
  auto cLast = (*c)({4, 5, 0, 0, 0, 0}, true);

  ASSERT_TRUE(expH.isSameShape(h));
  ASSERT_TRUE(expH.equalsTo(h));

  ASSERT_TRUE(expClast.isSameShape(&cLast));
  ASSERT_TRUE(expClast.equalsTo(&cLast));
}

///////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests4, relu6_test1) {
  auto input = NDArrayFactory::create<double>('c', {2, 4}, {-13., 10, -5, 0, 2, 7, 6, 12});
  auto expected = NDArrayFactory::create<double>('c', {2, 4}, {0., 6., 0., 0., 2., 6., 6., 6.});

  sd::ops::relu6 op;
  auto results = op.evaluate({&input}, {0.}, {});

  ASSERT_EQ(sd::Status::OK, results.status());

  auto output = results.at(0);

  ASSERT_TRUE(expected.isSameShape(output));
  ASSERT_TRUE(expected.equalsTo(output));
}

///////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests4, relu6_bp_test1) {
  auto input = NDArrayFactory::create<double>('c', {2, 4}, {-13., 10, -5, 0, 2, 7, 6, 5});
  auto gradO = NDArrayFactory::create<double>('c', {2, 4}, {-1., -2., 0., 4., 5., 6., 7., 8.});

  auto expected = NDArrayFactory::create<double>('c', {2, 4}, {0., 0., 0., 0., 5., 0., 0., 8.});

  sd::ops::relu6_bp op;
  auto results = op.evaluate({&input, &gradO}, {0.});

  ASSERT_EQ(sd::Status::OK, results.status());

  auto output = results.at(0);

  ASSERT_TRUE(expected.isSameShape(output));
  ASSERT_TRUE(expected.equalsTo(output));
}

////////////////////////////////////////////////////////////////////////////////
TYPED_TEST(TypedDeclarableOpsTests4, LrnTest_1) {
  auto x = NDArrayFactory::create<TypeParam>(
      'c', {2, 2, 2, 2}, {5.5f, 0.f, 0.3f, 5.5f, 8.6f, 0.f, 0.f, 0.4f, 1.5f, 1.f, 1.3f, 1.5f, 2.6f, 2.f, 3.f, 1.4f});

  auto exp = NDArrayFactory::create<TypeParam>(
      'c', {2, 2, 2, 2},
      {0.98386997f, 0.f, 0.05358852f, 0.9824562f, 0.99330735f, 0.f, 0.f, 0.37139067f, 0.72760683f, 0.4850712f,
       0.5848977f, 0.67488194f, 0.7581754f, 0.58321184f, 0.86747235f, 0.4048204f});

  sd::ops::lrn op;
  auto results = op.evaluate({&x}, {1.0, 1.0, 0.5}, {5});
  auto out = results.at(0);

  ASSERT_EQ(sd::Status::OK, results.status());
  ASSERT_TRUE(exp.isSameShape(out));
  // out->printIndexedBuffer("LRN out");
  // exp.printIndexedBuffer("LRN exp");
  ASSERT_TRUE(exp.equalsTo(out));
}

////////////////////////////////////////////////////////////////////////////////
TYPED_TEST(TypedDeclarableOpsTests4, LrnTest_2) {
  auto x = NDArrayFactory::create<TypeParam>(
      'c', {2, 2, 2, 2}, {5.5f, 0.f, 0.3f, 5.5f, 8.6f, 0.f, 0.f, 0.4f, 1.5f, 1.f, 1.3f, 1.5f, 2.6f, 2.f, 3.f, 1.4f});

  auto exp = NDArrayFactory::create<TypeParam>(
      'c', {2, 2, 2, 2},
      {0.98386997f, 0.f, 0.05358852f, 0.9824562f, 0.99330735f, 0.f, 0.f, 0.37139067f, 0.72760683f, 0.4850712f,
       0.5848977f, 0.67488194f, 0.7581754f, 0.58321184f, 0.86747235f, 0.4048204f});

  sd::ops::lrn op;
  auto results = op.evaluate({&x}, {1.0, 1.0, 0.5}, {2});
  auto out = results.at(0);

  ASSERT_EQ(sd::Status::OK, results.status());
  ASSERT_TRUE(exp.isSameShape(out));
  // out->printIndexedBuffer("LRN out");
  // exp.printIndexedBuffer("LRN exp");
  ASSERT_TRUE(exp.equalsTo(out));
}

////////////////////////////////////////////////////////////////////////////////
TYPED_TEST(TypedDeclarableOpsTests4, LrnTest_3) {
  auto x = NDArrayFactory::create<TypeParam>(
      'c', {2, 2, 2, 4},
      {

          5.5f, 0.f, 0.3f, 5.5f, 1.5f, 0.f, 1.3f, 6.5f, 8.6f, 0.f, 0.f, 0.4f, 2.5f, 1.f, 0.3f, 4.5f,
          1.5f, 1.f, 1.3f, 1.5f, 3.5f, 0.f, 1.3f, 2.5f, 2.6f, 2.f, 3.f, 1.4f, 4.5f, 1.f, 0.3f, 0.5f});

  auto exp = NDArrayFactory::create<TypeParam>(
      'c', {2, 2, 2, 4},
      {0.9824562f,  0.f,         0.03822664f, 0.9824562f,  0.67488194f, 0.f,         0.18924236f, 0.96960944f,
       0.99330735f, 0.f,         0.f,         0.37139067f, 0.86567914f, 0.18702209f, 0.05610663f, 0.9520745f,
       0.6154575f,  0.34942827f, 0.45425674f, 0.6154575f,  0.905509f,   0.f,         0.2824086f,  0.8361251f,
       0.57063663f, 0.41959068f, 0.629386f,   0.3504383f,  0.9520745f,  0.21039814f, 0.06311944f, 0.3268602f});

  sd::ops::lrn op;
  auto results = op.evaluate({&x}, {1.0, 1.0, 0.5}, {2});
  auto out = results.at(0);

  ASSERT_EQ(sd::Status::OK, results.status());
  ASSERT_TRUE(exp.isSameShape(out));
  // out->printIndexedBuffer("LRN out");
  // exp.printIndexedBuffer("LRN exp");
  ASSERT_TRUE(exp.equalsTo(out));
}

////////////////////////////////////////////////////////////////////////////////
TYPED_TEST(TypedDeclarableOpsTests4, LrnTest_4) {
  auto x = NDArrayFactory::create<TypeParam>(
      'c', {2, 2, 2, 4},
      {

          5.5f, 0.f, 0.3f, 5.5f, 1.5f, 0.f, 1.3f, 6.5f, 8.6f, 0.f, 0.f, 0.4f, 2.5f, 1.f, 0.3f, 4.5f,
          1.5f, 1.f, 1.3f, 1.5f, 3.5f, 0.f, 1.3f, 2.5f, 2.6f, 2.f, 3.f, 1.4f, 4.5f, 1.f, 0.3f, 0.5f});

  auto exp = NDArrayFactory::create<TypeParam>(
      'c', {2, 2, 2, 4},
      {0.70082176f, 0.f,         0.03822664f, 0.70082176f, 0.21835658f, 0.f,         0.18924236f, 0.9462118f,
       0.9922489f,  0.f,         0.f,         0.04615111f, 0.46755522f, 0.18702209f, 0.05610663f, 0.8415994f,
       0.5241424f,  0.34942827f, 0.45425674f, 0.5241424f,  0.76033086f, 0.f,         0.2824086f,  0.54309344f,
       0.54546785f, 0.41959068f, 0.629386f,   0.29371348f, 0.94679165f, 0.21039814f, 0.06311944f, 0.10519907f});

  sd::ops::lrn op;
  auto results = op.evaluate({&x}, {1.0, 1.0, 0.5}, {5});
  auto out = results.at(0);

  ASSERT_EQ(sd::Status::OK, results.status());
  ASSERT_TRUE(exp.isSameShape(out));
  // out->printIndexedBuffer("LRN out");
  // exp.printIndexedBuffer("LRN exp");
  ASSERT_TRUE(exp.equalsTo(out));
}

////////////////////////////////////////////////////////////////////////////////
TYPED_TEST(TypedDeclarableOpsTests4, LrnTest_5) {
  auto x = NDArrayFactory::create<TypeParam>(
      'c', {2, 2, 2, 4},
      {

          5.5f, 0.f, 0.3f, 5.5f, 1.5f, 0.f, 1.3f, 6.5f, 8.6f, 0.f, 0.f, 0.4f, 2.5f, 1.f, 0.3f, 4.5f,
          1.5f, 1.f, 1.3f, 1.5f, 3.5f, 0.f, 1.3f, 2.5f, 2.6f, 2.f, 3.f, 1.4f, 4.5f, 1.f, 0.3f, 0.5f});

  auto eps = NDArrayFactory::create<TypeParam>(
      'c', {2, 2, 2, 4},
      {0.70082176f, 0.f,         0.03822664f, 0.70082176f, 0.21835658f, 0.f,         0.18924236f, 0.9462118f,

       0.9922489f,  0.f,         0.f,         0.04615111f, 0.46755522f, 0.18702209f, 0.05610663f, 0.8415994f,

       0.5241424f,  0.34942827f, 0.45425674f, 0.5241424f,  0.76033086f, 0.f,         0.2824086f,  0.54309344f,

       0.54546785f, 0.41959068f, 0.629386f,   0.29371348f, 0.94679165f, 0.21039814f, 0.06311944f, 0.10519907f});

  auto exp = NDArrayFactory::create<TypeParam>('c', {2, 2, 2, 4});

  sd::ops::lrn_bp op;
  auto results = op.evaluate({&x, &eps}, {1.0, 1.0, 0.5}, {5}, {}, {}, false);
  auto out = results.at(0);

  ASSERT_EQ(sd::Status::OK, results.status());
  ASSERT_TRUE(exp.isSameShape(out));
  // out->printIndexedBuffer("LRN out");
  // exp.printIndexedBuffer("LRN exp");
  //    ASSERT_TRUE(exp.equalsTo(out));
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests4, tri_test1) {
  const int rows = 3;
  const int cols = 5;

  auto expected = NDArrayFactory::create<float>(
      'c', {rows, cols}, {1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f, 0.f, 0.f, 0.f, 1.f, 1.f, 1.f, 0.f, 0.f});

  sd::ops::tri op;
  auto results = op.evaluate({}, {}, {rows, cols});
  auto output = results.at(0);

  // output->printIndexedBuffer();

  ASSERT_EQ(sd::Status::OK, results.status());

  ASSERT_TRUE(expected.isSameShape(output));
  ASSERT_TRUE(expected.equalsTo(output));
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests4, tri_test2) {
  const int rows = 3;
  const int cols = 5;
  const int diag = 2;

  auto expected = NDArrayFactory::create<float>(
      'c', {rows, cols}, {1.f, 1.f, 1.f, 0.f, 0.f, 1.f, 1.f, 1.f, 1.f, 0.f, 1.f, 1.f, 1.f, 1.f, 1.f});

  sd::ops::tri op;
  auto results = op.evaluate({}, {}, {rows, cols, diag});
  auto output = results.at(0);

  ASSERT_EQ(sd::Status::OK, results.status());

  ASSERT_TRUE(expected.isSameShape(output));
  ASSERT_TRUE(expected.equalsTo(output));
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests4, tri_test3) {
  const int rows = 3;
  const int cols = 5;
  const int diag = -1;

  auto expected = NDArrayFactory::create<float>(
      'c', {rows, cols}, {0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f, 0.f, 0.f, 0.f});

  sd::ops::tri op;
  auto results = op.evaluate({}, {}, {rows, cols, diag});
  auto output = results.at(0);

  ASSERT_EQ(sd::Status::OK, results.status());

  ASSERT_TRUE(expected.isSameShape(output));
  ASSERT_TRUE(expected.equalsTo(output));
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests4, tri_test4) {
  const int rows = 3;
  const int cols = 5;
  const int diag = -2;

  auto expected = NDArrayFactory::create<float>(
      'c', {rows, cols}, {0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f});

  sd::ops::tri op;
  auto results = op.evaluate({}, {}, {rows, cols, diag});
  auto output = results.at(0);

  ASSERT_EQ(sd::Status::OK, results.status());

  ASSERT_TRUE(expected.isSameShape(output));
  ASSERT_TRUE(expected.equalsTo(output));
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests4, tri_test5) {
  const int rows = 5;

  auto expected =
      NDArrayFactory::create<float>('c', {rows, rows}, {1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f, 0.f, 0.f, 0.f, 1.f, 1.f, 1.f,
                                                        0.f, 0.f, 1.f, 1.f, 1.f, 1.f, 0.f, 1.f, 1.f, 1.f, 1.f, 1.f});

  sd::ops::tri op;
  auto results = op.evaluate({}, {}, {rows});
  auto output = results.at(0);

  ASSERT_EQ(sd::Status::OK, results.status());

  ASSERT_TRUE(expected.isSameShape(output));
  ASSERT_TRUE(expected.equalsTo(output));
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests4, tri_test6) {
  const int rows = 3;
  const int cols = 5;
  const int diag = -20;

  auto expected = NDArrayFactory::create<float>(
      'c', {rows, cols}, {0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f});

  sd::ops::tri op;
  auto results = op.evaluate({}, {}, {rows, cols, diag});
  auto output = results.at(0);

  ASSERT_EQ(sd::Status::OK, results.status());

  ASSERT_TRUE(expected.isSameShape(output));
  ASSERT_TRUE(expected.equalsTo(output));
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests4, tri_test7) {
  const int rows = 3;
  const int cols = 5;
  const int diag = 20;

  auto expected = NDArrayFactory::create<float>(
      'c', {rows, cols}, {1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f});

  sd::ops::tri op;
  auto results = op.evaluate({}, {}, {rows, cols, diag});
  auto output = results.at(0);

  ASSERT_EQ(sd::Status::OK, results.status());

  ASSERT_TRUE(expected.isSameShape(output));
  ASSERT_TRUE(expected.equalsTo(output));
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests4, triu_test1) {
  auto input = NDArrayFactory::create<double>('c', {4, 3}, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12});
  auto expected = NDArrayFactory::create<double>('c', {4, 3}, {1, 2, 3, 0, 5, 6, 0, 0, 9, 0, 0, 0});

  sd::ops::triu op;
  auto results = op.evaluate({&input}, {}, {});
  auto output = results.at(0);

  ASSERT_EQ(sd::Status::OK, results.status());

  ASSERT_TRUE(expected.isSameShape(output));
  ASSERT_TRUE(expected.equalsTo(output));
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests4, triu_test2) {
  auto input = NDArrayFactory::create<double>('c', {4, 3}, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12});
  auto expected = NDArrayFactory::create<double>('c', {4, 3}, {1, 2, 3, 4, 5, 6, 0, 8, 9, 0, 0, 12});

  sd::ops::triu op;
  auto results = op.evaluate({&input}, {}, {-1});
  auto output = results.at(0);

  ASSERT_EQ(sd::Status::OK, results.status());

  ASSERT_TRUE(expected.isSameShape(output));
  ASSERT_TRUE(expected.equalsTo(output));
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests4, triu_test3) {
  auto input = NDArrayFactory::create<double>('c', {2, 3, 2}, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12});
  auto expected = NDArrayFactory::create<double>('c', {2, 3, 2}, {1, 2, 3, 4, 0, 6, 7, 8, 9, 10, 0, 12});

  sd::ops::triu op;
  auto results = op.evaluate({&input}, {}, {-1});
  auto output = results.at(0);

  ASSERT_EQ(sd::Status::OK, results.status());

  ASSERT_TRUE(expected.isSameShape(output));
  ASSERT_TRUE(expected.equalsTo(output));
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests4, triu_test4) {
  auto input = NDArrayFactory::create<double>('c', {2, 3, 2}, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12});
  auto expected = NDArrayFactory::create<double>('c', {2, 3, 2}, {1, 2, 0, 4, 0, 0, 7, 8, 0, 10, 0, 0});

  sd::ops::triu op;
  auto results = op.evaluate({&input}, {}, {});
  auto output = results.at(0);

  ASSERT_EQ(sd::Status::OK, results.status());

  ASSERT_TRUE(expected.isSameShape(output));
  ASSERT_TRUE(expected.equalsTo(output));
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests4, triu_test5) {
  auto input = NDArrayFactory::create<double>('c', {2, 3, 2}, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12});
  auto expected = NDArrayFactory::create<double>('c', {2, 3, 2}, {0, 2, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0});

  sd::ops::triu op;
  auto results = op.evaluate({&input}, {}, {1});
  auto output = results.at(0);

  ASSERT_EQ(sd::Status::OK, results.status());

  ASSERT_TRUE(expected.isSameShape(output));
  ASSERT_TRUE(expected.equalsTo(output));
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests4, triu_test6) {
  auto input = NDArrayFactory::create<double>('c', {2, 3, 2}, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12});
  auto expected = NDArrayFactory::create<double>('c', {2, 3, 2}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});

  sd::ops::triu op;
  auto results = op.evaluate({&input}, {}, {10});
  auto output = results.at(0);

  ASSERT_EQ(sd::Status::OK, results.status());

  ASSERT_TRUE(expected.isSameShape(output));
  ASSERT_TRUE(expected.equalsTo(output));
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests4, triu_test7) {
  auto input = NDArrayFactory::create<double>('c', {2, 3, 2}, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12});
  auto expected = NDArrayFactory::create<double>('c', {2, 3, 2}, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12});

  sd::ops::triu op;
  auto results = op.evaluate({&input}, {}, {-10});
  auto output = results.at(0);

  ASSERT_EQ(sd::Status::OK, results.status());

  ASSERT_TRUE(expected.isSameShape(output));
  ASSERT_TRUE(expected.equalsTo(output));
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests4, triu_test8) {
  auto input = NDArrayFactory::create<double>('c', {6}, {1, 2, 3, 4, 5, 6});
  auto expected = NDArrayFactory::create<double>('c', {6, 6}, {1, 2, 3, 4, 5, 6, 0, 2, 3, 4, 5, 6, 0, 0, 3, 4, 5, 6,
                                                               0, 0, 0, 4, 5, 6, 0, 0, 0, 0, 5, 6, 0, 0, 0, 0, 0, 6});

  sd::ops::triu op;
  auto results = op.evaluate({&input}, {}, {});
  auto output = results.at(0);

  ASSERT_EQ(sd::Status::OK, results.status());

  ASSERT_TRUE(expected.isSameShape(output));
  ASSERT_TRUE(expected.equalsTo(output));
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests4, triu_test9) {
  auto input = NDArrayFactory::create<double>('c', {6}, {1, 2, 3, 4, 5, 6});
  auto expected = NDArrayFactory::create<double>('c', {6, 6}, {1, 2, 3, 4, 5, 6, 1, 2, 3, 4, 5, 6, 1, 2, 3, 4, 5, 6,
                                                               1, 2, 3, 4, 5, 6, 0, 2, 3, 4, 5, 6, 0, 0, 3, 4, 5, 6});

  sd::ops::triu op;
  auto results = op.evaluate({&input}, {}, {-3});
  auto output = results.at(0);

  ASSERT_EQ(sd::Status::OK, results.status());

  ASSERT_TRUE(expected.isSameShape(output));
  ASSERT_TRUE(expected.equalsTo(output));
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests4, triu_test10) {
  auto input = NDArrayFactory::create<double>('c', {6}, {1, 2, 3, 4, 5, 6});
  auto expected = NDArrayFactory::create<double>('c', {6, 6}, {0, 0, 0, 4, 5, 6, 0, 0, 0, 0, 5, 6, 0, 0, 0, 0, 0, 6,
                                                               0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});

  sd::ops::triu op;
  auto results = op.evaluate({&input}, {}, {3});
  auto output = results.at(0);

  ASSERT_EQ(sd::Status::OK, results.status());

  ASSERT_TRUE(expected.isSameShape(output));
  ASSERT_TRUE(expected.equalsTo(output));
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests4, triu_test11) {
  auto input = NDArrayFactory::create<double>('c', {6}, {1, 2, 3, 4, 5, 6});
  auto expected = NDArrayFactory::create<double>('c', {6, 6}, {1, 2, 3, 4, 5, 6, 1, 2, 3, 4, 5, 6, 1, 2, 3, 4, 5, 6,
                                                               1, 2, 3, 4, 5, 6, 1, 2, 3, 4, 5, 6, 1, 2, 3, 4, 5, 6});

  sd::ops::triu op;
  auto results = op.evaluate({&input}, {}, {-58});
  auto output = results.at(0);

  ASSERT_EQ(sd::Status::OK, results.status());

  ASSERT_TRUE(expected.isSameShape(output));
  ASSERT_TRUE(expected.equalsTo(output));
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests4, triu_bp_test1) {
  auto input = NDArrayFactory::create<double>('c', {2, 3, 2}, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12});
  auto gradO = NDArrayFactory::create<double>('c', {2, 3, 2});
  gradO = 0.5;

  auto expected = NDArrayFactory::create<double>('c', {2, 3, 2}, {0., 0.5, 0., 0., 0., 0., 0., 0.5, 0., 0., 0., 0.});

  sd::ops::triu_bp op;
  auto results = op.evaluate({&input, &gradO}, {}, {1});
  auto gradI = results.at(0);

  ASSERT_EQ(sd::Status::OK, results.status());

  ASSERT_TRUE(expected.isSameShape(gradI));
  ASSERT_TRUE(expected.equalsTo(gradI));
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests4, triu_bp_test2) {
  auto input = NDArrayFactory::create<double>('c', {2, 3, 2}, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12});
  auto gradO = NDArrayFactory::create<double>('c', {2, 3, 2});
  gradO = 0.5;

  auto expected =
      NDArrayFactory::create<double>('c', {2, 3, 2}, {0.5, 0.5, 0., 0.5, 0., 0., 0.5, 0.5, 0., 0.5, 0., 0.});

  sd::ops::triu_bp op;
  auto results = op.evaluate({&input, &gradO}, {}, {});
  auto gradI = results.at(0);

  ASSERT_EQ(sd::Status::OK, results.status());

  ASSERT_TRUE(expected.isSameShape(gradI));
  ASSERT_TRUE(expected.equalsTo(gradI));
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests4, triu_bp_test3) {
  auto input = NDArrayFactory::create<double>('c', {6}, {1, 2, 3, 4, 5, 6});
  auto gradO = NDArrayFactory::create<double>('c', {6, 6});
  gradO = 0.5;

  auto expected = NDArrayFactory::create<double>(
      'c', {6, 6}, {0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
                    0.,  0.5, 0.5, 0.5, 0.5, 0.5, 0.,  0.,  0.5, 0.5, 0.5, 0.5, 0.,  0.,  0.,  0.5, 0.5, 0.5});

  sd::ops::triu_bp op;
  auto results = op.evaluate({&input, &gradO}, {}, {-2});
  auto gradI = results.at(0);

  ASSERT_EQ(sd::Status::OK, results.status());

  ASSERT_TRUE(expected.isSameShape(gradI));
  ASSERT_TRUE(expected.equalsTo(gradI));
}

//////////////////////////////////////////////////////////////////////
TEST_F(DeclarableOpsTests4, triu_bp_test4) {
  auto input = NDArrayFactory::create<double>('c', {2, 3}, {1, 2, 3, 4, 5, 6});
  auto gradO = NDArrayFactory::create<double>('c', {2, 3});
  gradO = 0.5;

  auto expected = NDArrayFactory::create<double>('c', {2, 3}, {0., 0., 0., 0., 0., 0.});

  sd::ops::triu_bp op;
  auto results = op.evaluate({&input, &gradO}, {}, {10});
  auto gradI = results.at(0);

  ASSERT_EQ(sd::Status::OK, results.status());

  ASSERT_TRUE(expected.isSameShape(gradI));
  ASSERT_TRUE(expected.equalsTo(gradI));
}
