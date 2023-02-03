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

package org.nd4j.parameterserver.distributed.v2;

import io.reactivex.functions.Consumer;
import lombok.extern.slf4j.Slf4j;

import org.apache.commons.lang3.RandomUtils;
import org.junit.jupiter.api.*;
import org.nd4j.common.primitives.AtomicBoolean;
import org.nd4j.common.tests.BaseND4JTest;
import org.nd4j.common.tests.tags.NativeTag;
import org.nd4j.common.tests.tags.TagNames;
import org.nd4j.linalg.api.ndarray.INDArray;
import org.nd4j.linalg.factory.Nd4j;
import org.nd4j.parameterserver.distributed.conf.VoidConfiguration;
import org.nd4j.parameterserver.distributed.v2.enums.MeshBuildMode;
import org.nd4j.parameterserver.distributed.v2.messages.impl.GradientsUpdateMessage;
import org.nd4j.parameterserver.distributed.v2.messages.pairs.params.ModelParametersMessage;
import org.nd4j.parameterserver.distributed.v2.messages.pairs.params.ModelParametersRequest;
import org.nd4j.parameterserver.distributed.v2.messages.pairs.params.UpdaterParametersMessage;
import org.nd4j.parameterserver.distributed.v2.messages.pairs.params.UpdaterParametersRequest;
import org.nd4j.parameterserver.distributed.v2.transport.MessageCallable;
import org.nd4j.parameterserver.distributed.v2.transport.impl.DelayedDummyTransport;
import org.nd4j.parameterserver.distributed.v2.transport.impl.DummyTransport;
import org.nd4j.parameterserver.distributed.v2.util.AbstractSubscriber;
import org.nd4j.parameterserver.distributed.v2.util.AbstractUpdatesHandler;
import org.nd4j.parameterserver.distributed.v2.util.MessageSplitter;

import java.util.ArrayList;
import java.util.concurrent.atomic.AtomicInteger;

import static org.junit.jupiter.api.Assertions.*;

@Slf4j
@Disabled
@Tag(TagNames.FILE_IO)
@Tag(TagNames.DIST_SYSTEMS)
@NativeTag
public class DelayedModelParameterServerTest extends BaseND4JTest {
    private static final String rootId = "ROOT_NODE";

    @BeforeEach
    public void setUp() throws Exception {
        MessageSplitter.getInstance().reset();
    }

    @AfterEach
    public void setDown() throws Exception {
        MessageSplitter.getInstance().reset();
    }

    @Test()
    @Timeout(20000L)
    public void testBasicInitialization_1() throws Exception {
        var connector = new DummyTransport.Connector();
        var rootTransport = new DelayedDummyTransport(rootId, connector);

        connector.register(rootTransport);

        var rootServer = new ModelParameterServer(rootTransport, true);
        rootServer.launch();

        assertEquals(rootId, rootTransport.getUpstreamId());

        rootServer.shutdown();
    }

    @Test()
    @Timeout(40000L)
    public void testBasicInitialization_2() throws Exception {
        for (int e = 0; e < 100; e++) {
            var connector = new DummyTransport.Connector();
            var rootTransport = new DelayedDummyTransport(rootId, connector);
            var clientTransportA = new DelayedDummyTransport("123", connector, rootId);
            var clientTransportB = new DelayedDummyTransport("1234", connector, rootId);

            connector.register(rootTransport, clientTransportA, clientTransportB);

            var rootServer = new ModelParameterServer(rootTransport, true);
            var clientServerA = new ModelParameterServer(clientTransportA, false);
            var clientServerB = new ModelParameterServer(clientTransportB, false);
            rootServer.launch();
            clientServerA.launch();
            clientServerB.launch();

            // since clientB starts AFTER clientA, we have to wait till MeshUpdate message is propagated, since ithis message is NOT blocking
            Thread.sleep(25);

            var meshR = rootTransport.getMesh();
            var meshA = clientTransportA.getMesh();
            var meshB = clientTransportB.getMesh();

            assertEquals(3, meshR.totalNodes(),"Root node failed");
            assertEquals(3, meshB.totalNodes(),"B node failed");
            assertEquals(3, meshA.totalNodes(),"A node failed");
            assertEquals(meshR, meshA);
            assertEquals(meshA, meshB);

            log.info("Iteration [{}] finished", e);
        }
    }
    @Test()
    @Timeout(180000L)
    public void testUpdatesPropagation_1() throws Exception {
        var conf = VoidConfiguration.builder().meshBuildMode(MeshBuildMode.PLAIN).build();
        var array = Nd4j.ones(10, 10);

        var connector = new DummyTransport.Connector();
        var rootTransport = new DelayedDummyTransport(rootId, connector, rootId, conf);
        var clientTransportA = new DelayedDummyTransport("412334", connector, rootId, conf);
        var clientTransportB = new DelayedDummyTransport("123441", connector, rootId, conf);

        connector.register(rootTransport, clientTransportA, clientTransportB);

        var rootServer = new ModelParameterServer(rootTransport, true);
        var clientServerA = new ModelParameterServer(clientTransportA, false);
        var clientServerB = new ModelParameterServer(clientTransportB, false);
        rootServer.launch();
        clientServerA.launch();
        clientServerB.launch();

        var servers = new ArrayList<ModelParameterServer>();
        var transports = new ArrayList<DelayedDummyTransport>();
        for (int e = 0; e < 128; e++) {
            var clientTransport = new DelayedDummyTransport(String.valueOf(e), connector, rootId, conf);
            var clientServer = new ModelParameterServer(clientTransport, false);

            connector.register(clientTransport);
            servers.add(clientServer);
            transports.add(clientTransport);

            clientServer.launch();

            //log.info("Server [{}] started...", e);
        }
        connector.blockUntilFinished();

        // 259 == 256 + A+B+R
        assertEquals(servers.size() + 3, rootTransport.getMesh().totalNodes());

        clientServerA.sendUpdate(array);

        connector.blockUntilFinished();

        var updatesR = rootServer.getUpdates();
        var updatesA = clientServerA.getUpdates();
        var updatesB = clientServerB.getUpdates();

        assertEquals(1, updatesR.size());
        assertEquals(1, updatesB.size());

        // we should NOT get this message back to A
        assertEquals(0, updatesA.size());

        for (int e = 0; e < servers.size(); e++) {
            var s = servers.get(e);
            assertEquals(1, s.getUpdates().size(),"Failed at node [" + e + "]");
        }
    }

    @Test()
    @Timeout(180000L)
    public void testModelAndUpdaterParamsUpdate_1() throws Exception {
        var config = VoidConfiguration.builder().meshBuildMode(MeshBuildMode.PLAIN).build();
        var connector = new DummyTransport.Connector();
        var rootTransport = new DelayedDummyTransport(rootId, connector, rootId, config);
        rootTransport.addRequestConsumer(ModelParametersRequest.class, new Consumer<ModelParametersRequest>() {
            @Override
            public void accept(ModelParametersRequest modelParametersRequest) throws Exception {
                var msg = new ModelParametersMessage("123", Nd4j.create(10));
                msg.setRequestId(modelParametersRequest.getRequestId());
                rootTransport.sendMessage(msg, modelParametersRequest.getOriginatorId());
            }
        });

        rootTransport.addRequestConsumer(UpdaterParametersRequest.class, new Consumer<UpdaterParametersRequest>() {
            @Override
            public void accept(UpdaterParametersRequest updatersParametersRequest) throws Exception {
                var msg = new UpdaterParametersMessage("123", Nd4j.create(10));
                msg.setRequestId(updatersParametersRequest.getRequestId());
                rootTransport.sendMessage(msg, updatersParametersRequest.getOriginatorId());
            }
        });


        var updatedModel = new AtomicBoolean(false);
        var updatedUpdater = new AtomicBoolean(false);
        var gotGradients = new AtomicBoolean(false);

        connector.register(rootTransport);

        var counters = new AtomicInteger[128];
        var servers = new ArrayList<ModelParameterServer>();
        var transports = new ArrayList<DummyTransport>();
        for (int e = 0; e < 128; e++) {
            var clientTransport = new DelayedDummyTransport(java.util.UUID.randomUUID().toString(), connector, rootId, config);
            var clientServer = new ModelParameterServer(config, clientTransport, false);

            counters[e] = new AtomicInteger(0);

            var f = e;

            clientServer.addUpdatesSubscriber(new AbstractUpdatesHandler() {
                @Override
                public INDArray getParametersArray() {
                    return null;
                }

                @Override
                public void onNext(INDArray array) {
                    assertNotNull(array);
                    counters[f].incrementAndGet();
                }
            });

            servers.add(clientServer);
            transports.add(clientTransport);

            connector.register(clientTransport);

            clientServer.launch();
            //log.info("Client [{}] started", e );
        }

        Thread.sleep(100);
        var rootMesh = rootTransport.getMesh();

        // now we're picking one server that'll play bad role
        var badServer = servers.get(23);
        var badTransport = transports.get(23);
        var badId = badTransport.id();
        var badNode = rootMesh.getNodeById(badId);

        var upstreamId = badNode.getUpstreamNode().getId();
        log.info("Upstream: [{}]; Number of downstreams: [{}]", upstreamId, badNode.numberOfDownstreams());

        connector.dropConnection(badId);
        var clientTransport = new DummyTransport(badId, connector, rootId);
        var clientServer = new ModelParameterServer(clientTransport, false);

        clientServer.addUpdaterParamsSubscriber(new AbstractSubscriber<INDArray>() {
            @Override
            public void onNext(INDArray array) {
                assertNotNull(array);
                updatedUpdater.set(true);
            }
        });

        clientServer.addModelParamsSubscriber(new AbstractSubscriber<INDArray>() {
            @Override
            public void onNext(INDArray array) {
                assertNotNull(array);
                updatedModel.set(true);
            }
        });

        clientServer.addUpdatesSubscriber(new AbstractUpdatesHandler() {
            @Override
            public INDArray getParametersArray() {
                return null;
            }

            @Override
            public void onNext(INDArray array) {
                assertNotNull(array);
                assertEquals(Nd4j.linspace(1, 10, 100).reshape(10, 10), array);
                gotGradients.set(true);
            }
        });

        connector.register(clientTransport);

        clientServer.launch();

        connector.blockUntilFinished();

        // getting any server
        var serv = servers.get(96);
        serv.sendUpdate(Nd4j.linspace(1, 10, 100).reshape(10, 10));

        connector.blockUntilFinished();

        for (int e = 0; e < 128; e++) {
            // we're skipping node 23 since it was reconnected, and has different MPS instance
            // and node 96, since it sends update
            if (e != 23 && e != 96)
                assertEquals(1, counters[e].get(),"Failed at node: [" + e + "]");
        }

        assertTrue(updatedModel.get());
        assertTrue(updatedUpdater.get());
        assertTrue(gotGradients.get());
    }

    @Test()
    @Timeout(180000L)
    public void testMeshConsistency_1() throws Exception {
        Nd4j.create(1);
        final int numMessages = 500;
        var rootCount = new AtomicInteger(0);
        var rootSum = new AtomicInteger(0);
        var counter = new AtomicInteger(0);
        var sum = new AtomicInteger(0);
        var config = VoidConfiguration.builder().meshBuildMode(MeshBuildMode.PLAIN).build();
        var connector = new DummyTransport.Connector();
        var rootTransport = new DelayedDummyTransport(rootId, connector, rootId, config);

        rootTransport.addPrecursor(GradientsUpdateMessage.class, new MessageCallable<GradientsUpdateMessage>() {
            @Override
            public void apply(GradientsUpdateMessage message) {
                var array = message.getPayload();
                rootSum.addAndGet(array.meanNumber().intValue());
                rootCount.incrementAndGet();
            }
        });
        connector.register(rootTransport);

        var counters = new AtomicInteger[16];
        var servers = new ArrayList<ModelParameterServer>();
        var transports = new ArrayList<DummyTransport>();
        for (int e = 0; e < 16; e++) {
            var clientTransport = new DelayedDummyTransport(java.util.UUID.randomUUID().toString(), connector, rootId, config);
            var clientServer = new ModelParameterServer(config, clientTransport, false);

            var f = e;
            counters[f] = new AtomicInteger(0);
            clientServer.addUpdatesSubscriber(new AbstractUpdatesHandler() {
                @Override
                public INDArray getParametersArray() {
                    return null;
                }

                @Override
                public void onNext(INDArray array) {
                    assertNotNull(array);
                    counters[f].incrementAndGet();
                }
            });

            servers.add(clientServer);
            transports.add(clientTransport);

            connector.register(clientTransport);

            clientServer.launch();
            //log.info("Client [{}] started", e );
        }


        var deductions = new int[servers.size()];
        for (int e = 0; e < numMessages; e++) {
            var f = RandomUtils.nextInt(0, servers.size());
            var server = servers.get(f);

            // later we'll reduce this number from expected number of updates
            deductions[f]++;

            server.sendUpdate(Nd4j.create(5).assign(e));
            sum.addAndGet(e);
        }

        connector.blockUntilFinished();

        // checking if master node got all updates we've sent
        assertEquals(numMessages, rootCount.get());
        assertEquals(sum.get(), rootSum.get());

        // now we're checking all nodes, they should get numMessages - messages that were sent through them
        for (int e = 0; e < servers.size(); e++) {
            var server = servers.get(e);
            assertEquals(numMessages - deductions[e], counters[e].get(),"Failed at node: [" + e + "]");
        }
    }


    @Test()
    @Timeout(180000L)
    public void testMeshConsistency_2() throws Exception {
        Nd4j.create(1);
        final int numMessages = 100;
        var rootCount = new AtomicInteger(0);
        var rootSum = new AtomicInteger(0);
        var counter = new AtomicInteger(0);
        var sum = new AtomicInteger(0);
        var config = VoidConfiguration.builder().meshBuildMode(MeshBuildMode.MESH).build();
        var connector = new DummyTransport.Connector();
        var rootTransport = new DelayedDummyTransport(rootId, connector, rootId, config);

        rootTransport.addPrecursor(GradientsUpdateMessage.class, new MessageCallable<GradientsUpdateMessage>() {
            @Override
            public void apply(GradientsUpdateMessage message) {
                var array = message.getPayload();
                rootSum.addAndGet(array.meanNumber().intValue());
                rootCount.incrementAndGet();
            }
        });
        connector.register(rootTransport);

        var counters = new AtomicInteger[16];
        var servers = new ArrayList<ModelParameterServer>();
        var transports = new ArrayList<DummyTransport>();
        for (int e = 0; e < 16; e++) {
            var clientTransport = new DelayedDummyTransport(java.util.UUID.randomUUID().toString(), connector, rootId, config);
            var clientServer = new ModelParameterServer(config, clientTransport, false);

            var f = e;
            counters[f] = new AtomicInteger(0);
            clientServer.addUpdatesSubscriber(new AbstractUpdatesHandler() {
                @Override
                public INDArray getParametersArray() {
                    return null;
                }

                @Override
                public void onNext(INDArray array) {
                    assertNotNull(array);
                    counters[f].incrementAndGet();
                }
            });

            servers.add(clientServer);
            transports.add(clientTransport);

            connector.register(clientTransport);

            clientServer.launch();
            //log.info("Client [{}] started", e );
        }

        Thread.sleep(500);


        var deductions = new int[servers.size()];
        for (int e = 0; e < numMessages; e++) {
            var f = RandomUtils.nextInt(0, servers.size());
            var server = servers.get(f);

            // later we'll reduce this number from expected number of updates
            deductions[f]++;

            server.sendUpdate(Nd4j.create(5).assign(e));
            sum.addAndGet(e);
        }

        connector.blockUntilFinished();

        //Thread.sleep(1000);
        //Thread.sleep(3000000000000L);

        // checking if master node got all updates we've sent
        assertEquals(numMessages, rootCount.get());
        assertEquals(sum.get(), rootSum.get());

        // now we're checking all nodes, they should get numMessages - messages that were sent through them
        for (int e = 0; e < servers.size(); e++) {
            var server = servers.get(e);
            assertEquals( numMessages - deductions[e], counters[e].get(),"Failed at node: [" + e + "]");
        }
    }
}
