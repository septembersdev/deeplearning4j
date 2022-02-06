# automatically generated by the FlatBuffers compiler, do not modify

# namespace: graph

import flatbuffers
from flatbuffers.compat import import_numpy
np = import_numpy()

class FlatArrayList(object):
    __slots__ = ['_tab']

    @classmethod
    def GetRootAsFlatArrayList(cls, buf, offset):
        n = flatbuffers.encode.Get(flatbuffers.packer.uoffset, buf, offset)
        x = FlatArrayList()
        x.Init(buf, n + offset)
        return x

    # FlatArrayList
    def Init(self, buf, pos):
        self._tab = flatbuffers.table.Table(buf, pos)

    # FlatArrayList
    def List(self, j):
        o = flatbuffers.number_types.UOffsetTFlags.py_type(self._tab.Offset(4))
        if o != 0:
            x = self._tab.Vector(o)
            x += flatbuffers.number_types.UOffsetTFlags.py_type(j) * 4
            x = self._tab.Indirect(x)
            from sd.graph.FlatArray import FlatArray
            obj = FlatArray()
            obj.Init(self._tab.Bytes, x)
            return obj
        return None

    # FlatArrayList
    def ListLength(self):
        o = flatbuffers.number_types.UOffsetTFlags.py_type(self._tab.Offset(4))
        if o != 0:
            return self._tab.VectorLen(o)
        return 0

    # FlatArrayList
    def ListIsNone(self):
        o = flatbuffers.number_types.UOffsetTFlags.py_type(self._tab.Offset(4))
        return o == 0

def FlatArrayListStart(builder): builder.StartObject(1)
def FlatArrayListAddList(builder, list): builder.PrependUOffsetTRelativeSlot(0, flatbuffers.number_types.UOffsetTFlags.py_type(list), 0)
def FlatArrayListStartListVector(builder, numElems): return builder.StartVector(4, numElems, 4)
def FlatArrayListEnd(builder): return builder.EndObject()
