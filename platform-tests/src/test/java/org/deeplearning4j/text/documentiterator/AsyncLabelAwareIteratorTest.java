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

package org.deeplearning4j.text.documentiterator;

import org.deeplearning4j.BaseDL4JTest;
import org.deeplearning4j.text.sentenceiterator.BasicLineIterator;
import org.deeplearning4j.text.sentenceiterator.SentenceIterator;
import org.junit.jupiter.api.Tag;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.Timeout;
import org.nd4j.common.resources.Resources;
import org.nd4j.common.tests.tags.NativeTag;
import org.nd4j.common.tests.tags.TagNames;

import static org.junit.jupiter.api.Assertions.assertEquals;

@Tag(TagNames.FILE_IO)
@NativeTag
public class AsyncLabelAwareIteratorTest extends BaseDL4JTest {
    @Test()
    @Timeout(30000)
    public void nextDocument() throws Exception {
        SentenceIterator sentence = new BasicLineIterator(Resources.asFile("big/raw_sentences.txt"));
        BasicLabelAwareIterator backed = new BasicLabelAwareIterator.Builder(sentence).build();

        int cnt = 0;
        while (backed.hasNextDocument()) {
            backed.nextDocument();
            cnt++;
        }
        assertEquals(97162, cnt);

        backed.reset();

        AsyncLabelAwareIterator iterator = new AsyncLabelAwareIterator(backed, 64);
        cnt = 0;
        while (iterator.hasNext()) {
            iterator.next();
            cnt++;

            if (cnt == 10)
                iterator.reset();
        }
        assertEquals(97172, cnt);
    }

}
