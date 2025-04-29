#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define THREAD_CNT 10

void *foo(void *param) {
    sleep(1);
    int got_by = *((int *) param);
    for (int i=0; i<1e5; ++i) {
        printf("%d", got_by);
        fflush(stdout);
    }
}

int main() {
    pthread_t *threads = (pthread_t *) calloc(THREAD_CNT, sizeof(pthread_t));

    for (int i=0; i<THREAD_CNT; ++i) {
        int *got_by = malloc(sizeof(int));
        *got_by = i;
        pthread_create(&threads[i], NULL, foo, (void *) got_by);
    }
    
    
    system("ps  -L -o uid,pid,lwp,nlwp,pri,ni,policy,psr,comm > ps_tmp.log");

    for (int i=0; i<THREAD_CNT; ++i) {
        pthread_join(threads[i], NULL);
    }
}