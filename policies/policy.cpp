#include "thread_runner.cpp"

void testCFSEq() {
    ThreadRunner thread_runner({
        Param(SCHED_OTHER, 0, 0),
        Param(SCHED_OTHER, 0, 1),
        Param(SCHED_OTHER, 0, 2),
        Param(SCHED_OTHER, 0, 3),
        Param(SCHED_OTHER, 0, 4),
        Param(SCHED_OTHER, 0, 5),
        Param(SCHED_OTHER, 0, 6),
        Param(SCHED_OTHER, 0, 7),
        Param(SCHED_OTHER, 0, 8),
        Param(SCHED_OTHER, 0, 9),
    });
    thread_runner.run();
}

void testCFSDiff() {
    ThreadRunner thread_runner({
        Param(SCHED_OTHER, 20, 0),
        Param(SCHED_OTHER, 20, 1),
        Param(SCHED_OTHER, -19, 2),
        Param(SCHED_OTHER, 20, 3),
        Param(SCHED_OTHER, 20, 4),
        Param(SCHED_OTHER, -19, 5),
        Param(SCHED_OTHER, 0, 6),
        Param(SCHED_OTHER, -19, 7),
        Param(SCHED_OTHER, 20, 8),
        Param(SCHED_OTHER, 20, 9),
    });
    thread_runner.run();
}

void testFIFOEq() {
    ThreadRunner thread_runner({
        Param(SCHED_FIFO, 30, 0),
        Param(SCHED_FIFO, 30, 1),
        Param(SCHED_FIFO, 30, 2),
        Param(SCHED_FIFO, 30, 3),
        Param(SCHED_FIFO, 30, 4),
        Param(SCHED_FIFO, 30, 5),
        Param(SCHED_FIFO, 30, 6),
        Param(SCHED_FIFO, 30, 7),
        Param(SCHED_FIFO, 30, 8),
        Param(SCHED_FIFO, 30, 9),
    });
    thread_runner.run();
}

void testFIFODiff() {
    ThreadRunner thread_runner({
        Param(SCHED_FIFO, 30, 0),
        Param(SCHED_FIFO, 90, 1),
        Param(SCHED_FIFO, 30, 2),
        Param(SCHED_FIFO, 40, 3),
        Param(SCHED_FIFO, 40, 4),
        Param(SCHED_FIFO, 30, 5),
        Param(SCHED_FIFO, 30, 6),
        Param(SCHED_FIFO, 90, 7),
        Param(SCHED_FIFO, 90, 8),
        Param(SCHED_FIFO, 30, 9),
    });
    thread_runner.run();
}

void testRREq() {
    ThreadRunner thread_runner({
        Param(SCHED_RR, 30, 0),
        Param(SCHED_RR, 30, 1),
        Param(SCHED_RR, 30, 2),
        Param(SCHED_RR, 30, 3),
        Param(SCHED_RR, 30, 4),
        Param(SCHED_RR, 30, 5),
        Param(SCHED_RR, 30, 6),
        Param(SCHED_RR, 30, 7),
        Param(SCHED_RR, 30, 8),
        Param(SCHED_RR, 30, 9),
    });
    thread_runner.run();
}

void testRRDiff() {
    ThreadRunner thread_runner({
        Param(SCHED_RR, 30, 0),
        Param(SCHED_RR, 90, 1),
        Param(SCHED_RR, 30, 2),
        Param(SCHED_RR, 40, 3),
        Param(SCHED_RR, 40, 4),
        Param(SCHED_RR, 30, 5),
        Param(SCHED_RR, 30, 6),
        Param(SCHED_RR, 90, 7),
        Param(SCHED_RR, 90, 8),
        Param(SCHED_RR, 30, 9),
    });
    thread_runner.run();
}

void testMixed() {
    ThreadRunner thread_runner({
        Param(SCHED_OTHER, 0, 0),
        Param(SCHED_FIFO, 30, 1),
        Param(SCHED_OTHER, 0, 2),
        Param(SCHED_OTHER, 0, 3),
        Param(SCHED_FIFO, 30, 4),
        Param(SCHED_FIFO, 30, 5),
        Param(SCHED_FIFO, 30, 6),
        Param(SCHED_OTHER, 0, 7),
        Param(SCHED_FIFO, 30, 8),
        Param(SCHED_FIFO, 30, 9),
    });
    thread_runner.run();
}


int main(int argc, char **argv) {
    // testCFSEq();
    // testCFSDiff();
    // testFIFOEq();
    // testFIFODiff();
    // testRREq();
    // testRRDiff();
    // testMixed();
}