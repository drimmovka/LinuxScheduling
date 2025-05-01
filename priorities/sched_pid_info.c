#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <linux/sched.h>
#include <sched.h>
#include <sys/resource.h>
#include <syscall.h>

void printSchedulingInfo(pid_t pid);
const char* getPolicyName(int policy);
void printPriorityInfo(pid_t pid, struct sched_param param);

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <PID>\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    pid_t pid = atoi(argv[1]);
    printSchedulingInfo(pid);

    return EXIT_SUCCESS;
}

void printSchedulingInfo(pid_t pid) {
    int policy = sched_getscheduler(pid);
    if (policy == -1) {
        perror("sched_getscheduler");
        exit(EXIT_FAILURE);
    }

    struct sched_param param;
    if (sched_getparam(pid, &param) == -1) {
        perror("sched_getparam");
        exit(EXIT_FAILURE);
    }

    printf("PID: %d\n", pid);
    printf("Policy: %s\n", getPolicyName(policy));
    printPriorityInfo(pid, param);
}

const char* getPolicyName(int policy) {
    switch (policy) {
        case SCHED_OTHER:    return "SCHED_OTHER";
        case SCHED_FIFO:     return "SCHED_FIFO";
        case SCHED_RR:       return "SCHED_RR";
        case SCHED_BATCH:    return "SCHED_BATCH";
        case SCHED_IDLE:     return "SCHED_IDLE";
        case SCHED_DEADLINE: return "SCHED_DEADLINE";
        default:             return "UNKNOWN";
    }
}

void printPriorityInfo(pid_t pid, struct sched_param param) {
    printf("RTPRIO: %d\n", param.sched_priority);
    printf("NI: %d\n", getpriority(PRIO_PROCESS, pid));
    
}