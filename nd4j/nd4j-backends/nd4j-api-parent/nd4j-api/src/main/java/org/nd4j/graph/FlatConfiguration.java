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
public final class FlatConfiguration extends Table {
  public static void ValidateVersion() { Constants.FLATBUFFERS_1_12_0(); }
  public static FlatConfiguration getRootAsFlatConfiguration(ByteBuffer _bb) { return getRootAsFlatConfiguration(_bb, new FlatConfiguration()); }
  public static FlatConfiguration getRootAsFlatConfiguration(ByteBuffer _bb, FlatConfiguration obj) { _bb.order(java.nio.ByteOrder.LITTLE_ENDIAN); return (obj.__assign(_bb.getInt(_bb.position()) + _bb.position(), _bb)); }
  public void __init(int _i, ByteBuffer _bb) { __reset(_i, _bb); }
  public FlatConfiguration __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }

  public long id() { int o = __offset(4); return o != 0 ? bb.getLong(o + bb_pos) : 0L; }
  public byte executionMode() { int o = __offset(6); return o != 0 ? bb.get(o + bb_pos) : 0; }
  public byte profilingMode() { int o = __offset(8); return o != 0 ? bb.get(o + bb_pos) : 0; }
  public byte outputMode() { int o = __offset(10); return o != 0 ? bb.get(o + bb_pos) : 0; }
  public boolean timestats() { int o = __offset(12); return o != 0 ? 0!=bb.get(o + bb_pos) : false; }
  public long footprintForward() { int o = __offset(14); return o != 0 ? bb.getLong(o + bb_pos) : 0L; }
  public long footprintBackward() { int o = __offset(16); return o != 0 ? bb.getLong(o + bb_pos) : 0L; }
  public byte direction() { int o = __offset(18); return o != 0 ? bb.get(o + bb_pos) : 0; }

  public static int createFlatConfiguration(FlatBufferBuilder builder,
      long id,
      byte executionMode,
      byte profilingMode,
      byte outputMode,
      boolean timestats,
      long footprintForward,
      long footprintBackward,
      byte direction) {
    builder.startTable(8);
    FlatConfiguration.addFootprintBackward(builder, footprintBackward);
    FlatConfiguration.addFootprintForward(builder, footprintForward);
    FlatConfiguration.addId(builder, id);
    FlatConfiguration.addDirection(builder, direction);
    FlatConfiguration.addTimestats(builder, timestats);
    FlatConfiguration.addOutputMode(builder, outputMode);
    FlatConfiguration.addProfilingMode(builder, profilingMode);
    FlatConfiguration.addExecutionMode(builder, executionMode);
    return FlatConfiguration.endFlatConfiguration(builder);
  }

  public static void startFlatConfiguration(FlatBufferBuilder builder) { builder.startTable(8); }
  public static void addId(FlatBufferBuilder builder, long id) { builder.addLong(0, id, 0L); }
  public static void addExecutionMode(FlatBufferBuilder builder, byte executionMode) { builder.addByte(1, executionMode, 0); }
  public static void addProfilingMode(FlatBufferBuilder builder, byte profilingMode) { builder.addByte(2, profilingMode, 0); }
  public static void addOutputMode(FlatBufferBuilder builder, byte outputMode) { builder.addByte(3, outputMode, 0); }
  public static void addTimestats(FlatBufferBuilder builder, boolean timestats) { builder.addBoolean(4, timestats, false); }
  public static void addFootprintForward(FlatBufferBuilder builder, long footprintForward) { builder.addLong(5, footprintForward, 0L); }
  public static void addFootprintBackward(FlatBufferBuilder builder, long footprintBackward) { builder.addLong(6, footprintBackward, 0L); }
  public static void addDirection(FlatBufferBuilder builder, byte direction) { builder.addByte(7, direction, 0); }
  public static int endFlatConfiguration(FlatBufferBuilder builder) {
    int o = builder.endTable();
    return o;
  }
  public static void finishFlatConfigurationBuffer(FlatBufferBuilder builder, int offset) { builder.finish(offset); }
  public static void finishSizePrefixedFlatConfigurationBuffer(FlatBufferBuilder builder, int offset) { builder.finishSizePrefixed(offset); }

  public static final class Vector extends BaseVector {
    public Vector __assign(int _vector, int _element_size, ByteBuffer _bb) { __reset(_vector, _element_size, _bb); return this; }

    public FlatConfiguration get(int j) { return get(new FlatConfiguration(), j); }
    public FlatConfiguration get(FlatConfiguration obj, int j) {  return obj.__assign(__indirect(__element(j), bb), bb); }
  }
}

