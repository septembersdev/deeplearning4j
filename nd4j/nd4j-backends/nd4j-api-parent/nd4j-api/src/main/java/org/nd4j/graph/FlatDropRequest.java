// automatically generated by the FlatBuffers compiler, do not modify

package org.nd4j.graph;

import java.nio.*;
import java.lang.*;
import java.util.*;
import com.google.flatbuffers.*;

@SuppressWarnings("unused")
public final class FlatDropRequest extends Table {
  public static void ValidateVersion() { Constants.FLATBUFFERS_1_12_0(); }
  public static FlatDropRequest getRootAsFlatDropRequest(ByteBuffer _bb) { return getRootAsFlatDropRequest(_bb, new FlatDropRequest()); }
  public static FlatDropRequest getRootAsFlatDropRequest(ByteBuffer _bb, FlatDropRequest obj) { _bb.order(java.nio.ByteOrder.LITTLE_ENDIAN); return (obj.__assign(_bb.getInt(_bb.position()) + _bb.position(), _bb)); }
  public void __init(int _i, ByteBuffer _bb) { __reset(_i, _bb); }
  public FlatDropRequest __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }

  public long id() { int o = __offset(4); return o != 0 ? bb.getLong(o + bb_pos) : 0L; }

  public static int createFlatDropRequest(FlatBufferBuilder builder,
      long id) {
    builder.startTable(1);
    FlatDropRequest.addId(builder, id);
    return FlatDropRequest.endFlatDropRequest(builder);
  }

  public static void startFlatDropRequest(FlatBufferBuilder builder) { builder.startTable(1); }
  public static void addId(FlatBufferBuilder builder, long id) { builder.addLong(0, id, 0L); }
  public static int endFlatDropRequest(FlatBufferBuilder builder) {
    int o = builder.endTable();
    return o;
  }

  public static final class Vector extends BaseVector {
    public Vector __assign(int _vector, int _element_size, ByteBuffer _bb) { __reset(_vector, _element_size, _bb); return this; }

    public FlatDropRequest get(int j) { return get(new FlatDropRequest(), j); }
    public FlatDropRequest get(FlatDropRequest obj, int j) {  return obj.__assign(__indirect(__element(j), bb), bb); }
  }
}

