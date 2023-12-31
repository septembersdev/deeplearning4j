open module nd4j.cuda {
    requires org.apache.commons.lang3;
    requires org.bytedeco.cuda;
    requires slf4j.api;
    requires flatbuffers.java;
    requires guava;
    requires nd4j.api;
    requires nd4j.common;
    requires nd4j.cpu.api;
    requires nd4j.cuda.preset;
    requires org.bytedeco.javacpp;
    exports org.nd4j.jita.allocator;
    exports org.nd4j.jita.allocator.concurrency;
    exports org.nd4j.jita.allocator.enums;
    exports org.nd4j.jita.allocator.garbage;
    exports org.nd4j.jita.allocator.impl;
    exports org.nd4j.jita.allocator.pointers;
    exports org.nd4j.jita.allocator.pointers.cuda;
    exports org.nd4j.jita.allocator.tad;
    exports org.nd4j.jita.allocator.time;
    exports org.nd4j.jita.allocator.time.impl;
    exports org.nd4j.jita.allocator.time.providers;
    exports org.nd4j.jita.allocator.time.rings;
    exports org.nd4j.jita.allocator.utils;
    exports org.nd4j.jita.balance;
    exports org.nd4j.jita.concurrency;
    exports org.nd4j.jita.conf;
    exports org.nd4j.jita.constant;
    exports org.nd4j.jita.flow;
    exports org.nd4j.jita.flow.impl;
    exports org.nd4j.jita.handler;
    exports org.nd4j.jita.handler.impl;
    exports org.nd4j.jita.memory;
    exports org.nd4j.jita.workspace;
    exports org.nd4j.jita.allocator;
    exports org.nd4j.linalg.jcublas;
    exports org.nd4j.linalg.jcublas.bindings;
    exports org.nd4j.linalg.jcublas.blas;
    exports org.nd4j.linalg.jcublas.buffer;
    exports org.nd4j.linalg.jcublas.buffer.factory;
    exports org.nd4j.linalg.jcublas.context;
    exports org.nd4j.linalg.jcublas.ops.executioner;
    exports org.nd4j.linalg.jcublas.ops.executioner.aggregates;
    exports org.nd4j.linalg.jcublas.rng;
    exports org.nd4j.linalg.jcublas.util;
    provides org.nd4j.linalg.factory.Nd4jBackend with org.nd4j.linalg.jcublas.JCublasBackend;
}
