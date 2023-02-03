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
import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.NonNull;
import lombok.extern.slf4j.Slf4j;

import onnx.Onnx;
import org.nd4j.autodiff.samediff.SDVariable;
import org.nd4j.autodiff.samediff.SameDiff;
import org.nd4j.common.base.Preconditions;
import org.nd4j.imports.descriptors.properties.PropertyMapping;
import org.nd4j.linalg.api.buffer.DataType;
import org.nd4j.linalg.api.ndarray.INDArray;
import org.nd4j.linalg.api.ops.DynamicCustomOp;
import org.nd4j.linalg.api.ops.impl.layers.convolution.config.PaddingMode;
import org.nd4j.linalg.api.ops.impl.layers.convolution.config.Pooling2DConfig;
import org.nd4j.common.util.ArrayUtil;
import org.nd4j.linalg.util.LinAlgExceptions;
import org.tensorflow.framework.AttrValue;
import org.tensorflow.framework.GraphDef;
import org.tensorflow.framework.NodeDef;

import java.util.*;


/**
 * Average Pooling2D operation
 */
@Slf4j
@Getter
@NoArgsConstructor
public class AvgPooling2D extends DynamicCustomOp {

    protected Pooling2DConfig config;


    public enum Pooling2DType {
        MAX, AVG, PNORM,
    }

    public AvgPooling2D(@NonNull INDArray input, Pooling2DConfig config) {
        this(input, null, config);
    }


    @Builder(builderMethodName = "sameDiffBuilder")
    public AvgPooling2D(SameDiff sameDiff, SDVariable input, Pooling2DConfig config) {
        super(sameDiff, new SDVariable[]{input});
        config.setType(Pooling2D.Pooling2DType.AVG);

        this.config = config;
        addArgs();
    }

    public AvgPooling2D(@NonNull INDArray input, INDArray output, @NonNull Pooling2DConfig config){
        super(new INDArray[]{input}, wrapOrNull(output));
        config.setType(Pooling2D.Pooling2DType.AVG);

        this.config = config;
        addArgs();
    }

    @Override
    public Map<String, Map<String, PropertyMapping>> mappingsForFunction() {
        Map<String, Map<String, PropertyMapping>> ret = new HashMap<>();
        Map<String, PropertyMapping> map = new HashMap<>();
        var strideMapping = PropertyMapping.builder()
                .tfAttrName("strides")
                .onnxAttrName("strides")
                .propertyNames(new String[]{"sW", "sH"})
                .build();

        var paddingMapping = PropertyMapping.builder()
                .onnxAttrName("padding")
                .tfAttrName("padding")
                .propertyNames(new String[]{"pH", "pW"})
                .build();

        var kernelMapping = PropertyMapping.builder()
                .propertyNames(new String[]{"kH", "kW"})
                .tfInputPosition(1)
                .onnxAttrName("ksize")
                .build();

        var dilationMapping = PropertyMapping.builder()
                .onnxAttrName("dilations")
                .propertyNames(new String[]{"dW", "dH"})
                .tfAttrName("rates")
                .build();


        //data_format
        var dataFormatMapping = PropertyMapping.builder()
                .propertyNames(new String[]{"isNHWC"})
                .tfAttrName("data_format")
                .build();

        map.put("sW", strideMapping);
        map.put("sH", strideMapping);
        map.put("kH", kernelMapping);
        map.put("kW", kernelMapping);
        map.put("dW", dilationMapping);
        map.put("dH", dilationMapping);
        map.put("pH", paddingMapping);
        map.put("pW", paddingMapping);
        map.put("isNHWC", dataFormatMapping);

        ret.put(onnxName(), map);
        ret.put(tensorflowName(), map);


        return ret;
    }

    @Override
    public boolean isConfigProperties() {
        return true;
    }

    @Override
    public String configFieldName() {
        return "config";
    }

    @Override
    public Map<String, Object> propertiesForFunction() {
        if(config == null && iArguments.size() > 0) {
            //Perhaps loaded from FlatBuffers - hence we have IArgs but not Config object
            LinAlgExceptions.assertAllConfigured(this,11);
            initConfigFromArgs();
        }

        return config.toProperties();
    }

    private void initConfigFromArgs() {
        config = Pooling2DConfig.builder()
                .kH(iArguments.get(0))
                .kW(iArguments.get(1))
                .sH(iArguments.get(2))
                .sW(iArguments.get(3))
                .pH(iArguments.get(4))
                .pW(iArguments.get(5))
                .dH(iArguments.get(6))
                .dW(iArguments.get(7))
                .paddingMode(PaddingMode.fromNumber(iArguments.get(8).intValue()))
                .extra(iArguments.size() > 9 ? iArguments.get(9) : 0)
                .isNHWC(iArguments.size() > 10 ? iArguments.get(10) == 1 : false)
                .type(Pooling2D.Pooling2DType.AVG)
                .build();
    }

    private void addArgs() {
        addIArgument(config.getKH(),
                config.getKW(),
                config.getSH(),
                config.getSW(),
                config.getPH(),
                config.getPW(),
                config.getDH(),
                config.getDW(),
                config.getPaddingMode().index,
                (int) config.getExtra(),
                ArrayUtil.fromBoolean(config.isNHWC()));

    }

    @Override
    public void configureFromArguments() {
        initConfigFromArgs();
    }

    @Override
    public String opName() {
        return getPoolingPrefix() + "pool2d";
    }


    @Override
    public List<SDVariable> doDiff(List<SDVariable> f1) {
        List<SDVariable> ret = new ArrayList<>();
        List<SDVariable> inputs = new ArrayList<>();
        inputs.addAll(Arrays.asList(args()));
        inputs.add(f1.get(0));
        Pooling2DDerivative pooling2DDerivative = Pooling2DDerivative.derivativeBuilder()
                .inputs(inputs.toArray(new SDVariable[inputs.size()]))
                .sameDiff(sameDiff)
                .config(config)
                .build();
        ret.addAll(Arrays.asList(pooling2DDerivative.outputVariables()));
        return ret;
    }


    @Override
    public void initFromTensorFlow(NodeDef nodeDef, SameDiff initWith, Map<String, AttrValue> attributesForNode, GraphDef graph) {
        var aStrides = nodeDef.getAttrOrThrow("strides");
        var tfStrides = aStrides.getList().getIList();

        var aKernels = nodeDef.getAttrOrThrow("ksize");
        var tfKernels = aKernels.getList().getIList();

        int sH = 0;
        int sW = 0;

        int pH = 0;
        int pW = 0;

        int kH = 0;
        int kW = 0;

        var aPadding = nodeDef.getAttrOrThrow("padding");
        var padding = aPadding.getList().getIList();

        var paddingMode = aPadding.getS().toStringUtf8().replaceAll("\"", "");

        boolean isSameMode = paddingMode.equalsIgnoreCase("SAME");

        String data_format = "nhwc";
        if (nodeDef.containsAttr("data_format")) {
            var attr = nodeDef.getAttrOrThrow("data_format");

            data_format = attr.getS().toStringUtf8().toLowerCase();
        }

        if (data_format.equalsIgnoreCase("nhwc")) {
            sH = tfStrides.get(1).intValue();
            sW = tfStrides.get(2).intValue();

            kH = tfKernels.get(1).intValue();
            kW = tfKernels.get(2).intValue();

            pH = padding.size() > 0 ? padding.get(1).intValue() : 0;
            pW = padding.size() > 0 ? padding.get(2).intValue() : 0;
        } else {
            sH = tfStrides.get(2).intValue();
            sW = tfStrides.get(3).intValue();

            kH = tfKernels.get(2).intValue();
            kW = tfKernels.get(3).intValue();

            pH = padding.size() > 0 ? padding.get(2).intValue() : 0;
            pW = padding.size() > 0 ? padding.get(3).intValue() : 0;
        }

        Pooling2DConfig pooling2DConfig = Pooling2DConfig.builder()
                .sH(sH)
                .sW(sW)
                .type(Pooling2D.Pooling2DType.AVG)
                .paddingMode(PaddingMode.valueOf(paddingMode))
                .kH(kH)
                .kW(kW)
                .pH(pH)
                .pW(pW)
                .isNHWC(data_format.equalsIgnoreCase("nhwc"))
                .extra(0.0) // averaging only for non-padded values
                .build();
        this.config = pooling2DConfig;
        addArgs();
        //log.debug("Pooling: k: [{},{}]; s: [{}, {}], padding: {}", kH, kW, sH, sW, aPadding);
    }

    @Override
    public void initFromOnnx(Onnx.NodeProto node, SameDiff initWith, Map<String, Onnx.AttributeProto> attributesForNode, Onnx.GraphProto graph) {
        var paddingVal = !attributesForNode.containsKey("auto_pad") ? "VALID" : attributesForNode.get("auto_pad").getS().toStringUtf8();
        var kernelShape = attributesForNode.get("kernel_shape").getIntsList();
        var padding = !attributesForNode.containsKey("pads") ? Arrays.asList(1L) : attributesForNode.get("pads").getIntsList();
        var strides = attributesForNode.get("strides").getIntsList();

        Pooling2DConfig pooling2DConfig = Pooling2DConfig.builder()
                .sH(strides.get(0).intValue())
                .sW(strides.get(1).intValue())
                .type(Pooling2D.Pooling2DType.AVG)
                .paddingMode(PaddingMode.valueOf(paddingVal))
                .kH(kernelShape.get(0).intValue())
                .kW(kernelShape.get(1).intValue())
                .pH(padding.get(0).intValue())
                .pW(padding.size() < 2 ? padding.get(0).intValue() : padding.get(1).intValue())
                .build();
        this.config = pooling2DConfig;
        addArgs();
    }


    @Override
    public String onnxName() {
        return "AveragePool";
    }

    @Override
    public String tensorflowName() {
        return "AvgPool";
    }


    public String getPoolingPrefix() {
        return "avg";
    }

    @Override
    public List<DataType> calculateOutputDataTypes(List<DataType> inputDataTypes){
        Preconditions.checkState(inputDataTypes != null && inputDataTypes.size() == 1, "Expected 1 input data type for %s, got %s", getClass(), inputDataTypes);
        return Collections.singletonList(inputDataTypes.get(0));
    }
}
