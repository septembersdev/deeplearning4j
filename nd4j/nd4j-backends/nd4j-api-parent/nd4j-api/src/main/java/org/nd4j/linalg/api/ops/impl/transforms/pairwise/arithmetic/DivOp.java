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

package org.nd4j.linalg.api.ops.impl.transforms.pairwise.arithmetic;

import lombok.NonNull;
import org.nd4j.autodiff.samediff.SDVariable;
import org.nd4j.autodiff.samediff.SameDiff;
import org.nd4j.linalg.api.ndarray.INDArray;
import org.nd4j.linalg.api.ops.impl.transforms.BaseDynamicTransformOp;
import org.nd4j.linalg.api.ops.impl.transforms.pairwise.arithmetic.bp.DivBpOp;

import java.util.Arrays;
import java.util.List;

/**
 * Division operation
 *
 * @author Adam Gibson
 */
public class DivOp extends BaseDynamicTransformOp {
    public static final String OP_NAME = "divide";

    public DivOp() {}

    public DivOp( @NonNull SameDiff sameDiff, @NonNull SDVariable x, @NonNull SDVariable y) {
        super(sameDiff, new SDVariable[]{x, y}, false);
    }

    public DivOp(INDArray first, INDArray second, INDArray result) {
        this(new INDArray[]{first, second}, result == null ? null : new INDArray[]{result});
    }

    public DivOp( @NonNull INDArray x, INDArray y) {
        this(new INDArray[]{x,y}, null);
    }

    public DivOp( INDArray[] inputs, INDArray[] outputs) {
        super(inputs, outputs);
    }


    @Override
    public String opName() {
        return OP_NAME;
    }

    @Override
    public String onnxName() {
        return "Div";
    }

    @Override
    public String tensorflowName() {
        return "Div";
    }



    @Override
    public List<SDVariable> doDiff(List<SDVariable> i_v) {
        return Arrays.asList(new DivBpOp(sameDiff, larg(), rarg(), i_v.get(0)).outputVariables());
    }


}
