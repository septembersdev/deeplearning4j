/*
 *  ******************************************************************************
 *  *
 *  *
 *  * This program and the accompanying materials are made available under the
 *  * terms of the Apache License, Version 2.0 which is available at
 *  * https://www.apache.org/licenses/LICENSE-2.0.
 *  *
 *  *  See the NOTICE file distributed with this work for additional
 *  *  information regarding copyright ownership.
 *  * Unless required by applicable law or agreed to in writing, software
 *  * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 *  * License for the specific language governing permissions and limitations
 *  * under the License.
 *  *
 *  * SPDX-License-Identifier: Apache-2.0
 *  *****************************************************************************
 */

package org.nd4j.graph;

import java.nio.*;
import java.lang.*;
import java.util.*;
import com.google.flatbuffers.*;

@SuppressWarnings("unused")
public final class FlatProperties extends Table {
  public static void ValidateVersion() { Constants.FLATBUFFERS_1_12_0(); }
  public static FlatProperties getRootAsFlatProperties(ByteBuffer _bb) { return getRootAsFlatProperties(_bb, new FlatProperties()); }
  public static FlatProperties getRootAsFlatProperties(ByteBuffer _bb, FlatProperties obj) { _bb.order(java.nio.ByteOrder.LITTLE_ENDIAN); return (obj.__assign(_bb.getInt(_bb.position()) + _bb.position(), _bb)); }
  public void __init(int _i, ByteBuffer _bb) { __reset(_i, _bb); }
  public FlatProperties __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }

  public String name() { int o = __offset(4); return o != 0 ? __string(o + bb_pos) : null; }
  public ByteBuffer nameAsByteBuffer() { return __vector_as_bytebuffer(4, 1); }
  public ByteBuffer nameInByteBuffer(ByteBuffer _bb) { return __vector_in_bytebuffer(_bb, 4, 1); }
  public int i(int j) { int o = __offset(6); return o != 0 ? bb.getInt(__vector(o) + j * 4) : 0; }
  public int iLength() { int o = __offset(6); return o != 0 ? __vector_len(o) : 0; }
  public IntVector iVector() { return iVector(new IntVector()); }
  public IntVector iVector(IntVector obj) { int o = __offset(6); return o != 0 ? obj.__assign(__vector(o), bb) : null; }
  public ByteBuffer iAsByteBuffer() { return __vector_as_bytebuffer(6, 4); }
  public ByteBuffer iInByteBuffer(ByteBuffer _bb) { return __vector_in_bytebuffer(_bb, 6, 4); }
  public long l(int j) { int o = __offset(8); return o != 0 ? bb.getLong(__vector(o) + j * 8) : 0; }
  public int lLength() { int o = __offset(8); return o != 0 ? __vector_len(o) : 0; }
  public LongVector lVector() { return lVector(new LongVector()); }
  public LongVector lVector(LongVector obj) { int o = __offset(8); return o != 0 ? obj.__assign(__vector(o), bb) : null; }
  public ByteBuffer lAsByteBuffer() { return __vector_as_bytebuffer(8, 8); }
  public ByteBuffer lInByteBuffer(ByteBuffer _bb) { return __vector_in_bytebuffer(_bb, 8, 8); }
  public double d(int j) { int o = __offset(10); return o != 0 ? bb.getDouble(__vector(o) + j * 8) : 0; }
  public int dLength() { int o = __offset(10); return o != 0 ? __vector_len(o) : 0; }
  public DoubleVector dVector() { return dVector(new DoubleVector()); }
  public DoubleVector dVector(DoubleVector obj) { int o = __offset(10); return o != 0 ? obj.__assign(__vector(o), bb) : null; }
  public ByteBuffer dAsByteBuffer() { return __vector_as_bytebuffer(10, 8); }
  public ByteBuffer dInByteBuffer(ByteBuffer _bb) { return __vector_in_bytebuffer(_bb, 10, 8); }
  public FlatArray a(int j) { return a(new FlatArray(), j); }
  public FlatArray a(FlatArray obj, int j) { int o = __offset(12); return o != 0 ? obj.__assign(__indirect(__vector(o) + j * 4), bb) : null; }
  public int aLength() { int o = __offset(12); return o != 0 ? __vector_len(o) : 0; }
  public FlatArray.Vector aVector() { return aVector(new FlatArray.Vector()); }
  public FlatArray.Vector aVector(FlatArray.Vector obj) { int o = __offset(12); return o != 0 ? obj.__assign(__vector(o), 4, bb) : null; }
  public boolean b(int j) { int o = __offset(14); return o != 0 ? 0!=bb.get(__vector(o) + j * 1) : false; }
  public int bLength() { int o = __offset(14); return o != 0 ? __vector_len(o) : 0; }
  public BooleanVector bVector() { return bVector(new BooleanVector()); }
  public BooleanVector bVector(BooleanVector obj) { int o = __offset(14); return o != 0 ? obj.__assign(__vector(o), bb) : null; }
  public ByteBuffer bAsByteBuffer() { return __vector_as_bytebuffer(14, 1); }
  public ByteBuffer bInByteBuffer(ByteBuffer _bb) { return __vector_in_bytebuffer(_bb, 14, 1); }
  public String s(int j) { int o = __offset(16); return o != 0 ? __string(__vector(o) + j * 4) : null; }
  public int sLength() { int o = __offset(16); return o != 0 ? __vector_len(o) : 0; }
  public StringVector sVector() { return sVector(new StringVector()); }
  public StringVector sVector(StringVector obj) { int o = __offset(16); return o != 0 ? obj.__assign(__vector(o), 4, bb) : null; }
  public int shape(int j) { int o = __offset(18); return o != 0 ? bb.getInt(__vector(o) + j * 4) : 0; }
  public int shapeLength() { int o = __offset(18); return o != 0 ? __vector_len(o) : 0; }
  public IntVector shapeVector() { return shapeVector(new IntVector()); }
  public IntVector shapeVector(IntVector obj) { int o = __offset(18); return o != 0 ? obj.__assign(__vector(o), bb) : null; }
  public ByteBuffer shapeAsByteBuffer() { return __vector_as_bytebuffer(18, 4); }
  public ByteBuffer shapeInByteBuffer(ByteBuffer _bb) { return __vector_in_bytebuffer(_bb, 18, 4); }

  public static int createFlatProperties(FlatBufferBuilder builder,
      int nameOffset,
      int iOffset,
      int lOffset,
      int dOffset,
      int aOffset,
      int bOffset,
      int sOffset,
      int shapeOffset) {
    builder.startTable(8);
    FlatProperties.addShape(builder, shapeOffset);
    FlatProperties.addS(builder, sOffset);
    FlatProperties.addB(builder, bOffset);
    FlatProperties.addA(builder, aOffset);
    FlatProperties.addD(builder, dOffset);
    FlatProperties.addL(builder, lOffset);
    FlatProperties.addI(builder, iOffset);
    FlatProperties.addName(builder, nameOffset);
    return FlatProperties.endFlatProperties(builder);
  }

  public static void startFlatProperties(FlatBufferBuilder builder) { builder.startTable(8); }
  public static void addName(FlatBufferBuilder builder, int nameOffset) { builder.addOffset(0, nameOffset, 0); }
  public static void addI(FlatBufferBuilder builder, int iOffset) { builder.addOffset(1, iOffset, 0); }
  public static int createIVector(FlatBufferBuilder builder, int[] data) { builder.startVector(4, data.length, 4); for (int i = data.length - 1; i >= 0; i--) builder.addInt(data[i]); return builder.endVector(); }
  public static void startIVector(FlatBufferBuilder builder, int numElems) { builder.startVector(4, numElems, 4); }
  public static void addL(FlatBufferBuilder builder, int lOffset) { builder.addOffset(2, lOffset, 0); }
  public static int createLVector(FlatBufferBuilder builder, long[] data) { builder.startVector(8, data.length, 8); for (int i = data.length - 1; i >= 0; i--) builder.addLong(data[i]); return builder.endVector(); }
  public static void startLVector(FlatBufferBuilder builder, int numElems) { builder.startVector(8, numElems, 8); }
  public static void addD(FlatBufferBuilder builder, int dOffset) { builder.addOffset(3, dOffset, 0); }
  public static int createDVector(FlatBufferBuilder builder, double[] data) { builder.startVector(8, data.length, 8); for (int i = data.length - 1; i >= 0; i--) builder.addDouble(data[i]); return builder.endVector(); }
  public static void startDVector(FlatBufferBuilder builder, int numElems) { builder.startVector(8, numElems, 8); }
  public static void addA(FlatBufferBuilder builder, int aOffset) { builder.addOffset(4, aOffset, 0); }
  public static int createAVector(FlatBufferBuilder builder, int[] data) { builder.startVector(4, data.length, 4); for (int i = data.length - 1; i >= 0; i--) builder.addOffset(data[i]); return builder.endVector(); }
  public static void startAVector(FlatBufferBuilder builder, int numElems) { builder.startVector(4, numElems, 4); }
  public static void addB(FlatBufferBuilder builder, int bOffset) { builder.addOffset(5, bOffset, 0); }
  public static int createBVector(FlatBufferBuilder builder, boolean[] data) { builder.startVector(1, data.length, 1); for (int i = data.length - 1; i >= 0; i--) builder.addBoolean(data[i]); return builder.endVector(); }
  public static void startBVector(FlatBufferBuilder builder, int numElems) { builder.startVector(1, numElems, 1); }
  public static void addS(FlatBufferBuilder builder, int sOffset) { builder.addOffset(6, sOffset, 0); }
  public static int createSVector(FlatBufferBuilder builder, int[] data) { builder.startVector(4, data.length, 4); for (int i = data.length - 1; i >= 0; i--) builder.addOffset(data[i]); return builder.endVector(); }
  public static void startSVector(FlatBufferBuilder builder, int numElems) { builder.startVector(4, numElems, 4); }
  public static void addShape(FlatBufferBuilder builder, int shapeOffset) { builder.addOffset(7, shapeOffset, 0); }
  public static int createShapeVector(FlatBufferBuilder builder, int[] data) { builder.startVector(4, data.length, 4); for (int i = data.length - 1; i >= 0; i--) builder.addInt(data[i]); return builder.endVector(); }
  public static void startShapeVector(FlatBufferBuilder builder, int numElems) { builder.startVector(4, numElems, 4); }
  public static int endFlatProperties(FlatBufferBuilder builder) {
    int o = builder.endTable();
    return o;
  }
  public static void finishFlatPropertiesBuffer(FlatBufferBuilder builder, int offset) { builder.finish(offset); }
  public static void finishSizePrefixedFlatPropertiesBuffer(FlatBufferBuilder builder, int offset) { builder.finishSizePrefixed(offset); }

  public static final class Vector extends BaseVector {
    public Vector __assign(int _vector, int _element_size, ByteBuffer _bb) { __reset(_vector, _element_size, _bb); return this; }

    public FlatProperties get(int j) { return get(new FlatProperties(), j); }
    public FlatProperties get(FlatProperties obj, int j) {  return obj.__assign(__indirect(__element(j), bb), bb); }
  }
}

