# automatically generated by the FlatBuffers compiler, do not modify

# namespace: graph

import flatbuffers
from flatbuffers.compat import import_numpy
np = import_numpy()

class UISystemInfo(object):
    __slots__ = ['_tab']

    @classmethod
    def GetRootAsUISystemInfo(cls, buf, offset):
        n = flatbuffers.encode.Get(flatbuffers.packer.uoffset, buf, offset)
        x = UISystemInfo()
        x.Init(buf, n + offset)
        return x

    # UISystemInfo
    def Init(self, buf, pos):
        self._tab = flatbuffers.table.Table(buf, pos)

    # UISystemInfo
    def PhysicalCores(self):
        o = flatbuffers.number_types.UOffsetTFlags.py_type(self._tab.Offset(4))
        if o != 0:
            return self._tab.Get(flatbuffers.number_types.Int32Flags, o + self._tab.Pos)
        return 0

def UISystemInfoStart(builder): builder.StartObject(1)
def UISystemInfoAddPhysicalCores(builder, physicalCores): builder.PrependInt32Slot(0, physicalCores, 0)
def UISystemInfoEnd(builder): return builder.EndObject()
