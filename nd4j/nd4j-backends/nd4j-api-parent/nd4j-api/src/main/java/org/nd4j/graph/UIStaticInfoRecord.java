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

import com.google.flatbuffers.BaseVector;
import com.google.flatbuffers.Constants;
import com.google.flatbuffers.FlatBufferBuilder;
import com.google.flatbuffers.Table;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;

@SuppressWarnings("unused")
public final class UIStaticInfoRecord extends Table {
  public static void ValidateVersion() { Constants.FLATBUFFERS_1_12_0(); }
  public static UIStaticInfoRecord getRootAsUIStaticInfoRecord(ByteBuffer _bb) { return getRootAsUIStaticInfoRecord(_bb, new UIStaticInfoRecord()); }
  public static UIStaticInfoRecord getRootAsUIStaticInfoRecord(ByteBuffer _bb, UIStaticInfoRecord obj) { _bb.order(ByteOrder.LITTLE_ENDIAN); return (obj.__assign(_bb.getInt(_bb.position()) + _bb.position(), _bb)); }
  public void __init(int _i, ByteBuffer _bb) { __reset(_i, _bb); }
  public UIStaticInfoRecord __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }

  public byte infoType() { int o = __offset(4); return o != 0 ? bb.get(o + bb_pos) : 0; }

  public static int createUIStaticInfoRecord(FlatBufferBuilder builder,
      byte infoType) {
    builder.startTable(1);
    UIStaticInfoRecord.addInfoType(builder, infoType);
    return UIStaticInfoRecord.endUIStaticInfoRecord(builder);
  }

  public static void startUIStaticInfoRecord(FlatBufferBuilder builder) { builder.startTable(1); }
  public static void addInfoType(FlatBufferBuilder builder, byte infoType) { builder.addByte(0, infoType, 0); }
  public static int endUIStaticInfoRecord(FlatBufferBuilder builder) {
    int o = builder.endTable();
    return o;
  }

  public static final class Vector extends BaseVector {
    public Vector __assign(int _vector, int _element_size, ByteBuffer _bb) { __reset(_vector, _element_size, _bb); return this; }

    public UIStaticInfoRecord get(int j) { return get(new UIStaticInfoRecord(), j); }
    public UIStaticInfoRecord get(UIStaticInfoRecord obj, int j) {  return obj.__assign(__indirect(__element(j), bb), bb); }
  }
}

