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

package org.deeplearning4j.models.sequencevectors.serialization;

import org.deeplearning4j.BaseDL4JTest;
import org.deeplearning4j.models.word2vec.VocabWord;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Tag;
import org.junit.jupiter.api.Test;
import org.nd4j.common.tests.tags.NativeTag;
import org.nd4j.common.tests.tags.TagNames;

import static org.junit.jupiter.api.Assertions.assertEquals;

@Tag(TagNames.FILE_IO)
@NativeTag
public class AbstractElementFactoryTest extends BaseDL4JTest {

    @BeforeEach
    public void setUp() throws Exception {

    }

    @Test
    public void testDeserialize() throws Exception {
        VocabWord word = new VocabWord(1, "word");
        AbstractElementFactory<VocabWord> factory = new AbstractElementFactory<>(VocabWord.class);

        System.out.println("VocabWord JSON: " + word.toJSON());

        VocabWord word2 = factory.deserialize(word.toJSON());


        assertEquals(word, word2);
    }

    @Test
    public void testSerialize() throws Exception {
        VocabWord word = new VocabWord(1, "word");
        AbstractElementFactory<VocabWord> factory = new AbstractElementFactory<>(VocabWord.class);

        System.out.println("VocabWord JSON: " + factory.serialize(word));

        VocabWord word2 = factory.deserialize(factory.serialize(word));


        assertEquals(word, word2);
    }
}
