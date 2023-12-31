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

package org.deeplearning4j.nn.modelimport.keras.utils;

import lombok.extern.slf4j.Slf4j;
import org.deeplearning4j.nn.modelimport.keras.exceptions.InvalidKerasConfigurationException;
import org.deeplearning4j.nn.modelimport.keras.exceptions.UnsupportedKerasConfigurationException;
import org.nd4j.linalg.learning.config.*;
import org.nd4j.linalg.schedule.InverseSchedule;
import org.nd4j.linalg.schedule.ScheduleType;

import java.util.Map;

@Slf4j
public class KerasOptimizerUtils {

    protected static final String LR = "lr";
    protected static final String LR2 = "learning_rate";
    protected static final String EPSILON = "epsilon";
    protected static final String MOMENTUM = "momentum";
    protected static final String BETA_1 = "beta_1";
    protected static final String BETA_2 = "beta_2";
    protected static final String DECAY = "decay";
    protected static final String RHO = "rho";
    protected static final String SCHEDULE_DECAY = "schedule_decay";

    /**
     * Map Keras optimizer to DL4J IUpdater.
     *
     * @param optimizerConfig Optimizer configuration map
     * @return DL4J IUpdater instance
     */
    public static IUpdater mapOptimizer(Map<String, Object> optimizerConfig)
            throws UnsupportedKerasConfigurationException, InvalidKerasConfigurationException {

        if (!optimizerConfig.containsKey("class_name")) {
            throw new InvalidKerasConfigurationException("Optimizer config does not contain a name field.");
        }
        String optimizerName = (String) optimizerConfig.get("class_name");
        //newer keras versions
        if(optimizerName != null && optimizerName.startsWith("Custom>")) {
            optimizerName = optimizerName.replace("Custom>","");
        }

        if (!optimizerConfig.containsKey("config"))
            throw new InvalidKerasConfigurationException("Field config missing from layer config");
        Map<String, Object> optimizerParameters = (Map<String, Object>) optimizerConfig.get("config");

        IUpdater dl4jOptimizer;


        switch (optimizerName.toLowerCase()) {
            case "adam": {
                double lr = (double) (optimizerParameters.containsKey(LR) ? optimizerParameters.get(LR) : optimizerParameters.get(LR2));
                double beta1 = (double) optimizerParameters.get(BETA_1);
                double beta2 = (double) optimizerParameters.get(BETA_2);
                double epsilon = (double) optimizerParameters.get(EPSILON);
                double decay = (double) (optimizerParameters.containsKey(DECAY) ? optimizerParameters.get(DECAY) : Adam.DEFAULT_ADAM_BETA1_MEAN_DECAY);

                dl4jOptimizer = new Adam.Builder()
                        .beta1(beta1).beta2(beta2)
                        .epsilon(epsilon).learningRate(lr)
						.learningRateSchedule(decay == 0 ? null : new InverseSchedule(ScheduleType.ITERATION, lr, decay, 1))
                        .build();
                break;
            }
            case "adadelta": {
                double rho = (double) optimizerParameters.get(RHO);
                double epsilon = (double) optimizerParameters.get(EPSILON);

                dl4jOptimizer = new AdaDelta.Builder()
                        .epsilon(epsilon).rho(rho)
                        .build();
                break;
            }
            case "adagrad": {
                double lr = (double) (optimizerParameters.containsKey(LR) ? optimizerParameters.get(LR) : optimizerParameters.get(LR2));
                double epsilon = (double) optimizerParameters.get(EPSILON);
                double decay = (double) optimizerParameters.get(DECAY);

                dl4jOptimizer = new AdaGrad.Builder()
                        .epsilon(epsilon).learningRate(lr)
						.learningRateSchedule(decay == 0 ? null : new InverseSchedule(ScheduleType.ITERATION, lr, decay, 1))
                        .build();
                break;
            }
            case "adamax": {
                double lr = (double) (optimizerParameters.containsKey(LR) ? optimizerParameters.get(LR) : optimizerParameters.get(LR2));
                double beta1 = (double) optimizerParameters.get(BETA_1);
                double beta2 = (double) optimizerParameters.get(BETA_2);
                double epsilon = (double) optimizerParameters.get(EPSILON);

                dl4jOptimizer = new AdaMax(lr, beta1, beta2, epsilon);
                break;
            }
            case "nadam": {
                double lr = (double) (optimizerParameters.containsKey(LR) ? optimizerParameters.get(LR) : optimizerParameters.get(LR2));
                double beta1 = (double) optimizerParameters.get(BETA_1);
                double beta2 = (double) optimizerParameters.get(BETA_2);
                double epsilon = (double) optimizerParameters.get(EPSILON);
                double scheduleDecay = (double) optimizerParameters.getOrDefault(SCHEDULE_DECAY,0.0);

                dl4jOptimizer = new Nadam.Builder()
                        .beta1(beta1).beta2(beta2)
                        .epsilon(epsilon).learningRate(lr)
						.learningRateSchedule(scheduleDecay == 0 ? null : new InverseSchedule(ScheduleType.ITERATION, lr, 
								scheduleDecay, 1))
                        .build();
                break;
            }
            case "sgd": {
                double lr = (double) (optimizerParameters.containsKey(LR) ? optimizerParameters.get(LR) : optimizerParameters.get(LR2));
                double momentum = (double) (optimizerParameters.containsKey(EPSILON) ? optimizerParameters.get(EPSILON) : optimizerParameters.get(MOMENTUM));

                double decay = (double) optimizerParameters.get(DECAY);

                dl4jOptimizer = new Nesterovs.Builder()
                        .momentum(momentum).learningRate(lr)
						.learningRateSchedule(decay == 0 ? null : new InverseSchedule(ScheduleType.ITERATION, lr, decay, 1))
                        .build();
                break;
            }
            case "rmsprop": {
                double lr = (double) (optimizerParameters.containsKey(LR) ? optimizerParameters.get(LR) : optimizerParameters.get(LR2));
                double rho = (double) optimizerParameters.get(RHO);
                double epsilon = (double) optimizerParameters.get(EPSILON);
                double decay = (double) optimizerParameters.get(DECAY);

                dl4jOptimizer = new RmsProp.Builder()
                        .epsilon(epsilon).rmsDecay(rho).learningRate(lr)
						.learningRateSchedule(decay == 0 ? null : new InverseSchedule(ScheduleType.ITERATION, lr, decay, 1))
                        .build();
                break;
            }
            default:
                throw new UnsupportedKerasConfigurationException("Optimizer with name " + optimizerName + "can not be" +
                        "matched to a DL4J optimizer. Note that custom TFOptimizers are not supported by model import");
        }

        return dl4jOptimizer;

    }
}
