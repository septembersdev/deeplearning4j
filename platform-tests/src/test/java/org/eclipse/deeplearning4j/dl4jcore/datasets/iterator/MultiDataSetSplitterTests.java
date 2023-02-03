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

package org.eclipse.deeplearning4j.dl4jcore.datasets.iterator;


import org.deeplearning4j.BaseDL4JTest;
import org.deeplearning4j.datasets.iterator.MultiDataSetIteratorSplitter;
import org.eclipse.deeplearning4j.dl4jcore.datasets.iterator.tools.MultiDataSetGenerator;
import org.junit.jupiter.api.Tag;
import org.junit.jupiter.api.Test;
import org.nd4j.common.tests.tags.NativeTag;
import org.nd4j.common.tests.tags.TagNames;
import org.nd4j.linalg.dataset.api.iterator.MultiDataSetIterator;
import org.nd4j.linalg.exception.ND4JIllegalStateException;

import java.util.List;
import java.util.Random;

import static org.junit.jupiter.api.Assertions.*;
@NativeTag
@Tag(TagNames.FILE_IO)
public class MultiDataSetSplitterTests extends BaseDL4JTest {

    @Test
    public void testSplitter_1() throws Exception {
        var back = new MultiDataSetGenerator(1000, new int[]{32, 100}, new int[]{32, 5});

        var splitter = new MultiDataSetIteratorSplitter(back, 1000, 0.7);

        var train = splitter.getTrainIterator();
        var test = splitter.getTestIterator();
        var numEpochs = 10;

        int gcntTrain = 0;
        int gcntTest = 0;
        int global = 0;
        // emulating epochs here
        for (int e = 0; e < numEpochs; e++){
            int cnt = 0;
            while (train.hasNext()) {
                var data = train.next().getFeatures(0);

                assertEquals( (float) cnt++, data.getFloat(0), 1e-5,"Train failed on iteration " + cnt + "; epoch: " + e);
                gcntTrain++;
                global++;
            }

            train.reset();


            while (test.hasNext()) {
                var data = test.next().getFeatures(0);
                assertEquals( (float) cnt++, data.getFloat(0), 1e-5,"Train failed on iteration " + cnt + "; epoch: " + e);
                gcntTest++;
                global++;
            }

            test.reset();
        }

        assertEquals(1000 * numEpochs, global);
    }


    @Test
    public void testSplitter_2() throws Exception {
        var back = new MultiDataSetGenerator(1000, new int[]{32, 100}, new int[]{32, 5});

        var splitter = new MultiDataSetIteratorSplitter(back, 1000, 0.7);

        var train = splitter.getTrainIterator();
        var test = splitter.getTestIterator();
        var numEpochs = 10;

        int gcntTrain = 0;
        int gcntTest = 0;
        int global = 0;
        // emulating epochs here
        for (int e = 0; e < numEpochs; e++){
            int cnt = 0;
            while (train.hasNext()) {
                var data = train.next().getFeatures(0);

                assertEquals( (float) cnt++, data.getFloat(0), 1e-5,"Train failed on iteration " + cnt + "; epoch: " + e);
                gcntTrain++;
                global++;
            }

            train.reset();

            if (e % 2 == 0)
                while (test.hasNext()) {
                    var data = test.next().getFeatures(0);
                    assertEquals((float) cnt++, data.getFloat(0), 1e-5,"Train failed on iteration " + cnt + "; epoch: " + e);
                    gcntTest++;
                    global++;
                }
        }

        assertEquals(700 * numEpochs + (300 * numEpochs / 2), global);
    }

    @Test()
    public void testSplitter_3() throws Exception {
       assertThrows(ND4JIllegalStateException.class,() -> {
           var back = new MultiDataSetGenerator(1000, new int[]{32, 100}, new int[]{32, 5});

           var splitter = new MultiDataSetIteratorSplitter(back, 1000, 0.7);

           var train = splitter.getTrainIterator();
           var test = splitter.getTestIterator();
           var numEpochs = 10;

           int gcntTrain = 0;
           int gcntTest = 0;
           int global = 0;
           // emulating epochs here
           for (int e = 0; e < numEpochs; e++){
               int cnt = 0;
               while (train.hasNext()) {
                   var data = train.next().getFeatures(0);

                   assertEquals((float) cnt++, data.getFloat(0), 1e-5,"Train failed on iteration " + cnt + "; epoch: " + e);
                   gcntTrain++;
                   global++;
               }

               train.reset();


               while (test.hasNext()) {
                   var data = test.next().getFeatures(0);
                   assertEquals( (float) cnt++, data.getFloat(0), 1e-5,"Train failed on iteration " + cnt + "; epoch: " + e);
                   gcntTest++;
                   global++;
               }

               // shifting underlying iterator by one
               train.hasNext();
               back.shift();
           }

           assertEquals(1000 * numEpochs, global);
       });

    }

    @Test
    public void testMultiSplitter_1() {
        var back = new MultiDataSetGenerator(1000, new int[]{32, 100}, new int[]{32, 5});

        // we're going to mimic train+test+validation split
        var splitter = new MultiDataSetIteratorSplitter(back, new int[]{800, 100, 100});

        assertEquals(3, splitter.getIterators().size());

        var trainIter = splitter.getIterators().get(0);
        var testIter = splitter.getIterators().get(1);
        var validationIter = splitter.getIterators().get(2);

        // we're going to have multiple epochs
        int numEpochs = 10;
        for (int e = 0; e < numEpochs; e++) {
            int globalIter = 0;
            trainIter.reset();
            testIter.reset();
            validationIter.reset();

            boolean trained = false;
            while (trainIter.hasNext()) {
                trained = true;
                var ds = trainIter.next();
                assertNotNull(ds);

                for (int i = 0; i < ds.getFeatures().length; ++i) {
                    assertEquals( (double) globalIter, ds.getFeatures()[i].getDouble(0), 1e-5f,"Failed at iteration [" + globalIter + "]");
                }
                globalIter++;
            }
            assertTrue(trained,"Failed at epoch [" + e + "]");
            assertEquals(800, globalIter);


            // test set is used every epoch
            boolean tested = false;
            //testIter.reset();
            while (testIter.hasNext()) {
                tested = true;
                var ds = testIter.next();
                assertNotNull(ds);

                for (int i = 0; i < ds.getFeatures().length; ++i) {
                    assertEquals((double) globalIter, ds.getFeatures()[i].getDouble(0), 1e-5f,"Failed at iteration [" + globalIter + "]");
                }
                globalIter++;
            }
            assertTrue(tested,"Failed at epoch [" + e + "]");
            assertEquals(900, globalIter);

            // validation set is used every 5 epochs
            if (e % 5 == 0) {
                boolean validated = false;
                //validationIter.reset();
                while (validationIter.hasNext()) {
                    validated = true;
                    var ds = validationIter.next();
                    assertNotNull(ds);

                    for (int i = 0; i < ds.getFeatures().length; ++i) {
                        assertEquals( (double) globalIter, ds.getFeatures()[i].getDouble(0), 1e-5f,"Failed at iteration [" + globalIter + "]");
                    }
                    globalIter++;
                }
                assertTrue(validated,"Failed at epoch [" + e + "]");
            }

            // all 3 iterators have exactly 1000 elements combined
            if (e % 5 == 0)
                assertEquals(1000, globalIter);
            else
                assertEquals(900, globalIter);
            trainIter.reset();
        }
    }

    @Test
    public void testSplitter_5() {
        var back = new MultiDataSetGenerator(1000, new int[]{32, 100}, new int[]{32, 5});

        var splitter = new MultiDataSetIteratorSplitter(back, new int[]{900, 100});

        List<MultiDataSetIterator> iteratorList = splitter.getIterators();
        var numEpochs = 10;

        int global = 0;
        // emulating epochs here
        for (int e = 0; e < numEpochs; e++) {
            int iterNo = 0;
            int perEpoch = 0;
            for (var partIterator : iteratorList) {
                partIterator.reset();
                while (partIterator.hasNext()) {
                    int cnt = 0;
                    var data = partIterator.next().getFeatures();

                    for (int i = 0; i < data.length; ++i) {
                        assertEquals((float) perEpoch, data[i].getFloat(0), 1e-5,"Train failed on iteration " + cnt + "; epoch: " + e);
                    }
                    //gcntTrain++;
                    global++;
                    cnt++;
                    ++perEpoch;
                }
                ++iterNo;
            }
        }

        assertEquals(1000 * numEpochs, global);
    }

    @Test
    public void testSplitter_6() {
        var back = new MultiDataSetGenerator(1000, new int[]{32, 100}, new int[]{32, 5});

        // we're going to mimic train+test+validation split
        var splitter = new MultiDataSetIteratorSplitter(back, new int[]{800, 100, 100});

        assertEquals(3, splitter.getIterators().size());

        var trainIter = splitter.getIterators().get(0);
        var testIter = splitter.getIterators().get(1);
        var validationIter = splitter.getIterators().get(2);

        // we're going to have multiple epochs
        int numEpochs = 10;
        for (int e = 0; e < numEpochs; e++) {
            int globalIter = 0;
            trainIter.reset();
            testIter.reset();
            validationIter.reset();

            boolean trained = false;
            while (trainIter.hasNext()) {
                trained = true;
                var ds = trainIter.next();
                assertNotNull(ds);

                for (int i = 0; i < ds.getFeatures().length; ++i) {
                    assertEquals((double) globalIter,
                            ds.getFeatures()[i].getDouble(0), 1e-5f,"Failed at iteration [" + globalIter + "]");
                }
                globalIter++;
            }
            assertTrue(trained,"Failed at epoch [" + e + "]");
            assertEquals(800, globalIter);


            // test set is used every epoch
            boolean tested = false;
            //testIter.reset();
            while (testIter.hasNext()) {
                tested = true;
                var ds = testIter.next();
                assertNotNull(ds);
                for (int i = 0; i < ds.getFeatures().length; ++i) {
                    assertEquals((double) globalIter, ds.getFeatures()[i].getDouble(0), 1e-5f,"Failed at iteration [" + globalIter + "]");
                }
                globalIter++;
            }
            assertTrue(tested,"Failed at epoch [" + e + "]");
            assertEquals(900, globalIter);

            // validation set is used every 5 epochs
            if (e % 5 == 0) {
                boolean validated = false;
                //validationIter.reset();
                while (validationIter.hasNext()) {
                    validated = true;
                    var ds = validationIter.next();
                    assertNotNull(ds);

                    for (int i = 0; i < ds.getFeatures().length; ++i) {
                        assertEquals((double) globalIter,
                                ds.getFeatures()[i].getDouble(0), 1e-5f,"Failed at iteration [" + globalIter + "]");
                    }
                    globalIter++;
                }
                assertTrue(validated,"Failed at epoch [" + e + "]");
            }

            // all 3 iterators have exactly 1000 elements combined
            if (e % 5 == 0)
                assertEquals(1000, globalIter);
            else
                assertEquals(900, globalIter);
            trainIter.reset();
        }
    }

    @Test
    public void testUnorderedSplitter_1() {
        var back = new MultiDataSetGenerator(1000, new int[]{32, 100}, new int[]{32, 5});

        var splitter = new MultiDataSetIteratorSplitter(back, new int[]{500, 500});

        List<MultiDataSetIterator> iteratorList = splitter.getIterators();
        var numEpochs = 10;

        int global = 0;
        // emulating epochs here
        for (int e = 0; e < numEpochs; e++) {

            // Get data from second part, then rewind for the first one.
            int cnt = 0;
            int partNumber = 1;
            while (iteratorList.get(partNumber).hasNext()) {
                int farCnt = (1000 / 2) * (partNumber) + cnt;
                var data = iteratorList.get(partNumber).next().getFeatures();
                for (int i = 0; i < data.length; ++i) {
                    assertEquals( (float) farCnt, data[i].getFloat(0), 1e-5,"Train failed on iteration " + cnt + "; epoch: " + e);
                }
                cnt++;
                global++;
            }
            iteratorList.get(partNumber).reset();
            partNumber = 0;
            cnt = 0;
            while (iteratorList.get(0).hasNext()) {
                var data = iteratorList.get(0).next().getFeatures();
                for (int i = 0; i < data.length; ++i) {
                    assertEquals((float) cnt++,
                            data[i].getFloat(0), 1e-5,"Train failed on iteration " + cnt + "; epoch: " + e);
                }
                global++;
            }
        }
    }

    @Test
    public void testUnorderedSplitter_2() {
        var back = new MultiDataSetGenerator(1000, new int[]{32, 100}, new int[]{32, 5});

        var splitter = new MultiDataSetIteratorSplitter(back, new int[]{2});

        List<MultiDataSetIterator> iteratorList = splitter.getIterators();

        for (int partNumber = 0 ; partNumber < iteratorList.size(); ++partNumber) {
            int cnt = 0;
            while (iteratorList.get(partNumber).hasNext()) {
                var data = iteratorList.get(partNumber).next().getFeatures();
                for (int i = 0; i < data.length; ++i) {
                    assertEquals( (float) (500 * partNumber + cnt), data[i].getFloat(0), 1e-5,"Train failed on iteration " + cnt);
                }
                cnt++;
            }
        }
    }

    @Test
    public void testUnorderedSplitter_3() {
        var back = new MultiDataSetGenerator(1000, new int[]{32, 100}, new int[]{32, 5});

        var splitter = new MultiDataSetIteratorSplitter(back, new int[]{10});

        List<MultiDataSetIterator> iteratorList = splitter.getIterators();
        Random random = new Random();
        int[] indexes = new int[iteratorList.size()];
        for (int i = 0; i < indexes.length; ++i) {
            indexes[i] = random.nextInt(iteratorList.size());
        }

        for (int partNumber : indexes) {
            int cnt = 0;
            while (iteratorList.get(partNumber).hasNext()) {
                var data = iteratorList.get(partNumber).next().getFeatures();
                for (int i = 0; i < data.length; ++i) {
                    assertEquals( (float) (500 * partNumber + cnt),
                            data[i].getFloat(0), 1e-5,"Train failed on iteration " + cnt);
                }
                cnt++;
            }
        }
    }

    @Test
    public void testUnorderedSplitter_4() {
        var back = new MultiDataSetGenerator(1000, new int[]{32, 100}, new int[]{32, 5});

        // we're going to mimic train+test+validation split
        var splitter = new MultiDataSetIteratorSplitter(back, new int[]{80, 10, 5});

        assertEquals(3, splitter.getIterators().size());

        var trainIter = splitter.getIterators().get(0);  // 0..79
        var testIter = splitter.getIterators().get(1);   // 80 ..89
        var validationIter = splitter.getIterators().get(2); // 90..94

        // we're skipping train/test and go for validation first. we're that crazy, right.
        int valCnt = 0;
        while (validationIter.hasNext()) {
            var ds = validationIter.next();
            assertNotNull(ds);
            for (int i = 0; i < ds.getFeatures().length; ++i) {
                assertEquals((float) valCnt + 90,
                        ds.getFeatures()[i].getFloat(0), 1e-5,"Validation failed on iteration " + valCnt);
            }
            valCnt++;
        }
        assertEquals(5, valCnt);
    }
}
