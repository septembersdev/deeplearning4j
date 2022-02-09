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


#ifndef FLATBUFFERS_GENERATED_CONFIG_SD_GRAPH_H_
#define FLATBUFFERS_GENERATED_CONFIG_SD_GRAPH_H_

#include "flatbuffers/flatbuffers.h"

namespace sd {
namespace graph {

struct FlatConfiguration;
struct FlatConfigurationBuilder;

enum ProfilingMode {
  ProfilingMode_NONE = 0,
  ProfilingMode_NAN_PANIC = 1,
  ProfilingMode_INF_PANIC = 2,
  ProfilingMode_ANY_PANIC = 3,
  ProfilingMode_MIN = ProfilingMode_NONE,
  ProfilingMode_MAX = ProfilingMode_ANY_PANIC
};

inline const ProfilingMode (&EnumValuesProfilingMode())[4] {
  static const ProfilingMode values[] = {
    ProfilingMode_NONE,
    ProfilingMode_NAN_PANIC,
    ProfilingMode_INF_PANIC,
    ProfilingMode_ANY_PANIC
  };
  return values;
}

inline const char * const *EnumNamesProfilingMode() {
  static const char * const names[5] = {
    "NONE",
    "NAN_PANIC",
    "INF_PANIC",
    "ANY_PANIC",
    nullptr
  };
  return names;
}

inline const char *EnumNameProfilingMode(ProfilingMode e) {
  if (flatbuffers::IsOutRange(e, ProfilingMode_NONE, ProfilingMode_ANY_PANIC)) return "";
  const size_t index = static_cast<size_t>(e);
  return EnumNamesProfilingMode()[index];
}

enum ExecutionMode {
  ExecutionMode_SEQUENTIAL = 0,
  ExecutionMode_STRICT = 1,
  ExecutionMode_AUTO = 2,
  ExecutionMode_MIN = ExecutionMode_SEQUENTIAL,
  ExecutionMode_MAX = ExecutionMode_AUTO
};

inline const ExecutionMode (&EnumValuesExecutionMode())[3] {
  static const ExecutionMode values[] = {
    ExecutionMode_SEQUENTIAL,
    ExecutionMode_STRICT,
    ExecutionMode_AUTO
  };
  return values;
}

inline const char * const *EnumNamesExecutionMode() {
  static const char * const names[4] = {
    "SEQUENTIAL",
    "STRICT",
    "AUTO",
    nullptr
  };
  return names;
}

inline const char *EnumNameExecutionMode(ExecutionMode e) {
  if (flatbuffers::IsOutRange(e, ExecutionMode_SEQUENTIAL, ExecutionMode_AUTO)) return "";
  const size_t index = static_cast<size_t>(e);
  return EnumNamesExecutionMode()[index];
}

enum OutputMode {
  OutputMode_IMPLICIT = 0,
  OutputMode_EXPLICIT = 1,
  OutputMode_EXPLICIT_AND_IMPLICIT = 2,
  OutputMode_VARIABLE_SPACE = 3,
  OutputMode_OPTIMIZED = 4,
  OutputMode_MIN = OutputMode_IMPLICIT,
  OutputMode_MAX = OutputMode_OPTIMIZED
};

inline const OutputMode (&EnumValuesOutputMode())[5] {
  static const OutputMode values[] = {
    OutputMode_IMPLICIT,
    OutputMode_EXPLICIT,
    OutputMode_EXPLICIT_AND_IMPLICIT,
    OutputMode_VARIABLE_SPACE,
    OutputMode_OPTIMIZED
  };
  return values;
}

inline const char * const *EnumNamesOutputMode() {
  static const char * const names[6] = {
    "IMPLICIT",
    "EXPLICIT",
    "EXPLICIT_AND_IMPLICIT",
    "VARIABLE_SPACE",
    "OPTIMIZED",
    nullptr
  };
  return names;
}

inline const char *EnumNameOutputMode(OutputMode e) {
  if (flatbuffers::IsOutRange(e, OutputMode_IMPLICIT, OutputMode_OPTIMIZED)) return "";
  const size_t index = static_cast<size_t>(e);
  return EnumNamesOutputMode()[index];
}

enum Direction {
  Direction_FORWARD_ONLY = 0,
  Direction_FORWARD_AND_BACKWARD = 1,
  Direction_BACKWARD_ONLY = 2,
  Direction_MIN = Direction_FORWARD_ONLY,
  Direction_MAX = Direction_BACKWARD_ONLY
};

inline const Direction (&EnumValuesDirection())[3] {
  static const Direction values[] = {
    Direction_FORWARD_ONLY,
    Direction_FORWARD_AND_BACKWARD,
    Direction_BACKWARD_ONLY
  };
  return values;
}

inline const char * const *EnumNamesDirection() {
  static const char * const names[4] = {
    "FORWARD_ONLY",
    "FORWARD_AND_BACKWARD",
    "BACKWARD_ONLY",
    nullptr
  };
  return names;
}

inline const char *EnumNameDirection(Direction e) {
  if (flatbuffers::IsOutRange(e, Direction_FORWARD_ONLY, Direction_BACKWARD_ONLY)) return "";
  const size_t index = static_cast<size_t>(e);
  return EnumNamesDirection()[index];
}

struct FlatConfiguration FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef FlatConfigurationBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_ID = 4,
    VT_EXECUTIONMODE = 6,
    VT_PROFILINGMODE = 8,
    VT_OUTPUTMODE = 10,
    VT_TIMESTATS = 12,
    VT_FOOTPRINTFORWARD = 14,
    VT_FOOTPRINTBACKWARD = 16,
    VT_DIRECTION = 18
  };
  int64_t id() const {
    return GetField<int64_t>(VT_ID, 0);
  }
  sd::graph::ExecutionMode executionMode() const {
    return static_cast<sd::graph::ExecutionMode>(GetField<int8_t>(VT_EXECUTIONMODE, 0));
  }
  sd::graph::ProfilingMode profilingMode() const {
    return static_cast<sd::graph::ProfilingMode>(GetField<int8_t>(VT_PROFILINGMODE, 0));
  }
  sd::graph::OutputMode outputMode() const {
    return static_cast<sd::graph::OutputMode>(GetField<int8_t>(VT_OUTPUTMODE, 0));
  }
  bool timestats() const {
    return GetField<uint8_t>(VT_TIMESTATS, 0) != 0;
  }
  int64_t footprintForward() const {
    return GetField<int64_t>(VT_FOOTPRINTFORWARD, 0);
  }
  int64_t footprintBackward() const {
    return GetField<int64_t>(VT_FOOTPRINTBACKWARD, 0);
  }
  sd::graph::Direction direction() const {
    return static_cast<sd::graph::Direction>(GetField<int8_t>(VT_DIRECTION, 0));
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<int64_t>(verifier, VT_ID) &&
           VerifyField<int8_t>(verifier, VT_EXECUTIONMODE) &&
           VerifyField<int8_t>(verifier, VT_PROFILINGMODE) &&
           VerifyField<int8_t>(verifier, VT_OUTPUTMODE) &&
           VerifyField<uint8_t>(verifier, VT_TIMESTATS) &&
           VerifyField<int64_t>(verifier, VT_FOOTPRINTFORWARD) &&
           VerifyField<int64_t>(verifier, VT_FOOTPRINTBACKWARD) &&
           VerifyField<int8_t>(verifier, VT_DIRECTION) &&
           verifier.EndTable();
  }
};

struct FlatConfigurationBuilder {
  typedef FlatConfiguration Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_id(int64_t id) {
    fbb_.AddElement<int64_t>(FlatConfiguration::VT_ID, id, 0);
  }
  void add_executionMode(sd::graph::ExecutionMode executionMode) {
    fbb_.AddElement<int8_t>(FlatConfiguration::VT_EXECUTIONMODE, static_cast<int8_t>(executionMode), 0);
  }
  void add_profilingMode(sd::graph::ProfilingMode profilingMode) {
    fbb_.AddElement<int8_t>(FlatConfiguration::VT_PROFILINGMODE, static_cast<int8_t>(profilingMode), 0);
  }
  void add_outputMode(sd::graph::OutputMode outputMode) {
    fbb_.AddElement<int8_t>(FlatConfiguration::VT_OUTPUTMODE, static_cast<int8_t>(outputMode), 0);
  }
  void add_timestats(bool timestats) {
    fbb_.AddElement<uint8_t>(FlatConfiguration::VT_TIMESTATS, static_cast<uint8_t>(timestats), 0);
  }
  void add_footprintForward(int64_t footprintForward) {
    fbb_.AddElement<int64_t>(FlatConfiguration::VT_FOOTPRINTFORWARD, footprintForward, 0);
  }
  void add_footprintBackward(int64_t footprintBackward) {
    fbb_.AddElement<int64_t>(FlatConfiguration::VT_FOOTPRINTBACKWARD, footprintBackward, 0);
  }
  void add_direction(sd::graph::Direction direction) {
    fbb_.AddElement<int8_t>(FlatConfiguration::VT_DIRECTION, static_cast<int8_t>(direction), 0);
  }
  explicit FlatConfigurationBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  FlatConfigurationBuilder &operator=(const FlatConfigurationBuilder &);
  flatbuffers::Offset<FlatConfiguration> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<FlatConfiguration>(end);
    return o;
  }
};

inline flatbuffers::Offset<FlatConfiguration> CreateFlatConfiguration(
    flatbuffers::FlatBufferBuilder &_fbb,
    int64_t id = 0,
    sd::graph::ExecutionMode executionMode = sd::graph::ExecutionMode_SEQUENTIAL,
    sd::graph::ProfilingMode profilingMode = sd::graph::ProfilingMode_NONE,
    sd::graph::OutputMode outputMode = sd::graph::OutputMode_IMPLICIT,
    bool timestats = false,
    int64_t footprintForward = 0,
    int64_t footprintBackward = 0,
    sd::graph::Direction direction = sd::graph::Direction_FORWARD_ONLY) {
  FlatConfigurationBuilder builder_(_fbb);
  builder_.add_footprintBackward(footprintBackward);
  builder_.add_footprintForward(footprintForward);
  builder_.add_id(id);
  builder_.add_direction(direction);
  builder_.add_timestats(timestats);
  builder_.add_outputMode(outputMode);
  builder_.add_profilingMode(profilingMode);
  builder_.add_executionMode(executionMode);
  return builder_.Finish();
}

inline const sd::graph::FlatConfiguration *GetFlatConfiguration(const void *buf) {
  return flatbuffers::GetRoot<sd::graph::FlatConfiguration>(buf);
}

inline const sd::graph::FlatConfiguration *GetSizePrefixedFlatConfiguration(const void *buf) {
  return flatbuffers::GetSizePrefixedRoot<sd::graph::FlatConfiguration>(buf);
}

inline bool VerifyFlatConfigurationBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<sd::graph::FlatConfiguration>(nullptr);
}

inline bool VerifySizePrefixedFlatConfigurationBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<sd::graph::FlatConfiguration>(nullptr);
}

inline void FinishFlatConfigurationBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<sd::graph::FlatConfiguration> root) {
  fbb.Finish(root);
}

inline void FinishSizePrefixedFlatConfigurationBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<sd::graph::FlatConfiguration> root) {
  fbb.FinishSizePrefixed(root);
}

}  // namespace graph
}  // namespace sd

#endif  // FLATBUFFERS_GENERATED_CONFIG_SD_GRAPH_H_
