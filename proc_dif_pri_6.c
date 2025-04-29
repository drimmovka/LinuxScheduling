#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sched.h>
#include <wait.h>
#include <time.h>

#define PROC_CNT 10

void setPol(int sched_priority) {
    struct sched_param sched_pram;
    sched_pram.sched_priority = sched_priority;
    sched_setscheduler(getpid(), SCHED_RR, &sched_pram);
}

int main() {
    for (int i=0; i<PROC_CNT; ++i) {
        pid_t pid = fork();
        if (pid == 0) {
            // child
            if (i == 5 || i == 8) setPol(70);
            else setPol(50);

            sleep(1);
            
            int _ = 0;
            time_t start = time(NULL);
            while (time(NULL) - start < 10) {
                _ ^= 1;
            }

            for (int j=0; j<1e5; ++j) {
                // printf("%d", j);
                // fflush(stdout);
            }

            printf("PID: [%d] | DONE\n", getpid());
            exit(0);
        } else {
            // father
        }
    }

    system("ps -o state,uid,ppid,pid,pri,ni,policy,psr,comm > ps_tmp.log");

    for (int i=0; i<PROC_CNT; ++i) {
        waitpid(-1, NULL, 0);
    }
}