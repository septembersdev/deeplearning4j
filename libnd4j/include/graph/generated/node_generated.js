// automatically generated by the FlatBuffers compiler, do not modify

/**
 * @const
 * @namespace
 */
var sd = sd || {};

/**
 * @const
 * @namespace
 */
sd.graph = sd.graph || {};

/**
 * @constructor
 */
sd.graph.FlatNode = function() {
  /**
   * @type {flatbuffers.ByteBuffer}
   */
  this.bb = null;

  /**
   * @type {number}
   */
  this.bb_pos = 0;
};

/**
 * @param {number} i
 * @param {flatbuffers.ByteBuffer} bb
 * @returns {sd.graph.FlatNode}
 */
sd.graph.FlatNode.prototype.__init = function(i, bb) {
  this.bb_pos = i;
  this.bb = bb;
  return this;
};

/**
 * @param {flatbuffers.ByteBuffer} bb
 * @param {sd.graph.FlatNode=} obj
 * @returns {sd.graph.FlatNode}
 */
sd.graph.FlatNode.getRootAsFlatNode = function(bb, obj) {
  return (obj || new sd.graph.FlatNode).__init(bb.readInt32(bb.position()) + bb.position(), bb);
};

/**
 * @param {flatbuffers.ByteBuffer} bb
 * @param {sd.graph.FlatNode=} obj
 * @returns {sd.graph.FlatNode}
 */
sd.graph.FlatNode.getSizePrefixedRootAsFlatNode = function(bb, obj) {
  bb.setPosition(bb.position() + flatbuffers.SIZE_PREFIX_LENGTH);
  return (obj || new sd.graph.FlatNode).__init(bb.readInt32(bb.position()) + bb.position(), bb);
};

/**
 * @returns {number}
 */
sd.graph.FlatNode.prototype.id = function() {
  var offset = this.bb.__offset(this.bb_pos, 4);
  return offset ? this.bb.readInt32(this.bb_pos + offset) : 0;
};

/**
 * @param {flatbuffers.Encoding=} optionalEncoding
 * @returns {string|Uint8Array|null}
 */
sd.graph.FlatNode.prototype.name = function(optionalEncoding) {
  var offset = this.bb.__offset(this.bb_pos, 6);
  return offset ? this.bb.__string(this.bb_pos + offset, optionalEncoding) : null;
};

/**
 * @returns {sd.graph.OpType}
 */
sd.graph.FlatNode.prototype.opType = function() {
  var offset = this.bb.__offset(this.bb_pos, 8);
  return offset ? /** @type {sd.graph.OpType} */ (this.bb.readInt8(this.bb_pos + offset)) : sd.graph.OpType.TRANSFORM_FLOAT;
};

/**
 * @returns {flatbuffers.Long}
 */
sd.graph.FlatNode.prototype.opNum = function() {
  var offset = this.bb.__offset(this.bb_pos, 10);
  return offset ? this.bb.readInt64(this.bb_pos + offset) : this.bb.createLong(0, 0);
};

/**
 * @param {number} index
 * @param {sd.graph.FlatProperties=} obj
 * @returns {sd.graph.FlatProperties}
 */
sd.graph.FlatNode.prototype.properties = function(index, obj) {
  var offset = this.bb.__offset(this.bb_pos, 12);
  return offset ? (obj || new sd.graph.FlatProperties).__init(this.bb.__indirect(this.bb.__vector(this.bb_pos + offset) + index * 4), this.bb) : null;
};

/**
 * @returns {number}
 */
sd.graph.FlatNode.prototype.propertiesLength = function() {
  var offset = this.bb.__offset(this.bb_pos, 12);
  return offset ? this.bb.__vector_len(this.bb_pos + offset) : 0;
};

/**
 * @param {number} index
 * @returns {number}
 */
sd.graph.FlatNode.prototype.input = function(index) {
  var offset = this.bb.__offset(this.bb_pos, 14);
  return offset ? this.bb.readInt32(this.bb.__vector(this.bb_pos + offset) + index * 4) : 0;
};

/**
 * @returns {number}
 */
sd.graph.FlatNode.prototype.inputLength = function() {
  var offset = this.bb.__offset(this.bb_pos, 14);
  return offset ? this.bb.__vector_len(this.bb_pos + offset) : 0;
};

/**
 * @returns {Int32Array}
 */
sd.graph.FlatNode.prototype.inputArray = function() {
  var offset = this.bb.__offset(this.bb_pos, 14);
  return offset ? new Int32Array(this.bb.bytes().buffer, this.bb.bytes().byteOffset + this.bb.__vector(this.bb_pos + offset), this.bb.__vector_len(this.bb_pos + offset)) : null;
};

/**
 * @param {number} index
 * @param {sd.graph.IntPair=} obj
 * @returns {sd.graph.IntPair}
 */
sd.graph.FlatNode.prototype.inputPaired = function(index, obj) {
  var offset = this.bb.__offset(this.bb_pos, 16);
  return offset ? (obj || new sd.graph.IntPair).__init(this.bb.__indirect(this.bb.__vector(this.bb_pos + offset) + index * 4), this.bb) : null;
};

/**
 * @returns {number}
 */
sd.graph.FlatNode.prototype.inputPairedLength = function() {
  var offset = this.bb.__offset(this.bb_pos, 16);
  return offset ? this.bb.__vector_len(this.bb_pos + offset) : 0;
};

/**
 * @param {number} index
 * @returns {number}
 */
sd.graph.FlatNode.prototype.output = function(index) {
  var offset = this.bb.__offset(this.bb_pos, 18);
  return offset ? this.bb.readInt32(this.bb.__vector(this.bb_pos + offset) + index * 4) : 0;
};

/**
 * @returns {number}
 */
sd.graph.FlatNode.prototype.outputLength = function() {
  var offset = this.bb.__offset(this.bb_pos, 18);
  return offset ? this.bb.__vector_len(this.bb_pos + offset) : 0;
};

/**
 * @returns {Int32Array}
 */
sd.graph.FlatNode.prototype.outputArray = function() {
  var offset = this.bb.__offset(this.bb_pos, 18);
  return offset ? new Int32Array(this.bb.bytes().buffer, this.bb.bytes().byteOffset + this.bb.__vector(this.bb_pos + offset), this.bb.__vector_len(this.bb_pos + offset)) : null;
};

/**
 * @param {number} index
 * @returns {number}
 */
sd.graph.FlatNode.prototype.extraParams = function(index) {
  var offset = this.bb.__offset(this.bb_pos, 20);
  return offset ? this.bb.readFloat64(this.bb.__vector(this.bb_pos + offset) + index * 8) : 0;
};

/**
 * @returns {number}
 */
sd.graph.FlatNode.prototype.extraParamsLength = function() {
  var offset = this.bb.__offset(this.bb_pos, 20);
  return offset ? this.bb.__vector_len(this.bb_pos + offset) : 0;
};

/**
 * @returns {Float64Array}
 */
sd.graph.FlatNode.prototype.extraParamsArray = function() {
  var offset = this.bb.__offset(this.bb_pos, 20);
  return offset ? new Float64Array(this.bb.bytes().buffer, this.bb.bytes().byteOffset + this.bb.__vector(this.bb_pos + offset), this.bb.__vector_len(this.bb_pos + offset)) : null;
};

/**
 * @param {number} index
 * @returns {flatbuffers.Long}
 */
sd.graph.FlatNode.prototype.extraInteger = function(index) {
  var offset = this.bb.__offset(this.bb_pos, 22);
  return offset ? this.bb.readInt64(this.bb.__vector(this.bb_pos + offset) + index * 8) : this.bb.createLong(0, 0);
};

/**
 * @returns {number}
 */
sd.graph.FlatNode.prototype.extraIntegerLength = function() {
  var offset = this.bb.__offset(this.bb_pos, 22);
  return offset ? this.bb.__vector_len(this.bb_pos + offset) : 0;
};

/**
 * @param {number} index
 * @returns {boolean}
 */
sd.graph.FlatNode.prototype.extraBools = function(index) {
  var offset = this.bb.__offset(this.bb_pos, 24);
  return offset ? !!this.bb.readInt8(this.bb.__vector(this.bb_pos + offset) + index) : false;
};

/**
 * @returns {number}
 */
sd.graph.FlatNode.prototype.extraBoolsLength = function() {
  var offset = this.bb.__offset(this.bb_pos, 24);
  return offset ? this.bb.__vector_len(this.bb_pos + offset) : 0;
};

/**
 * @returns {Int8Array}
 */
sd.graph.FlatNode.prototype.extraBoolsArray = function() {
  var offset = this.bb.__offset(this.bb_pos, 24);
  return offset ? new Int8Array(this.bb.bytes().buffer, this.bb.bytes().byteOffset + this.bb.__vector(this.bb_pos + offset), this.bb.__vector_len(this.bb_pos + offset)) : null;
};

/**
 * @param {number} index
 * @returns {number}
 */
sd.graph.FlatNode.prototype.dimensions = function(index) {
  var offset = this.bb.__offset(this.bb_pos, 26);
  return offset ? this.bb.readInt32(this.bb.__vector(this.bb_pos + offset) + index * 4) : 0;
};

/**
 * @returns {number}
 */
sd.graph.FlatNode.prototype.dimensionsLength = function() {
  var offset = this.bb.__offset(this.bb_pos, 26);
  return offset ? this.bb.__vector_len(this.bb_pos + offset) : 0;
};

/**
 * @returns {Int32Array}
 */
sd.graph.FlatNode.prototype.dimensionsArray = function() {
  var offset = this.bb.__offset(this.bb_pos, 26);
  return offset ? new Int32Array(this.bb.bytes().buffer, this.bb.bytes().byteOffset + this.bb.__vector(this.bb_pos + offset), this.bb.__vector_len(this.bb_pos + offset)) : null;
};

/**
 * @returns {number}
 */
sd.graph.FlatNode.prototype.device = function() {
  var offset = this.bb.__offset(this.bb_pos, 28);
  return offset ? this.bb.readInt32(this.bb_pos + offset) : 0;
};

/**
 * @returns {number}
 */
sd.graph.FlatNode.prototype.scopeId = function() {
  var offset = this.bb.__offset(this.bb_pos, 30);
  return offset ? this.bb.readInt32(this.bb_pos + offset) : 0;
};

/**
 * @param {flatbuffers.Encoding=} optionalEncoding
 * @returns {string|Uint8Array|null}
 */
sd.graph.FlatNode.prototype.scopeName = function(optionalEncoding) {
  var offset = this.bb.__offset(this.bb_pos, 32);
  return offset ? this.bb.__string(this.bb_pos + offset, optionalEncoding) : null;
};

/**
 * @param {number} index
 * @param {flatbuffers.Encoding=} optionalEncoding
 * @returns {string|Uint8Array}
 */
sd.graph.FlatNode.prototype.outputNames = function(index, optionalEncoding) {
  var offset = this.bb.__offset(this.bb_pos, 34);
  return offset ? this.bb.__string(this.bb.__vector(this.bb_pos + offset) + index * 4, optionalEncoding) : null;
};

/**
 * @returns {number}
 */
sd.graph.FlatNode.prototype.outputNamesLength = function() {
  var offset = this.bb.__offset(this.bb_pos, 34);
  return offset ? this.bb.__vector_len(this.bb_pos + offset) : 0;
};

/**
 * @param {flatbuffers.Encoding=} optionalEncoding
 * @returns {string|Uint8Array|null}
 */
sd.graph.FlatNode.prototype.opName = function(optionalEncoding) {
  var offset = this.bb.__offset(this.bb_pos, 36);
  return offset ? this.bb.__string(this.bb_pos + offset, optionalEncoding) : null;
};

/**
 * @param {number} index
 * @returns {sd.graph.DType}
 */
sd.graph.FlatNode.prototype.outputTypes = function(index) {
  var offset = this.bb.__offset(this.bb_pos, 38);
  return offset ? /** @type {sd.graph.DType} */ (this.bb.readInt8(this.bb.__vector(this.bb_pos + offset) + index)) : /** @type {sd.graph.DType} */ (0);
};

/**
 * @returns {number}
 */
sd.graph.FlatNode.prototype.outputTypesLength = function() {
  var offset = this.bb.__offset(this.bb_pos, 38);
  return offset ? this.bb.__vector_len(this.bb_pos + offset) : 0;
};

/**
 * @returns {Int8Array}
 */
sd.graph.FlatNode.prototype.outputTypesArray = function() {
  var offset = this.bb.__offset(this.bb_pos, 38);
  return offset ? new Int8Array(this.bb.bytes().buffer, this.bb.bytes().byteOffset + this.bb.__vector(this.bb_pos + offset), this.bb.__vector_len(this.bb_pos + offset)) : null;
};

/**
 * @param {sd.graph.FlatArray=} obj
 * @returns {sd.graph.FlatArray|null}
 */
sd.graph.FlatNode.prototype.scalar = function(obj) {
  var offset = this.bb.__offset(this.bb_pos, 40);
  return offset ? (obj || new sd.graph.FlatArray).__init(this.bb.__indirect(this.bb_pos + offset), this.bb) : null;
};

/**
 * @param {number} index
 * @param {flatbuffers.Encoding=} optionalEncoding
 * @returns {string|Uint8Array}
 */
sd.graph.FlatNode.prototype.controlDeps = function(index, optionalEncoding) {
  var offset = this.bb.__offset(this.bb_pos, 42);
  return offset ? this.bb.__string(this.bb.__vector(this.bb_pos + offset) + index * 4, optionalEncoding) : null;
};

/**
 * @returns {number}
 */
sd.graph.FlatNode.prototype.controlDepsLength = function() {
  var offset = this.bb.__offset(this.bb_pos, 42);
  return offset ? this.bb.__vector_len(this.bb_pos + offset) : 0;
};

/**
 * @param {number} index
 * @param {flatbuffers.Encoding=} optionalEncoding
 * @returns {string|Uint8Array}
 */
sd.graph.FlatNode.prototype.varControlDeps = function(index, optionalEncoding) {
  var offset = this.bb.__offset(this.bb_pos, 44);
  return offset ? this.bb.__string(this.bb.__vector(this.bb_pos + offset) + index * 4, optionalEncoding) : null;
};

/**
 * @returns {number}
 */
sd.graph.FlatNode.prototype.varControlDepsLength = function() {
  var offset = this.bb.__offset(this.bb_pos, 44);
  return offset ? this.bb.__vector_len(this.bb_pos + offset) : 0;
};

/**
 * @param {number} index
 * @param {flatbuffers.Encoding=} optionalEncoding
 * @returns {string|Uint8Array}
 */
sd.graph.FlatNode.prototype.controlDepFor = function(index, optionalEncoding) {
  var offset = this.bb.__offset(this.bb_pos, 46);
  return offset ? this.bb.__string(this.bb.__vector(this.bb_pos + offset) + index * 4, optionalEncoding) : null;
};

/**
 * @returns {number}
 */
sd.graph.FlatNode.prototype.controlDepForLength = function() {
  var offset = this.bb.__offset(this.bb_pos, 46);
  return offset ? this.bb.__vector_len(this.bb_pos + offset) : 0;
};

/**
 * @param {number} index
 * @returns {sd.graph.DType}
 */
sd.graph.FlatNode.prototype.extraTypes = function(index) {
  var offset = this.bb.__offset(this.bb_pos, 48);
  return offset ? /** @type {sd.graph.DType} */ (this.bb.readInt8(this.bb.__vector(this.bb_pos + offset) + index)) : /** @type {sd.graph.DType} */ (0);
};

/**
 * @returns {number}
 */
sd.graph.FlatNode.prototype.extraTypesLength = function() {
  var offset = this.bb.__offset(this.bb_pos, 48);
  return offset ? this.bb.__vector_len(this.bb_pos + offset) : 0;
};

/**
 * @returns {Int8Array}
 */
sd.graph.FlatNode.prototype.extraTypesArray = function() {
  var offset = this.bb.__offset(this.bb_pos, 48);
  return offset ? new Int8Array(this.bb.bytes().buffer, this.bb.bytes().byteOffset + this.bb.__vector(this.bb_pos + offset), this.bb.__vector_len(this.bb_pos + offset)) : null;
};

/**
 * @param {number} index
 * @param {flatbuffers.Encoding=} optionalEncoding
 * @returns {string|Uint8Array}
 */
sd.graph.FlatNode.prototype.extraStrings = function(index, optionalEncoding) {
  var offset = this.bb.__offset(this.bb_pos, 50);
  return offset ? this.bb.__string(this.bb.__vector(this.bb_pos + offset) + index * 4, optionalEncoding) : null;
};

/**
 * @returns {number}
 */
sd.graph.FlatNode.prototype.extraStringsLength = function() {
  var offset = this.bb.__offset(this.bb_pos, 50);
  return offset ? this.bb.__vector_len(this.bb_pos + offset) : 0;
};

/**
 * @param {flatbuffers.Builder} builder
 */
sd.graph.FlatNode.startFlatNode = function(builder) {
  builder.startObject(24);
};

/**
 * @param {flatbuffers.Builder} builder
 * @param {number} id
 */
sd.graph.FlatNode.addId = function(builder, id) {
  builder.addFieldInt32(0, id, 0);
};

/**
 * @param {flatbuffers.Builder} builder
 * @param {flatbuffers.Offset} nameOffset
 */
sd.graph.FlatNode.addName = function(builder, nameOffset) {
  builder.addFieldOffset(1, nameOffset, 0);
};

/**
 * @param {flatbuffers.Builder} builder
 * @param {sd.graph.OpType} opType
 */
sd.graph.FlatNode.addOpType = function(builder, opType) {
  builder.addFieldInt8(2, opType, sd.graph.OpType.TRANSFORM_FLOAT);
};

/**
 * @param {flatbuffers.Builder} builder
 * @param {flatbuffers.Long} opNum
 */
sd.graph.FlatNode.addOpNum = function(builder, opNum) {
  builder.addFieldInt64(3, opNum, builder.createLong(0, 0));
};

/**
 * @param {flatbuffers.Builder} builder
 * @param {flatbuffers.Offset} propertiesOffset
 */
sd.graph.FlatNode.addProperties = function(builder, propertiesOffset) {
  builder.addFieldOffset(4, propertiesOffset, 0);
};

/**
 * @param {flatbuffers.Builder} builder
 * @param {Array.<flatbuffers.Offset>} data
 * @returns {flatbuffers.Offset}
 */
sd.graph.FlatNode.createPropertiesVector = function(builder, data) {
  builder.startVector(4, data.length, 4);
  for (var i = data.length - 1; i >= 0; i--) {
    builder.addOffset(data[i]);
  }
  return builder.endVector();
};

/**
 * @param {flatbuffers.Builder} builder
 * @param {number} numElems
 */
sd.graph.FlatNode.startPropertiesVector = function(builder, numElems) {
  builder.startVector(4, numElems, 4);
};

/**
 * @param {flatbuffers.Builder} builder
 * @param {flatbuffers.Offset} inputOffset
 */
sd.graph.FlatNode.addInput = function(builder, inputOffset) {
  builder.addFieldOffset(5, inputOffset, 0);
};

/**
 * @param {flatbuffers.Builder} builder
 * @param {Array.<number>} data
 * @returns {flatbuffers.Offset}
 */
sd.graph.FlatNode.createInputVector = function(builder, data) {
  builder.startVector(4, data.length, 4);
  for (var i = data.length - 1; i >= 0; i--) {
    builder.addInt32(data[i]);
  }
  return builder.endVector();
};

/**
 * @param {flatbuffers.Builder} builder
 * @param {number} numElems
 */
sd.graph.FlatNode.startInputVector = function(builder, numElems) {
  builder.startVector(4, numElems, 4);
};

/**
 * @param {flatbuffers.Builder} builder
 * @param {flatbuffers.Offset} inputPairedOffset
 */
sd.graph.FlatNode.addInputPaired = function(builder, inputPairedOffset) {
  builder.addFieldOffset(6, inputPairedOffset, 0);
};

/**
 * @param {flatbuffers.Builder} builder
 * @param {Array.<flatbuffers.Offset>} data
 * @returns {flatbuffers.Offset}
 */
sd.graph.FlatNode.createInputPairedVector = function(builder, data) {
  builder.startVector(4, data.length, 4);
  for (var i = data.length - 1; i >= 0; i--) {
    builder.addOffset(data[i]);
  }
  return builder.endVector();
};

/**
 * @param {flatbuffers.Builder} builder
 * @param {number} numElems
 */
sd.graph.FlatNode.startInputPairedVector = function(builder, numElems) {
  builder.startVector(4, numElems, 4);
};

/**
 * @param {flatbuffers.Builder} builder
 * @param {flatbuffers.Offset} outputOffset
 */
sd.graph.FlatNode.addOutput = function(builder, outputOffset) {
  builder.addFieldOffset(7, outputOffset, 0);
};

/**
 * @param {flatbuffers.Builder} builder
 * @param {Array.<number>} data
 * @returns {flatbuffers.Offset}
 */
sd.graph.FlatNode.createOutputVector = function(builder, data) {
  builder.startVector(4, data.length, 4);
  for (var i = data.length - 1; i >= 0; i--) {
    builder.addInt32(data[i]);
  }
  return builder.endVector();
};

/**
 * @param {flatbuffers.Builder} builder
 * @param {number} numElems
 */
sd.graph.FlatNode.startOutputVector = function(builder, numElems) {
  builder.startVector(4, numElems, 4);
};

/**
 * @param {flatbuffers.Builder} builder
 * @param {flatbuffers.Offset} extraParamsOffset
 */
sd.graph.FlatNode.addExtraParams = function(builder, extraParamsOffset) {
  builder.addFieldOffset(8, extraParamsOffset, 0);
};

/**
 * @param {flatbuffers.Builder} builder
 * @param {Array.<number>} data
 * @returns {flatbuffers.Offset}
 */
sd.graph.FlatNode.createExtraParamsVector = function(builder, data) {
  builder.startVector(8, data.length, 8);
  for (var i = data.length - 1; i >= 0; i--) {
    builder.addFloat64(data[i]);
  }
  return builder.endVector();
};

/**
 * @param {flatbuffers.Builder} builder
 * @param {number} numElems
 */
sd.graph.FlatNode.startExtraParamsVector = function(builder, numElems) {
  builder.startVector(8, numElems, 8);
};

/**
 * @param {flatbuffers.Builder} builder
 * @param {flatbuffers.Offset} extraIntegerOffset
 */
sd.graph.FlatNode.addExtraInteger = function(builder, extraIntegerOffset) {
  builder.addFieldOffset(9, extraIntegerOffset, 0);
};

/**
 * @param {flatbuffers.Builder} builder
 * @param {Array.<flatbuffers.Long>} data
 * @returns {flatbuffers.Offset}
 */
sd.graph.FlatNode.createExtraIntegerVector = function(builder, data) {
  builder.startVector(8, data.length, 8);
  for (var i = data.length - 1; i >= 0; i--) {
    builder.addInt64(data[i]);
  }
  return builder.endVector();
};

/**
 * @param {flatbuffers.Builder} builder
 * @param {number} numElems
 */
sd.graph.FlatNode.startExtraIntegerVector = function(builder, numElems) {
  builder.startVector(8, numElems, 8);
};

/**
 * @param {flatbuffers.Builder} builder
 * @param {flatbuffers.Offset} extraBoolsOffset
 */
sd.graph.FlatNode.addExtraBools = function(builder, extraBoolsOffset) {
  builder.addFieldOffset(10, extraBoolsOffset, 0);
};

/**
 * @param {flatbuffers.Builder} builder
 * @param {Array.<boolean>} data
 * @returns {flatbuffers.Offset}
 */
sd.graph.FlatNode.createExtraBoolsVector = function(builder, data) {
  builder.startVector(1, data.length, 1);
  for (var i = data.length - 1; i >= 0; i--) {
    builder.addInt8(+data[i]);
  }
  return builder.endVector();
};

/**
 * @param {flatbuffers.Builder} builder
 * @param {number} numElems
 */
sd.graph.FlatNode.startExtraBoolsVector = function(builder, numElems) {
  builder.startVector(1, numElems, 1);
};

/**
 * @param {flatbuffers.Builder} builder
 * @param {flatbuffers.Offset} dimensionsOffset
 */
sd.graph.FlatNode.addDimensions = function(builder, dimensionsOffset) {
  builder.addFieldOffset(11, dimensionsOffset, 0);
};

/**
 * @param {flatbuffers.Builder} builder
 * @param {Array.<number>} data
 * @returns {flatbuffers.Offset}
 */
sd.graph.FlatNode.createDimensionsVector = function(builder, data) {
  builder.startVector(4, data.length, 4);
  for (var i = data.length - 1; i >= 0; i--) {
    builder.addInt32(data[i]);
  }
  return builder.endVector();
};

/**
 * @param {flatbuffers.Builder} builder
 * @param {number} numElems
 */
sd.graph.FlatNode.startDimensionsVector = function(builder, numElems) {
  builder.startVector(4, numElems, 4);
};

/**
 * @param {flatbuffers.Builder} builder
 * @param {number} device
 */
sd.graph.FlatNode.addDevice = function(builder, device) {
  builder.addFieldInt32(12, device, 0);
};

/**
 * @param {flatbuffers.Builder} builder
 * @param {number} scopeId
 */
sd.graph.FlatNode.addScopeId = function(builder, scopeId) {
  builder.addFieldInt32(13, scopeId, 0);
};

/**
 * @param {flatbuffers.Builder} builder
 * @param {flatbuffers.Offset} scopeNameOffset
 */
sd.graph.FlatNode.addScopeName = function(builder, scopeNameOffset) {
  builder.addFieldOffset(14, scopeNameOffset, 0);
};

/**
 * @param {flatbuffers.Builder} builder
 * @param {flatbuffers.Offset} outputNamesOffset
 */
sd.graph.FlatNode.addOutputNames = function(builder, outputNamesOffset) {
  builder.addFieldOffset(15, outputNamesOffset, 0);
};

/**
 * @param {flatbuffers.Builder} builder
 * @param {Array.<flatbuffers.Offset>} data
 * @returns {flatbuffers.Offset}
 */
sd.graph.FlatNode.createOutputNamesVector = function(builder, data) {
  builder.startVector(4, data.length, 4);
  for (var i = data.length - 1; i >= 0; i--) {
    builder.addOffset(data[i]);
  }
  return builder.endVector();
};

/**
 * @param {flatbuffers.Builder} builder
 * @param {number} numElems
 */
sd.graph.FlatNode.startOutputNamesVector = function(builder, numElems) {
  builder.startVector(4, numElems, 4);
};

/**
 * @param {flatbuffers.Builder} builder
 * @param {flatbuffers.Offset} opNameOffset
 */
sd.graph.FlatNode.addOpName = function(builder, opNameOffset) {
  builder.addFieldOffset(16, opNameOffset, 0);
};

/**
 * @param {flatbuffers.Builder} builder
 * @param {flatbuffers.Offset} outputTypesOffset
 */
sd.graph.FlatNode.addOutputTypes = function(builder, outputTypesOffset) {
  builder.addFieldOffset(17, outputTypesOffset, 0);
};

/**
 * @param {flatbuffers.Builder} builder
 * @param {Array.<sd.graph.DType>} data
 * @returns {flatbuffers.Offset}
 */
sd.graph.FlatNode.createOutputTypesVector = function(builder, data) {
  builder.startVector(1, data.length, 1);
  for (var i = data.length - 1; i >= 0; i--) {
    builder.addInt8(data[i]);
  }
  return builder.endVector();
};

/**
 * @param {flatbuffers.Builder} builder
 * @param {number} numElems
 */
sd.graph.FlatNode.startOutputTypesVector = function(builder, numElems) {
  builder.startVector(1, numElems, 1);
};

/**
 * @param {flatbuffers.Builder} builder
 * @param {flatbuffers.Offset} scalarOffset
 */
sd.graph.FlatNode.addScalar = function(builder, scalarOffset) {
  builder.addFieldOffset(18, scalarOffset, 0);
};

/**
 * @param {flatbuffers.Builder} builder
 * @param {flatbuffers.Offset} controlDepsOffset
 */
sd.graph.FlatNode.addControlDeps = function(builder, controlDepsOffset) {
  builder.addFieldOffset(19, controlDepsOffset, 0);
};

/**
 * @param {flatbuffers.Builder} builder
 * @param {Array.<flatbuffers.Offset>} data
 * @returns {flatbuffers.Offset}
 */
sd.graph.FlatNode.createControlDepsVector = function(builder, data) {
  builder.startVector(4, data.length, 4);
  for (var i = data.length - 1; i >= 0; i--) {
    builder.addOffset(data[i]);
  }
  return builder.endVector();
};

/**
 * @param {flatbuffers.Builder} builder
 * @param {number} numElems
 */
sd.graph.FlatNode.startControlDepsVector = function(builder, numElems) {
  builder.startVector(4, numElems, 4);
};

/**
 * @param {flatbuffers.Builder} builder
 * @param {flatbuffers.Offset} varControlDepsOffset
 */
sd.graph.FlatNode.addVarControlDeps = function(builder, varControlDepsOffset) {
  builder.addFieldOffset(20, varControlDepsOffset, 0);
};

/**
 * @param {flatbuffers.Builder} builder
 * @param {Array.<flatbuffers.Offset>} data
 * @returns {flatbuffers.Offset}
 */
sd.graph.FlatNode.createVarControlDepsVector = function(builder, data) {
  builder.startVector(4, data.length, 4);
  for (var i = data.length - 1; i >= 0; i--) {
    builder.addOffset(data[i]);
  }
  return builder.endVector();
};

/**
 * @param {flatbuffers.Builder} builder
 * @param {number} numElems
 */
sd.graph.FlatNode.startVarControlDepsVector = function(builder, numElems) {
  builder.startVector(4, numElems, 4);
};

/**
 * @param {flatbuffers.Builder} builder
 * @param {flatbuffers.Offset} controlDepForOffset
 */
sd.graph.FlatNode.addControlDepFor = function(builder, controlDepForOffset) {
  builder.addFieldOffset(21, controlDepForOffset, 0);
};

/**
 * @param {flatbuffers.Builder} builder
 * @param {Array.<flatbuffers.Offset>} data
 * @returns {flatbuffers.Offset}
 */
sd.graph.FlatNode.createControlDepForVector = function(builder, data) {
  builder.startVector(4, data.length, 4);
  for (var i = data.length - 1; i >= 0; i--) {
    builder.addOffset(data[i]);
  }
  return builder.endVector();
};

/**
 * @param {flatbuffers.Builder} builder
 * @param {number} numElems
 */
sd.graph.FlatNode.startControlDepForVector = function(builder, numElems) {
  builder.startVector(4, numElems, 4);
};

/**
 * @param {flatbuffers.Builder} builder
 * @param {flatbuffers.Offset} extraTypesOffset
 */
sd.graph.FlatNode.addExtraTypes = function(builder, extraTypesOffset) {
  builder.addFieldOffset(22, extraTypesOffset, 0);
};

/**
 * @param {flatbuffers.Builder} builder
 * @param {Array.<sd.graph.DType>} data
 * @returns {flatbuffers.Offset}
 */
sd.graph.FlatNode.createExtraTypesVector = function(builder, data) {
  builder.startVector(1, data.length, 1);
  for (var i = data.length - 1; i >= 0; i--) {
    builder.addInt8(data[i]);
  }
  return builder.endVector();
};

/**
 * @param {flatbuffers.Builder} builder
 * @param {number} numElems
 */
sd.graph.FlatNode.startExtraTypesVector = function(builder, numElems) {
  builder.startVector(1, numElems, 1);
};

/**
 * @param {flatbuffers.Builder} builder
 * @param {flatbuffers.Offset} extraStringsOffset
 */
sd.graph.FlatNode.addExtraStrings = function(builder, extraStringsOffset) {
  builder.addFieldOffset(23, extraStringsOffset, 0);
};

/**
 * @param {flatbuffers.Builder} builder
 * @param {Array.<flatbuffers.Offset>} data
 * @returns {flatbuffers.Offset}
 */
sd.graph.FlatNode.createExtraStringsVector = function(builder, data) {
  builder.startVector(4, data.length, 4);
  for (var i = data.length - 1; i >= 0; i--) {
    builder.addOffset(data[i]);
  }
  return builder.endVector();
};

/**
 * @param {flatbuffers.Builder} builder
 * @param {number} numElems
 */
sd.graph.FlatNode.startExtraStringsVector = function(builder, numElems) {
  builder.startVector(4, numElems, 4);
};

/**
 * @param {flatbuffers.Builder} builder
 * @returns {flatbuffers.Offset}
 */
sd.graph.FlatNode.endFlatNode = function(builder) {
  var offset = builder.endObject();
  return offset;
};

/**
 * @param {flatbuffers.Builder} builder
 * @param {flatbuffers.Offset} offset
 */
sd.graph.FlatNode.finishFlatNodeBuffer = function(builder, offset) {
  builder.finish(offset);
};

/**
 * @param {flatbuffers.Builder} builder
 * @param {flatbuffers.Offset} offset
 */
sd.graph.FlatNode.finishSizePrefixedFlatNodeBuffer = function(builder, offset) {
  builder.finish(offset, undefined, true);
};

/**
 * @param {flatbuffers.Builder} builder
 * @param {number} id
 * @param {flatbuffers.Offset} nameOffset
 * @param {NS10750192814939967615.sd.graph.OpType} opType
 * @param {flatbuffers.Long} opNum
 * @param {flatbuffers.Offset} propertiesOffset
 * @param {flatbuffers.Offset} inputOffset
 * @param {flatbuffers.Offset} inputPairedOffset
 * @param {flatbuffers.Offset} outputOffset
 * @param {flatbuffers.Offset} extraParamsOffset
 * @param {flatbuffers.Offset} extraIntegerOffset
 * @param {flatbuffers.Offset} extraBoolsOffset
 * @param {flatbuffers.Offset} dimensionsOffset
 * @param {number} device
 * @param {number} scopeId
 * @param {flatbuffers.Offset} scopeNameOffset
 * @param {flatbuffers.Offset} outputNamesOffset
 * @param {flatbuffers.Offset} opNameOffset
 * @param {NS2914996894544545883.flatbuffers.Offset} outputTypesOffset
 * @param {flatbuffers.Offset} scalarOffset
 * @param {flatbuffers.Offset} controlDepsOffset
 * @param {flatbuffers.Offset} varControlDepsOffset
 * @param {flatbuffers.Offset} controlDepForOffset
 * @param {NS2914996894544545883.flatbuffers.Offset} extraTypesOffset
 * @param {flatbuffers.Offset} extraStringsOffset
 * @returns {flatbuffers.Offset}
 */
sd.graph.FlatNode.createFlatNode = function(builder, id, nameOffset, opType, opNum, propertiesOffset, inputOffset, inputPairedOffset, outputOffset, extraParamsOffset, extraIntegerOffset, extraBoolsOffset, dimensionsOffset, device, scopeId, scopeNameOffset, outputNamesOffset, opNameOffset, outputTypesOffset, scalarOffset, controlDepsOffset, varControlDepsOffset, controlDepForOffset, extraTypesOffset, extraStringsOffset) {
  sd.graph.FlatNode.startFlatNode(builder);
  sd.graph.FlatNode.addId(builder, id);
  sd.graph.FlatNode.addName(builder, nameOffset);
  sd.graph.FlatNode.addOpType(builder, opType);
  sd.graph.FlatNode.addOpNum(builder, opNum);
  sd.graph.FlatNode.addProperties(builder, propertiesOffset);
  sd.graph.FlatNode.addInput(builder, inputOffset);
  sd.graph.FlatNode.addInputPaired(builder, inputPairedOffset);
  sd.graph.FlatNode.addOutput(builder, outputOffset);
  sd.graph.FlatNode.addExtraParams(builder, extraParamsOffset);
  sd.graph.FlatNode.addExtraInteger(builder, extraIntegerOffset);
  sd.graph.FlatNode.addExtraBools(builder, extraBoolsOffset);
  sd.graph.FlatNode.addDimensions(builder, dimensionsOffset);
  sd.graph.FlatNode.addDevice(builder, device);
  sd.graph.FlatNode.addScopeId(builder, scopeId);
  sd.graph.FlatNode.addScopeName(builder, scopeNameOffset);
  sd.graph.FlatNode.addOutputNames(builder, outputNamesOffset);
  sd.graph.FlatNode.addOpName(builder, opNameOffset);
  sd.graph.FlatNode.addOutputTypes(builder, outputTypesOffset);
  sd.graph.FlatNode.addScalar(builder, scalarOffset);
  sd.graph.FlatNode.addControlDeps(builder, controlDepsOffset);
  sd.graph.FlatNode.addVarControlDeps(builder, varControlDepsOffset);
  sd.graph.FlatNode.addControlDepFor(builder, controlDepForOffset);
  sd.graph.FlatNode.addExtraTypes(builder, extraTypesOffset);
  sd.graph.FlatNode.addExtraStrings(builder, extraStringsOffset);
  return sd.graph.FlatNode.endFlatNode(builder);
}

// Exports for Node.js and RequireJS
this.sd = sd;
