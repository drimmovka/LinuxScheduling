#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>

int main() {
    printf("MIN: [%d] | MAX: [%d]\n", sched_get_priority_min(SCHED_RR), sched_get_priority_max(SCHED_RR));
}