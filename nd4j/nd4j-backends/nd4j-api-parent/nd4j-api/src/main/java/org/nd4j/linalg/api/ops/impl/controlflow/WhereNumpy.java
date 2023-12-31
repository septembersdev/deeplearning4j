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

package org.nd4j.linalg.api.ops.impl.controlflow;

import lombok.NoArgsConstructor;
import org.nd4j.autodiff.samediff.SDVariable;
import org.nd4j.autodiff.samediff.SameDiff;
import org.nd4j.linalg.api.ndarray.INDArray;
import org.nd4j.linalg.api.ops.DynamicCustomOp;

import java.util.List;

/**
 *
 */
@NoArgsConstructor
public class WhereNumpy extends DynamicCustomOp {
    public WhereNumpy(SameDiff sameDiff, SDVariable[] args) {
        super(null, sameDiff, args);
    }

    public WhereNumpy(String opName, INDArray[] inputs, INDArray[] outputs, List<Double> tArguments, List<Long> iArguments) {
        super(opName, inputs, outputs, tArguments, iArguments);
    }

    public WhereNumpy(INDArray[] inputs, INDArray[] outputs) {
        super(null, inputs, outputs);
    }

    public WhereNumpy(SameDiff sameDiff, SDVariable[] args, boolean inPlace) {
        super(null, sameDiff, args, inPlace);
    }

    public WhereNumpy(SameDiff sd, SDVariable x, SDVariable y, SDVariable condition) {
        this(sd,new SDVariable[]{x,y,condition});
    }

    public WhereNumpy(INDArray x, INDArray y, INDArray condition) {
        this(new INDArray[]{x,y,condition},null);
    }


    @Override
    public String opName() {
        return "where_np";
    }
}
