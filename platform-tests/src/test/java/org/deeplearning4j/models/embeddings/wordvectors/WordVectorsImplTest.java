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

package org.deeplearning4j.models.embeddings.wordvectors;

import org.deeplearning4j.BaseDL4JTest;
import org.deeplearning4j.models.embeddings.WeightLookupTable;
import org.deeplearning4j.models.sequencevectors.sequence.SequenceElement;
import org.deeplearning4j.models.word2vec.wordstore.VocabCache;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Tag;
import org.junit.jupiter.api.Test;
import org.mockito.Mockito;
import org.nd4j.common.tests.tags.NativeTag;
import org.nd4j.common.tests.tags.TagNames;
import org.nd4j.linalg.api.ndarray.INDArray;
import org.nd4j.linalg.factory.Nd4j;
import org.nd4j.shade.guava.collect.Lists;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.mockito.Mockito.when;
@Tag(TagNames.FILE_IO)
@NativeTag
public class WordVectorsImplTest extends BaseDL4JTest {
    private VocabCache vocabCache;
    private WeightLookupTable weightLookupTable;
    private WordVectorsImpl<SequenceElement> wordVectors;

    @BeforeEach
    public void init() throws Exception {
        vocabCache = Mockito.mock(VocabCache.class);
        weightLookupTable = Mockito.mock(WeightLookupTable.class);
        wordVectors = new WordVectorsImpl<>();
    }

    @Test
    public void getWordVectors_HaveTwoWordsNotInVocabAndOneIn_ExpectAllNonWordsRemoved() {
        INDArray wordVector = Nd4j.create(1, 1);
        wordVector.putScalar(0, 5);
        when(vocabCache.indexOf("word")).thenReturn(0);
        when(vocabCache.containsWord("word")).thenReturn(true);
        when(weightLookupTable.getWeights()).thenReturn(wordVector);
        wordVectors.setVocab(vocabCache);
        wordVectors.setLookupTable(weightLookupTable);

        INDArray indArray = wordVectors.getWordVectors(Lists.newArrayList("word", "here", "is"));

        assertEquals(wordVector, indArray);
    }
}
