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

package org.nd4j.linalg.api.ops.impl.layers.convolution;

import lombok.Builder;
import org.nd4j.autodiff.samediff.SDVariable;
import org.nd4j.autodiff.samediff.SameDiff;
import org.nd4j.common.base.Preconditions;
import org.nd4j.linalg.api.buffer.DataType;
import org.nd4j.linalg.api.ndarray.INDArray;
import org.nd4j.linalg.api.ops.DynamicCustomOp;
import org.nd4j.linalg.api.ops.impl.layers.convolution.config.Conv2DConfig;
import org.nd4j.common.util.ArrayUtil;
import org.nd4j.linalg.api.ops.impl.layers.convolution.config.PaddingMode;

import java.util.Collections;
import java.util.List;
import java.util.Map;


public class Im2col extends DynamicCustomOp {

    protected Conv2DConfig conv2DConfig;

    @Builder(builderMethodName = "builder")
    public Im2col(SameDiff sameDiff,  INDArray[] inputArrays, INDArray[] outputs, Conv2DConfig conv2DConfig) {
        super(null, inputArrays, outputs);
        if (sameDiff != null) {
            this.sameDiff = sameDiff;
        }

        this.conv2DConfig = conv2DConfig;

        addArgs();
    }

    public Im2col(SameDiff sd, SDVariable input, Conv2DConfig config) {
        super(null, sd, new SDVariable[]{input});
        this.conv2DConfig = config;
        addArgs();
    }

    public Im2col() {}

    public Im2col(INDArray in, Conv2DConfig conv2DConfig) {
        super("im2Col",in,null,null,null);
        this.conv2DConfig = conv2DConfig;
        addArgs();
    }


    protected void addArgs() {
        addIArgument(conv2DConfig.getKH());
        addIArgument(conv2DConfig.getKW());
        addIArgument(conv2DConfig.getSH());
        addIArgument(conv2DConfig.getSW());
        addIArgument(conv2DConfig.getPH());
        addIArgument(conv2DConfig.getPW());
        addIArgument(conv2DConfig.getDH());
        addIArgument(conv2DConfig.getDW());
        addIArgument(conv2DConfig.getPaddingMode().index);
    }


    @Override
    public void setPropertiesForFunction(Map<String, Object> properties) {
        if(conv2DConfig == null) {
            Conv2DConfig.Conv2DConfigBuilder builder =  Conv2DConfig.builder();
            Long dH = getLongValueFromProperty("dH",properties);
            if(dH != null)
                builder.dH(dH);
            Long sW = getLongValueFromProperty("sW",properties);
            if(sW != null)
                builder.sW(sW);
            Long pW = getLongValueFromProperty("pW",properties);
            if(pW != null)
                builder.pW(pW);


            Long dW = getLongValueFromProperty("dW",properties);
            if(dW != null)
                builder.dW(dW);


            Long sH = getLongValueFromProperty("sH",properties);
            if(sH != null)
                builder.sH(sH);

            Long pH = getLongValueFromProperty("pH",properties);
            if(pH != null)
                builder.pH(pH);


            Long kW = getLongValueFromProperty("kW",properties);
            if(kW != null)
                builder.kW(kW);

            Long kH = getLongValueFromProperty("kH",properties);
            if(kH != null)
                builder.kH(kH);


            String paddingMode = getStringFromProperty("paddingMode",properties);
            if(paddingMode != null)
                builder.paddingMode(PaddingMode.valueOf(paddingMode));



            this.conv2DConfig = builder.build();

        }
    }

    @Override
    public void configureFromArguments() {
        if(conv2DConfig == null && iArguments.size() >= 10) {
            conv2DConfig = Conv2DConfig.builder()
                    .kH(iArguments.get(0))
                    .kW(iArguments.get(1))
                    .sH(iArguments.get(2))
                    .sW(iArguments.get(3))
                    .pH(iArguments.get(4))
                    .pW(iArguments.get(5))
                    .dH(iArguments.get(6))
                    .dW(iArguments.get(7))
                    .paddingMode(PaddingMode.fromNumber(iArguments.get(8).intValue()))
                    .build();
        }
    }


    @Override
    public Map<String, Object> propertiesForFunction() {
        return conv2DConfig.toProperties();
    }

    @Override
    public String opName() {
        return "im2col";
    }

    @Override
    public List<SDVariable> doDiff(List<SDVariable> grad) {
        return new Im2colBp(sameDiff, arg(), grad.get(0), conv2DConfig).outputs();
    }

    @Override
    public List<DataType> calculateOutputDataTypes(List<DataType> inputDataTypes){
        Preconditions.checkState(inputDataTypes != null && inputDataTypes.size() == 1, "Expected 1 input data type for %s, got %s", getClass(), inputDataTypes);
        return Collections.singletonList(inputDataTypes.get(0));
    }
}
