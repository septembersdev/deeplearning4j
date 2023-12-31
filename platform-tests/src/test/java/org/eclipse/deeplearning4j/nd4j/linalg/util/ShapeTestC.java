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

package org.eclipse.deeplearning4j.nd4j.linalg.util;

import lombok.extern.slf4j.Slf4j;
import lombok.val;
import org.junit.jupiter.api.Tag;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.MethodSource;

import org.nd4j.common.tests.tags.NativeTag;
import org.nd4j.common.tests.tags.TagNames;
import org.nd4j.linalg.BaseNd4jTestWithBackends;
import org.nd4j.linalg.api.buffer.DataType;
import org.nd4j.linalg.api.ndarray.INDArray;
import org.nd4j.linalg.api.ops.impl.shape.Tile;
import org.nd4j.linalg.api.shape.Shape;
import org.nd4j.linalg.exception.ND4JIllegalStateException;
import org.nd4j.linalg.factory.Nd4j;
import org.nd4j.linalg.factory.Nd4jBackend;

import static org.junit.jupiter.api.Assertions.*;

/**
 * @author Adam Gibson
 */
@Slf4j
@Tag(TagNames.NDARRAY_INDEXING)
@NativeTag
public class ShapeTestC extends BaseNd4jTestWithBackends {



    @ParameterizedTest
    @MethodSource("org.nd4j.linalg.BaseNd4jTestWithBackends#configs")
    public void testToOffsetZero(Nd4jBackend backend) {
        INDArray matrix = Nd4j.rand(3, 5);
        INDArray rowOne = matrix.getRow(1);
        INDArray row1Copy = Shape.toOffsetZero(rowOne);
        assertEquals(rowOne, row1Copy);
        INDArray rows = matrix.getRows(1, 2);
        INDArray rowsOffsetZero = Shape.toOffsetZero(rows);
        assertEquals(rows, rowsOffsetZero);

        INDArray tensor = Nd4j.rand(new int[] {3, 3, 3});
        INDArray getTensor = tensor.slice(1).slice(1);
        INDArray getTensorZero = Shape.toOffsetZero(getTensor);
        assertEquals(getTensor, getTensorZero);


    }


    @ParameterizedTest
    @MethodSource("org.nd4j.linalg.BaseNd4jTestWithBackends#configs")
    public void testTile(Nd4jBackend backend) {
        INDArray arr = Nd4j.scalar(DataType.DOUBLE, 1.0).reshape(1, 1);
        //INDArray[] inputs, INDArray[] outputs, int[] axis
        INDArray result = Nd4j.createUninitialized(DataType.DOUBLE, 2,2);
        Tile tile = new Tile(new INDArray[]{arr},new INDArray[]{result},new int[] {2,2});
        Nd4j.getExecutioner().execAndReturn(tile);
        INDArray tiled = Nd4j.tile(arr,2,2).castTo(DataType.DOUBLE);
        assertEquals(tiled,result);

    }

    @ParameterizedTest
    @MethodSource("org.nd4j.linalg.BaseNd4jTestWithBackends#configs")
    public void testElementWiseCompareOnesInMiddle(Nd4jBackend backend) {
        INDArray arr = Nd4j.linspace(1, 6, 6).reshape(2, 3);
        INDArray onesInMiddle = Nd4j.linspace(1, 6, 6).reshape(2, 1, 3);
        for (int i = 0; i < arr.length(); i++)
            assertEquals(arr.getDouble(i), onesInMiddle.getDouble(i), 1e-3);
    }


    @ParameterizedTest
    @MethodSource("org.nd4j.linalg.BaseNd4jTestWithBackends#configs")
    public void testKeepDimsShape_1_T(Nd4jBackend backend) {
        val shape = new long[]{5, 5};
        val axis = new long[]{1, 0, 1};

        val result = Shape.getReducedShape(shape, axis, true, true);

        assertArrayEquals(new long[]{1, 1}, result);
    }

    @ParameterizedTest
    @MethodSource("org.nd4j.linalg.BaseNd4jTestWithBackends#configs")
    public void testKeepDimsShape_1_F(Nd4jBackend backend) {
        val shape = new long[]{5, 5};
        val axis = new long[]{0, 0, 1};

        val result = Shape.getReducedShape(shape, axis, false, true);

        assertArrayEquals(new long[]{}, result);
    }

    @ParameterizedTest
    @MethodSource("org.nd4j.linalg.BaseNd4jTestWithBackends#configs")
    public void testKeepDimsShape_2_T(Nd4jBackend backend) {
        val shape = new long[]{5, 5, 5};
        val axis = new long[]{1, 0, 1};

        val result = Shape.getReducedShape(shape, axis, true, true);

        assertArrayEquals(new long[]{1, 1, 5}, result);
    }

    @ParameterizedTest
    @MethodSource("org.nd4j.linalg.BaseNd4jTestWithBackends#configs")
    public void testKeepDimsShape_2_F(Nd4jBackend backend) {
        val shape = new long[]{5, 5, 5};
        val axis = new long[]{0, 0, 1};

        val result = Shape.getReducedShape(shape, axis, false, true);

        assertArrayEquals(new long[]{5}, result);
    }


    @ParameterizedTest
    @MethodSource("org.nd4j.linalg.BaseNd4jTestWithBackends#configs")
    public void testKeepDimsShape_3_T(Nd4jBackend backend) {
        val shape = new long[]{1, 1};
        val axis = new long[]{1, 0, 1};

        val result = Shape.getReducedShape(shape, axis, true, true);

        assertArrayEquals(new long[]{1, 1}, result);
    }

    @ParameterizedTest
    @MethodSource("org.nd4j.linalg.BaseNd4jTestWithBackends#configs")
    public void testKeepDimsShape_3_F(Nd4jBackend backend) {
        val shape = new long[]{1, 1};
        val axis = new long[]{0, 0};

        val result = Shape.getReducedShape(shape, axis, false, true);

        log.info("Result: {}", result);

        assertArrayEquals(new long[]{1}, result);
    }


    @ParameterizedTest
    @MethodSource("org.nd4j.linalg.BaseNd4jTestWithBackends#configs")
    public void testKeepDimsShape_4_F(Nd4jBackend backend) {
        val shape = new long[]{4, 4};
        val axis = new long[]{0, 0};

        val result = Shape.getReducedShape(shape, axis, false, true);

        log.info("Result: {}", result);

        assertArrayEquals(new long[]{4}, result);
    }


    @ParameterizedTest
    @MethodSource("org.nd4j.linalg.BaseNd4jTestWithBackends#configs")
    public void testAxisNormalization_1(Nd4jBackend backend) {
        val axis = new long[] {1, -2};
        val rank = 2;
        val exp = new long[] {0, 1};

        val norm = Shape.normalizeAxis(rank, axis);
        assertArrayEquals(exp, norm);
    }

    @ParameterizedTest
    @MethodSource("org.nd4j.linalg.BaseNd4jTestWithBackends#configs")
    public void testAxisNormalization_2(Nd4jBackend backend) {
        val axis = new long[] {1, -2, 0};
        val rank = 2;
        val exp = new long[] {0, 1};

        val norm = Shape.normalizeAxis(rank, axis);
        assertArrayEquals(exp, norm);
    }

    @ParameterizedTest
    @MethodSource("org.nd4j.linalg.BaseNd4jTestWithBackends#configs")
    public void testAxisNormalization_3(Nd4jBackend backend) {
        assertThrows(ND4JIllegalStateException.class,() -> {
            val axis = new long[] {1, -2, 2};
            val rank = 2;
            val exp = new long[] {0, 1};

            val norm = Shape.normalizeAxis(rank, axis);
            assertArrayEquals(exp, norm);
        });

    }

    @ParameterizedTest
    @MethodSource("org.nd4j.linalg.BaseNd4jTestWithBackends#configs")
    public void testAxisNormalization_4(Nd4jBackend backend) {
        val axis = new long[] {1, 2, 0};
        val rank = 3;
        val exp = new long[] {0, 1, 2};

        val norm = Shape.normalizeAxis(rank, axis);
        assertArrayEquals(exp, norm);
    }

    @Override
    public char ordering() {
        return 'c';
    }
}
