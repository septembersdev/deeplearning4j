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

package org.datavec.local.transforms.functions;

import org.apache.commons.io.FileUtils;
import org.datavec.api.records.reader.impl.csv.CSVRecordReader;
import org.datavec.api.split.FileSplit;
import org.datavec.api.writable.Writable;
import org.junit.jupiter.api.Tag;
import org.junit.jupiter.api.Test;
import org.nd4j.common.io.ClassPathResource;
import org.nd4j.common.tests.tags.TagNames;

import java.io.File;
import java.util.HashSet;
import java.util.List;
import java.util.Set;
import java.util.stream.Collectors;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertTrue;
@Tag(TagNames.FILE_IO)
@Tag(TagNames.JAVA_ONLY)
public class TestLineRecordReaderFunction  {

    @Test
    public void testLineRecordReader() throws Exception {

        File dataFile = new ClassPathResource("iris.dat").getFile();
        List<String> lines = FileUtils.readLines(dataFile);

        List<String> linesRdd = (lines);

        CSVRecordReader rr = new CSVRecordReader(0, ',');

        List<List<Writable>> out = linesRdd.stream().map(input -> new LineRecordReaderFunction(rr).apply(input)).collect(Collectors.toList());
        List<List<Writable>> outList = out;


        CSVRecordReader rr2 = new CSVRecordReader(0, ',');
        rr2.initialize(new FileSplit(dataFile));
        Set<List<Writable>> expectedSet = new HashSet<>();
        int totalCount = 0;
        while (rr2.hasNext()) {
            expectedSet.add(rr2.next());
            totalCount++;
        }

        assertEquals(totalCount, outList.size());

        for (List<Writable> line : outList) {
            assertTrue(expectedSet.contains(line));
        }
    }
}
