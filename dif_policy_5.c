#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <sys/syscall.h>

#define THREAD_CNT 3

void getPOL() {
    int tid = syscall(SYS_gettid);
    char command[256];
    snprintf(command, sizeof(command), "chrt -p %d", tid);
    system(command);
}

void setPol(int sched_priority, int policy) {
    struct sched_param sched_param;
    sched_param.sched_priority = sched_priority;
    pthread_setschedparam(pthread_self(), policy, &sched_param);
    // sched_setscheduler(0, SCHED_RR, &sched_param); // another variant (0 for cur_thread)
}

void *foo(void *param) {
    int got_by = *((int *) param);
    
    // const PRI
    setPol(50, got_by%3);
    
    // getPOL();

    int _ = 0;
    time_t start = time(NULL);
    while (time(NULL) - start < 20 + (rand() % 40)) {
        _ ^= 1;
    }
    
    for (int i=0; i<1e6; ++i) {
        // printf("%d", got_by);
        // fflush(stdout);
    }

    // for ps to catch up
    // since I use threads and not processes, after finishing the work, 
    // the thread will be completed and the information about it will be lost to ps
    sleep(1);
}

int main() {
    srand(time(NULL));
    
    // seems like no need in here, bc main thread must not wait for other threads to complete
    // setPol(50);

    pthread_t *threads = (pthread_t *) calloc(THREAD_CNT, sizeof(pthread_t));

    for (int i=0; i<THREAD_CNT; ++i) {
        int *got_by = malloc(sizeof(int));
        *got_by = i;

        pthread_create(&threads[i], NULL, foo, (void *) got_by);
    }
    
    system("ps -L -o state,uid,ppid,pid,lwp,nlwp,pri,ni,policy,psr,comm > ps_tmp.log");

    for (int i=0; i<THREAD_CNT; ++i) {
        pthread_join(threads[i], NULL);
    }

}