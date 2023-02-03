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

package org.nd4j.linalg.api.ops.performance;

import lombok.NonNull;
import lombok.extern.slf4j.Slf4j;

import org.nd4j.linalg.api.ops.executioner.OpExecutioner;
import org.nd4j.linalg.api.ops.performance.primitives.AveragingTransactionsHolder;
import org.nd4j.linalg.factory.Nd4j;
import org.nd4j.linalg.api.memory.MemcpyDirection;
import org.nd4j.linalg.profiler.OpProfiler;

import java.util.HashMap;
import java.util.Map;

@Slf4j
public class PerformanceTracker {
    private static final PerformanceTracker INSTANCE = new PerformanceTracker();

    private Map<Integer, AveragingTransactionsHolder> bandwidth = new HashMap<>();
    private Map<Integer, AveragingTransactionsHolder> operations = new HashMap<>();

    private PerformanceTracker() {
        // we put in initial holders, one per device
        var nd = Nd4j.getAffinityManager().getNumberOfDevices();
        for (int e = 0; e < nd; e++) {
            bandwidth.put(e, new AveragingTransactionsHolder());
            operations.put(e, new AveragingTransactionsHolder());
        }
    }

    public static PerformanceTracker getInstance() {
        return INSTANCE;
    }

    /**
     * This method stores bandwidth used for given transaction.
     *
     * PLEASE NOTE: Bandwidth is stored in per millisecond value.
     *
     * @param deviceId device used for this transaction
     * @param timeSpentNanos time spent on this transaction in nanoseconds
     * @param numberOfBytes number of bytes
     */
    public long addMemoryTransaction(int deviceId, long timeSpentNanos, long numberOfBytes) {
        // default is H2H transaction
        return addMemoryTransaction(deviceId, timeSpentNanos, numberOfBytes, MemcpyDirection.HOST_TO_HOST);
    }

    /**
     * This method stores bandwidth used for given transaction.
     *
     * PLEASE NOTE: Bandwidth is stored in per millisecond value.
     *
     * @param deviceId device used for this transaction
     * @param timeSpentNanos time spent on this transaction in nanoseconds
     * @param numberOfBytes number of bytes
     * @param direction direction for the given memory transaction
     */
    public long addMemoryTransaction(int deviceId, long timeSpentNanos, long numberOfBytes, @NonNull MemcpyDirection direction) {
        // we calculate bytes per microsecond now
        var bw = (long) (numberOfBytes / (timeSpentNanos / (double) 1000.0));

        // we skip too small values
        if (bw > 0)
            bandwidth.get(deviceId).addValue(direction, bw);

        return bw;
    }

    public void clear() {
        for (var k: bandwidth.keySet())
            bandwidth.get(k).clear();
    }


    public long helperStartTransaction() {
        if (OpProfiler.getInstance().getConfig().isCheckBandwidth())
            return System.nanoTime();
        else
            return 0L;
    }


    public void helperRegisterTransaction(int deviceId, long timeSpentNanos, long numberOfBytes, @NonNull MemcpyDirection direction) {
        // only do something if profiling is enabled

        if (OpProfiler.getInstance().getConfig().isCheckBandwidth()) {
            addMemoryTransaction(deviceId, System.nanoTime() - timeSpentNanos, numberOfBytes, direction);
        }
    }

    public Map<Integer, Map<MemcpyDirection, Long>> getCurrentBandwidth() {
        var result = new HashMap<Integer, Map<MemcpyDirection, Long>>();
        var keys = bandwidth.keySet();
        for (var d: keys) {

            result.put(d, new HashMap<>());

            // get average for each MemcpyDirection and store it
            for (var m: MemcpyDirection.values())
                result.get(d).put(m, bandwidth.get(d).getAverageValue(m));

        }

        return result;
    }
}
