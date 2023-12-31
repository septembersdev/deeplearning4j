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

package org.nd4j.linalg.api.ops.impl.transforms.custom;

import org.nd4j.shade.guava.primitives.Doubles;
import org.nd4j.shade.guava.primitives.Ints;
import org.nd4j.autodiff.samediff.SDVariable;
import org.nd4j.autodiff.samediff.SameDiff;
import org.nd4j.common.base.Preconditions;
import org.nd4j.linalg.api.buffer.DataType;
import org.nd4j.linalg.api.ndarray.INDArray;
import org.nd4j.linalg.api.ops.DynamicCustomOp;
import org.nd4j.linalg.exception.ND4JIllegalArgumentException;
import org.nd4j.linalg.indexing.conditions.Condition;

import java.util.Collections;
import java.util.List;

public class Choose extends DynamicCustomOp {
    private Condition condition;

    public Choose(SameDiff sameDiff, SDVariable[] args, Condition condition) {
        super(null, sameDiff, args);
        if(condition == null) {
            throw new ND4JIllegalArgumentException("Must specify a condition.");
        }

        this.inPlace = true;
        this.inplaceCall = true;
        addIArgument(condition.conditionType().index);
        this.condition = condition;
    }

    public Choose(String opName, INDArray[] inputs, INDArray[] outputs, List<Double> tArguments, List<Long> iArguments) {
        super(opName, inputs, outputs, tArguments, iArguments);
    }

    public Choose(String opName, INDArray[] inputs, Condition condition) {
        super(opName, inputs, null);
        if(condition == null) {
            throw new ND4JIllegalArgumentException("Must specify a condition.");
        }

        addInputArgument(inputs);
        addIArgument(condition.conditionType().index);
    }

    /**
     *
     * @param inputs
     * @param condition
     */
    public Choose(INDArray[] inputs,Condition condition) {
        this(inputs, Collections.<Integer>emptyList(),Collections.<Double>emptyList(),condition);
    }

    /**
     * Note that iArgs (integer arguments) and  tArgs(double/float arguments)
     * may end up being used under the following conditions:
     * scalar operations (if a scalar is specified the you do not need to specify an ndarray)
     * otherwise, if an ndarray is needed as a second input then put it in the inputs
     *
     * Usually, you only need 1 input (the equivalent of the array you're trying to do indexing on)
     *
     * @param inputs the inputs in to the op
     * @param iArgs the integer arguments as needed
     * @param tArgs the arguments
     * @param condition the condition to filter on
     */
    public Choose(INDArray[] inputs,List<Integer> iArgs, List<Double> tArgs,Condition condition) {
        super(null, inputs, null);
        if(condition == null) {
            throw new ND4JIllegalArgumentException("Must specify a condition.");
        }

        if(!iArgs.isEmpty())
            addIArgument(Ints.toArray(iArgs));

        if(!tArgs.isEmpty())
            addTArgument(Doubles.toArray(tArgs));
        addIArgument(condition.conditionType().index);
    }

    public Choose(String opName, SameDiff sameDiff, SDVariable[] args, boolean inPlace) {
        super(opName, sameDiff, args, inPlace);
    }

    public Choose(){
        //No-arg constructor for use in DifferentialFunctionClassHolder
    }

    @Override
    public String opName() {
        return "choose";
    }

    @Override
    public List<DataType> calculateOutputDataTypes(List<DataType> dataTypes){
        Preconditions.checkState(dataTypes != null && dataTypes.size() == 1, "Expected exactly 1 input datatype for %s, got %s", getClass(), dataTypes);
        return Collections.singletonList(dataTypes.get(0));
    }
}
