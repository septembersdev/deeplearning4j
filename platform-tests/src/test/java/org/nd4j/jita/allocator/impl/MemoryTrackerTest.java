/* ******************************************************************************
 *
 *
 * This program and the accompanying materials are made available under the
 * terms of the Apache License, Version 2.0 which is available at
 * https://www.apache.org/licenses/LICENSE-2.0.
 *
 *  See the NOTICE file distributed with this work for additional
 *  information regarding copyright ownership.
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 * SPDX-License-Identifier: Apache-2.0
 ******************************************************************************/

package org.nd4j.jita.allocator.impl;

import lombok.extern.slf4j.Slf4j;

import org.junit.jupiter.api.Tag;
import org.junit.jupiter.api.Test;
import org.nd4j.allocator.impl.MemoryTracker;
import org.nd4j.common.tests.BaseND4JTest;
import org.nd4j.common.tests.tags.TagNames;
import org.nd4j.linalg.api.buffer.DataType;
import org.nd4j.linalg.factory.Nd4j;

@Slf4j
@Tag(TagNames.LARGE_RESOURCES)
@Tag(TagNames.LONG_TEST)
public class MemoryTrackerTest extends BaseND4JTest {

    @Test
    public void testAllocatedDelta() {
        var precBefore = MemoryTracker.getInstance().getPreciseFreeMemory(0);
        var approxBefore = MemoryTracker.getInstance().getApproximateFreeMemory(0);
        var deltaBefore = precBefore - approxBefore;

        for (int i = 0; i < 100; i++) {
            var buffer = Nd4j.createBuffer(DataType.FLOAT, 100000, false);
        }

        var precAfter = MemoryTracker.getInstance().getPreciseFreeMemory(0);
        var approxAfter = MemoryTracker.getInstance().getApproximateFreeMemory(0);
        var deltaAfter =  precAfter - approxAfter;

        log.info("Initial delta: {}; Allocation delta: {}", deltaBefore, deltaAfter);
        log.info("BEFORE: Precise: {}; Approx: {};", precBefore, approxBefore);
        log.info("AFTER: Precise: {}; Approx: {};", precAfter, approxAfter);
        log.info("Precise allocated: {}", precBefore - precAfter);
        log.info("Approx allocated: {}", MemoryTracker.getInstance().getActiveMemory(0));
    }
}