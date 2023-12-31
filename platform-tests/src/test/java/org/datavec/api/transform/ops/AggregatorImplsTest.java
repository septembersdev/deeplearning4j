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
package org.datavec.api.transform.ops;


import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;
import org.nd4j.common.tests.BaseND4JTest;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import static org.junit.jupiter.api.Assertions.*;

@DisplayName("Aggregator Impls Test")
class AggregatorImplsTest extends BaseND4JTest {

    private List<Integer> intList = new ArrayList<>(Arrays.asList(1, 2, 3, 4, 5, 6, 7, 8, 9));

    private List<String> stringList = new ArrayList<>(Arrays.asList("arakoa", "abracadabra", "blast", "acceptance"));

    @Test
    @DisplayName("Aggregable First Test")
    void aggregableFirstTest() {
        AggregatorImpls.AggregableFirst<Integer> first = new AggregatorImpls.AggregableFirst<>();
        for (int i = 0; i < intList.size(); i++) {
            first.accept(intList.get(i));
        }
        assertEquals(1, first.get().toInt());
        AggregatorImpls.AggregableFirst<String> firstS = new AggregatorImpls.AggregableFirst<>();
        for (int i = 0; i < stringList.size(); i++) {
            firstS.accept(stringList.get(i));
        }
        assertTrue(firstS.get().toString().equals("arakoa"));
        AggregatorImpls.AggregableFirst<Integer> reverse = new AggregatorImpls.AggregableFirst<>();
        for (int i = 0; i < intList.size(); i++) {
            reverse.accept(intList.get(intList.size() - i - 1));
        }
        first.combine(reverse);
        assertEquals(1, first.get().toInt());
    }

    @Test
    @DisplayName("Aggregable Last Test")
    void aggregableLastTest() {
        AggregatorImpls.AggregableLast<Integer> last = new AggregatorImpls.AggregableLast<>();
        for (int i = 0; i < intList.size(); i++) {
            last.accept(intList.get(i));
        }
        assertEquals(9, last.get().toInt());
        AggregatorImpls.AggregableLast<String> lastS = new AggregatorImpls.AggregableLast<>();
        for (int i = 0; i < stringList.size(); i++) {
            lastS.accept(stringList.get(i));
        }
        assertTrue(lastS.get().toString().equals("acceptance"));
        AggregatorImpls.AggregableLast<Integer> reverse = new AggregatorImpls.AggregableLast<>();
        for (int i = 0; i < intList.size(); i++) {
            reverse.accept(intList.get(intList.size() - i - 1));
        }
        last.combine(reverse);
        assertEquals(1, last.get().toInt());
    }

    @Test
    @DisplayName("Aggregable Count Test")
    void aggregableCountTest() {
        AggregatorImpls.AggregableCount<Integer> cnt = new AggregatorImpls.AggregableCount<>();
        for (int i = 0; i < intList.size(); i++) {
            cnt.accept(intList.get(i));
        }
        assertEquals(9, cnt.get().toInt());
        AggregatorImpls.AggregableCount<String> lastS = new AggregatorImpls.AggregableCount<>();
        for (int i = 0; i < stringList.size(); i++) {
            lastS.accept(stringList.get(i));
        }
        assertEquals(4, lastS.get().toInt());
        AggregatorImpls.AggregableCount<Integer> reverse = new AggregatorImpls.AggregableCount<>();
        for (int i = 0; i < intList.size(); i++) {
            reverse.accept(intList.get(intList.size() - i - 1));
        }
        cnt.combine(reverse);
        assertEquals(18, cnt.get().toInt());
    }

    @Test
    @DisplayName("Aggregable Max Test")
    void aggregableMaxTest() {
        AggregatorImpls.AggregableMax<Integer> mx = new AggregatorImpls.AggregableMax<>();
        for (int i = 0; i < intList.size(); i++) {
            mx.accept(intList.get(i));
        }
        assertEquals(9, mx.get().toInt());
        AggregatorImpls.AggregableMax<Integer> reverse = new AggregatorImpls.AggregableMax<>();
        for (int i = 0; i < intList.size(); i++) {
            reverse.accept(intList.get(intList.size() - i - 1));
        }
        mx.combine(reverse);
        assertEquals(9, mx.get().toInt());
    }

    @Test
    @DisplayName("Aggregable Range Test")
    void aggregableRangeTest() {
        AggregatorImpls.AggregableRange<Integer> mx = new AggregatorImpls.AggregableRange<>();
        for (int i = 0; i < intList.size(); i++) {
            mx.accept(intList.get(i));
        }
        assertEquals(8, mx.get().toInt());
        AggregatorImpls.AggregableRange<Integer> reverse = new AggregatorImpls.AggregableRange<>();
        for (int i = 0; i < intList.size(); i++) {
            reverse.accept(intList.get(intList.size() - i - 1) + 9);
        }
        mx.combine(reverse);
        assertEquals(17, mx.get().toInt());
    }

    @Test
    @DisplayName("Aggregable Min Test")
    void aggregableMinTest() {
        AggregatorImpls.AggregableMin<Integer> mn = new AggregatorImpls.AggregableMin<>();
        for (int i = 0; i < intList.size(); i++) {
            mn.accept(intList.get(i));
        }
        assertEquals(1, mn.get().toInt());
        AggregatorImpls.AggregableMin<Integer> reverse = new AggregatorImpls.AggregableMin<>();
        for (int i = 0; i < intList.size(); i++) {
            reverse.accept(intList.get(intList.size() - i - 1));
        }
        mn.combine(reverse);
        assertEquals(1, mn.get().toInt());
    }

    @Test
    @DisplayName("Aggregable Sum Test")
    void aggregableSumTest() {
        AggregatorImpls.AggregableSum<Integer> sm = new AggregatorImpls.AggregableSum<>();
        for (int i = 0; i < intList.size(); i++) {
            sm.accept(intList.get(i));
        }
        assertEquals(45, sm.get().toInt());
        AggregatorImpls.AggregableSum<Integer> reverse = new AggregatorImpls.AggregableSum<>();
        for (int i = 0; i < intList.size(); i++) {
            reverse.accept(intList.get(intList.size() - i - 1));
        }
        sm.combine(reverse);
        assertEquals(90, sm.get().toInt());
    }

    @Test
    @DisplayName("Aggregable Mean Test")
    void aggregableMeanTest() {
        AggregatorImpls.AggregableMean<Integer> mn = new AggregatorImpls.AggregableMean<>();
        for (int i = 0; i < intList.size(); i++) {
            mn.accept(intList.get(i));
        }
        assertEquals(9l, (long) mn.getCount());
        assertEquals(5D, mn.get().toDouble(), 0.001);
        AggregatorImpls.AggregableMean<Integer> reverse = new AggregatorImpls.AggregableMean<>();
        for (int i = 0; i < intList.size(); i++) {
            reverse.accept(intList.get(intList.size() - i - 1));
        }
        mn.combine(reverse);
        assertEquals(18l, (long) mn.getCount());
        assertEquals(5D, mn.get().toDouble(), 0.001);
    }

    @Test
    @DisplayName("Aggregable Std Dev Test")
    void aggregableStdDevTest() {
        AggregatorImpls.AggregableStdDev<Integer> sd = new AggregatorImpls.AggregableStdDev<>();
        for (int i = 0; i < intList.size(); i++) {
            sd.accept(intList.get(i));
        }
        assertTrue(Math.abs(sd.get().toDouble() - 2.7386) < 0.0001);
        AggregatorImpls.AggregableStdDev<Integer> reverse = new AggregatorImpls.AggregableStdDev<>();
        for (int i = 0; i < intList.size(); i++) {
            reverse.accept(intList.get(intList.size() - i - 1));
        }
        sd.combine(reverse);
        assertTrue(Math.abs(sd.get().toDouble() - 1.8787) < 0.0001,"" + sd.get().toDouble());
    }

    @Test
    @DisplayName("Aggregable Variance")
    void aggregableVariance() {
        AggregatorImpls.AggregableVariance<Integer> sd = new AggregatorImpls.AggregableVariance<>();
        for (int i = 0; i < intList.size(); i++) {
            sd.accept(intList.get(i));
        }
        assertTrue(Math.abs(sd.get().toDouble() - 60D / 8) < 0.0001);
        AggregatorImpls.AggregableVariance<Integer> reverse = new AggregatorImpls.AggregableVariance<>();
        for (int i = 0; i < intList.size(); i++) {
            reverse.accept(intList.get(intList.size() - i - 1));
        }
        sd.combine(reverse);
        assertTrue(Math.abs(sd.get().toDouble() - 3.5294) < 0.0001,"" + sd.get().toDouble());
    }

    @Test
    @DisplayName("Aggregable Uncorrected Std Dev Test")
    void aggregableUncorrectedStdDevTest() {
        AggregatorImpls.AggregableUncorrectedStdDev<Integer> sd = new AggregatorImpls.AggregableUncorrectedStdDev<>();
        for (int i = 0; i < intList.size(); i++) {
            sd.accept(intList.get(i));
        }
        assertTrue(Math.abs(sd.get().toDouble() - 2.582) < 0.0001);
        AggregatorImpls.AggregableUncorrectedStdDev<Integer> reverse = new AggregatorImpls.AggregableUncorrectedStdDev<>();
        for (int i = 0; i < intList.size(); i++) {
            reverse.accept(intList.get(intList.size() - i - 1));
        }
        sd.combine(reverse);
        assertTrue(Math.abs(sd.get().toDouble() - 1.8257) < 0.0001,"" + sd.get().toDouble());
    }

    @Test
    @DisplayName("Aggregable Population Variance")
    void aggregablePopulationVariance() {
        AggregatorImpls.AggregablePopulationVariance<Integer> sd = new AggregatorImpls.AggregablePopulationVariance<>();
        for (int i = 0; i < intList.size(); i++) {
            sd.accept(intList.get(i));
        }
        assertTrue(Math.abs(sd.get().toDouble() - 60D / 9) < 0.0001);
        AggregatorImpls.AggregablePopulationVariance<Integer> reverse = new AggregatorImpls.AggregablePopulationVariance<>();
        for (int i = 0; i < intList.size(); i++) {
            reverse.accept(intList.get(intList.size() - i - 1));
        }
        sd.combine(reverse);
        assertTrue(Math.abs(sd.get().toDouble() - 30D / 9) < 0.0001,"" + sd.get().toDouble());
    }

    @Test
    @DisplayName("Aggregable Count Unique Test")
    void aggregableCountUniqueTest() {
        // at this low range, it's linear counting
        AggregatorImpls.AggregableCountUnique<Integer> cu = new AggregatorImpls.AggregableCountUnique<>();
        for (int i = 0; i < intList.size(); i++) {
            cu.accept(intList.get(i));
        }
        assertEquals(9, cu.get().toInt());
        cu.accept(1);
        assertEquals(9, cu.get().toInt());
        AggregatorImpls.AggregableCountUnique<Integer> reverse = new AggregatorImpls.AggregableCountUnique<>();
        for (int i = 0; i < intList.size(); i++) {
            reverse.accept(intList.get(intList.size() - i - 1));
        }
        cu.combine(reverse);
        assertEquals(9, cu.get().toInt());
    }



    @Test
    @DisplayName("Incompatible Aggregator Test")
    void incompatibleAggregatorTest() {
        assertThrows(UnsupportedOperationException.class,() -> {
            AggregatorImpls.AggregableSum<Integer> sm = new AggregatorImpls.AggregableSum<>();
            for (int i = 0; i < intList.size(); i++) {
                sm.accept(intList.get(i));
            }
            assertEquals(45, sm.get().toInt());
            AggregatorImpls.AggregableMean<Integer> reverse = new AggregatorImpls.AggregableMean<>();
            for (int i = 0; i < intList.size(); i++) {
                reverse.accept(intList.get(intList.size() - i - 1));
            }

            sm.combine(reverse);
            assertEquals(45, sm.get().toInt());
        });

    }
}
