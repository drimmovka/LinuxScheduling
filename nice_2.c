#include <sys/time.h>
#include <sys/resource.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    struct timeval stop, start;
    gettimeofday(&start, NULL);
    
    long long int i = 0;
    while (i < 1000000000LL) {
        i++;
    }
    
    gettimeofday(&stop, NULL);
    long long diff_seconds = stop.tv_sec - start.tv_sec;
    long long diff_useconds = stop.tv_usec - start.tv_usec;
    long long total_us = diff_seconds * 1000000LL + diff_useconds;
    
    double num_seconds = (double)total_us / 1000000.0;
    int pid = getpid();
    int nice = getpriority(PRIO_PROCESS, 0);
    
    char *arg = (argc > 1) ? argv[1] : "(no argument)"; 
    
    printf("PID: [%d]; NI: [%d] | index=%s, finished in %.02f sec\n", pid, nice, arg, num_seconds);
}