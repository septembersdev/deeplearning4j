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
package org.nd4j.samediff.frameworkimport.rule.attribute

import org.nd4j.ir.OpNamespace
import org.nd4j.samediff.frameworkimport.context.MappingContext
import org.nd4j.samediff.frameworkimport.lookupIndexForArgDescriptor
import org.nd4j.shade.protobuf.GeneratedMessageV3
import org.nd4j.shade.protobuf.ProtocolMessageEnum
import java.lang.IllegalArgumentException

abstract class NumberToBoolean<
        GRAPH_DEF: GeneratedMessageV3,
        OP_DEF_TYPE: GeneratedMessageV3,
        NODE_TYPE: GeneratedMessageV3,
        ATTR_DEF : GeneratedMessageV3,
        ATTR_VALUE_TYPE : GeneratedMessageV3,
        TENSOR_TYPE : GeneratedMessageV3, DATA_TYPE: ProtocolMessageEnum>(mappingNamesToPerform: Map<String, String>,
                                                                          transformerArgs: Map<String, List<OpNamespace.ArgDescriptor>>):
    BaseAttributeExtractionRule<GRAPH_DEF, OP_DEF_TYPE, NODE_TYPE, ATTR_DEF, ATTR_VALUE_TYPE, TENSOR_TYPE, DATA_TYPE>
        (name = "booleantonumber", mappingNamesToPerform = mappingNamesToPerform, transformerArgs = transformerArgs) {

    override fun acceptsInputType(argDescriptorType: AttributeValueType): Boolean {
        return argDescriptorType == AttributeValueType.INT || argDescriptorType == AttributeValueType.FLOAT
    }

    override fun outputsType(argDescriptorType: List<OpNamespace.ArgDescriptor.ArgType>): Boolean {
        return argDescriptorType.contains(OpNamespace.ArgDescriptor.ArgType.BOOL)
    }

    override fun convertAttributes(mappingCtx: MappingContext<GRAPH_DEF, NODE_TYPE, OP_DEF_TYPE, TENSOR_TYPE, ATTR_DEF, ATTR_VALUE_TYPE, DATA_TYPE>): List<OpNamespace.ArgDescriptor> {
        val ret = ArrayList<OpNamespace.ArgDescriptor>()

        for ((k, v) in mappingNamesToPerform()) {
            val descriptorBuilder = OpNamespace.ArgDescriptor.newBuilder()
            descriptorBuilder.name = k
            val irAttribute = mappingCtx.irAttributeValueForNode(v)
            val targetIdx = lookupIndexForArgDescriptor(
                argDescriptorName = k,
                opDescriptorName = mappingCtx.nd4jOpName(),
                argDescriptorType = OpNamespace.ArgDescriptor.ArgType.BOOL
            )

            if(targetIdx < 0) {
                throw java.lang.IllegalArgumentException("Output attribute $k not found with boolean type for op name ${mappingCtx.nd4jOpName()} and input op name ${mappingCtx.opName()}")
            }


            descriptorBuilder.argIndex = targetIdx
            descriptorBuilder.argType = OpNamespace.ArgDescriptor.ArgType.BOOL


            when(irAttribute.attributeValueType()) {
                AttributeValueType.FLOAT -> {
                    descriptorBuilder.boolValue = irAttribute.floatValue() > 0
                }
                AttributeValueType.INT -> {
                    descriptorBuilder.boolValue = irAttribute.intValue() > 0
                }
                else -> {
                    throw IllegalArgumentException("Illegal type ${irAttribute.attributeValueType()}")
                }

            }

            ret.add(descriptorBuilder.build())
        }
        return ret
    }
}