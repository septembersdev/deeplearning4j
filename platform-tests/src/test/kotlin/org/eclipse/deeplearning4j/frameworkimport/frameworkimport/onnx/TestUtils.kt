package org.eclipse.deeplearning4j.frameworkimport.frameworkimport.onnx

import onnx.Onnx
import org.junit.jupiter.api.Assertions
import org.nd4j.linalg.api.ndarray.INDArray
import org.nd4j.samediff.frameworkimport.ImportGraph
import org.nd4j.samediff.frameworkimport.onnx.*
import org.nd4j.samediff.frameworkimport.onnx.definitions.OnnxOpDeclarations
import org.nd4j.samediff.frameworkimport.onnx.definitions.registry
import org.nd4j.samediff.frameworkimport.onnx.ir.OnnxIRGraph
import org.nd4j.samediff.frameworkimport.onnx.ir.OnnxIRGraphRunner
import org.nd4j.shade.protobuf.ByteString
import java.nio.charset.Charset

fun runAssertion(graph: Onnx.GraphProto,input: Map<String,INDArray>,outputs: List<String>) {
    var declarations = OnnxOpDeclarations
    var onnxOpRegistry = registry()

    var onnxIRGraph = OnnxIRGraph(graph,onnxOpRegistry)
    var onnxGraphRunner = OnnxIRGraphRunner(onnxIRGraph,input.keys.toList(),outputs)
    var assertion = onnxGraphRunner.run(input)
    var importGraph = ImportGraph<Onnx.GraphProto,Onnx.NodeProto,Onnx.NodeProto,Onnx.TensorProto,Onnx.AttributeProto,Onnx.AttributeProto,Onnx.TensorProto.DataType>()

    var importedGraph = importGraph.importGraph(onnxIRGraph,null,null, convertToOnnxTensors(input),onnxOpRegistry)
    var result = importedGraph.output(input,outputs)
    Assertions.assertEquals(assertion, result)
}

fun createSingleNodeGraph(inputs: Map<String, INDArray>, op: String, attributes: Map<String,Any>, outputs: List<String>, inputNames: List<String>, templateTensor: INDArray = inputs.values.first()): Onnx.GraphProto {

    var op = NodeProto {
        inputNames.forEach { t ->
            Input(t)
        }

        name = op.toLowerCase()

        outputs.forEach {
            Output(it)
        }
        attributes.forEach { (t, u) ->
            var attr = AttributeProto {
                name = t
            }
            var toBuilder = attr.toBuilder()
            when(u) {
                is Onnx.TensorProto-> {
                    toBuilder.t = u
                    toBuilder.type = Onnx.AttributeProto.AttributeType.TENSOR
                }
                is java.lang.Double -> {
                    toBuilder.f = (u as Double).toFloat()
                    toBuilder.type = Onnx.AttributeProto.AttributeType.STRING
                }
                is java.lang.Float -> {
                    toBuilder.f = u as Float
                    toBuilder.type = Onnx.AttributeProto.AttributeType.FLOAT
                }
                is Integer -> {
                    toBuilder.i = (u as Integer).toLong()
                    toBuilder.type = Onnx.AttributeProto.AttributeType.INT
                }
                is java.lang.Long -> {
                    toBuilder.i = u as Long
                    toBuilder.type = Onnx.AttributeProto.AttributeType.INT
                }
                is java.lang.String -> {
                    toBuilder.s = byteString(u as String)
                    toBuilder.type = Onnx.AttributeProto.AttributeType.STRING
                }

               is java.util.List<*> -> {
                    when(u[0]) {
                        is java.lang.Double -> {
                            var intsCast = u as List<Double>
                            toBuilder.addAllFloats(intsCast.map { input -> input.toFloat() })
                            toBuilder.type = Onnx.AttributeProto.AttributeType.FLOATS

                        }

                        is java.lang.Float -> {
                            var intsCast = u as List<Float>
                            toBuilder.addAllFloats(intsCast.map { input -> input })
                            toBuilder.type = Onnx.AttributeProto.AttributeType.FLOATS

                        }

                        is Integer -> {
                            var intsCast = u as List<Integer>
                            toBuilder.addAllInts(intsCast.map { input -> input.toLong() })
                            toBuilder.type = Onnx.AttributeProto.AttributeType.INTS

                        }

                        is java.lang.Long -> {
                            var intsCast = u as List<Long>
                            toBuilder.addAllInts(intsCast.map { input -> input })
                            toBuilder.type = Onnx.AttributeProto.AttributeType.INTS
                        }

                        is java.lang.String -> {
                            var stringCast  = u as List<String>
                            toBuilder.addAllStrings(stringCast.map { input -> ByteString.copyFrom(input, Charset.defaultCharset()) })
                            toBuilder.type = Onnx.AttributeProto.AttributeType.STRINGS
                        }

                        is Onnx.TensorProto -> {
                            var listTensors = u as List<Onnx.TensorProto>
                            toBuilder.addAllTensors(listTensors)
                            toBuilder.type = Onnx.AttributeProto.AttributeType.TENSORS
                        }

                    }
                }
            }

            toBuilder.name = t
            Attribute(toBuilder.build())
        }
        opType = op
    }

    var graphRet = GraphProto {
        Node(op)
        name = op.opType.toLowerCase()
        inputs.forEach { (t, u) ->
            if(!t.isEmpty())
                Input(createValueInfoFromTensor(u,t,false))
        }
        outputs.forEach {
            Output(createValueInfoFromTensor(templateTensor,it,false))
        }


    }

    return graphRet

}
