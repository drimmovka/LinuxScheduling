#include <sched.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <syscall.h>


int main() {
    
    system("ps -o uid,pid,pri,ni,policy,comm");


    pid_t pid = getpid();
    int shed = sched_getscheduler(pid);

    struct sched_param *param;
    sched_getparam(pid, param);

    printf("PID: [%d]; POL: ", pid);
    switch (shed) {
        case SCHED_OTHER: printf("[SCHED_OTHER]; PRI: [%d]\n", param->sched_priority); break;
        case SCHED_FIFO: printf("SCHED_FIFO; PRI: [%d] \n", param->sched_priority); break;
        case SCHED_RR: printf("SCHED_RR; PRI: [%d] \n", param->sched_priority); break;
        default: printf("unknown\n"); break;
    }

    int row_pri = syscall(SYS_getpriority, PRIO_PROCESS, pid);
    int nice = getpriority(PRIO_PROCESS, pid);
    printf("ROW PRI: [%d]\n", row_pri);
    printf("NICE: [%d]\n", nice);
    printf("PS PRI MUST BE: 20 + (NICE) = 20 - %d ≈ [%d]\n", nice, 20 - nice);
}