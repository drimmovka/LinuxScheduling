#include <vector>
#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <sched.h>
#include <sys/resource.h>
#include <sys/syscall.h>

using Policy = int;


class Param {
public:
    explicit Param(Policy pol, int pri, int id) 
        : _pol(pol)
        , _id(id)
    {
        if (pol == SCHED_FIFO || pol == SCHED_RR) {
            _pri.ni = std::max(1, std::min(99, pri));
        } else if (pol == SCHED_OTHER) {
            _pri.ni = std::max(-20, std::min(19, pri));
        } else {
            throw std::invalid_argument("Invalid policy");
        }
    }

    bool rt() { return (_pol == SCHED_FIFO || _pol == SCHED_RR); }

    Policy pol() { return _pol; }

    int pri() { return (rt() ? _pri.rtprio : _pri.ni); }

    int id() {return _id; }

private:
    Policy _pol;
    union {
        int rtprio;
        int ni;
    } _pri;
    int _id;
};

class ThreadRunner {
public:
    explicit ThreadRunner(std::vector<Param>&& params)
        : _threads(params.size())
        , _params(std::move(params))
    {
        system("> race.log");
    }

    ~ThreadRunner() = default;
    
    void run() {
        for (int i=0; i<_threads.size(); ++i) {
            pthread_create(&_threads[i], NULL, foo, (void *) &_params[i]);
        }
        
        system("ps -L -o pid,lwp,nlwp,rtprio,ni,policy,psr,comm > ps.log");

        for (int i=0; i<_threads.size(); ++i) {
            pthread_join(_threads[i], NULL);
        }
    
    }

private:
    std::vector<pthread_t> _threads;
    std::vector<Param> _params;

private:
    static void *foo(void *param) {
        setParam((Param *) param);
        int id = ((Param *) param)->id();
        
        // sleep() here added for testFIFODiff and testMixed (bc FF and RR are cooperative policies we need sleep,
        // so that the threads can wait for each other to set priorities.)
        sleep(1);
        

        // routine for SCHED_OTHER and SCHED_FIFO policies
        if (((Param *) param)->pol() == SCHED_OTHER || ((Param *) param)->pol() == SCHED_FIFO) {
            for (int i=0; i<1e5; ++i) {
                printf("%d|", id);
                fflush(stdout);
            }
        }

        // routine for SCHED_RR policy
        if (((Param *) param)->pol() == SCHED_RR) {
            for (int i=0; i<5; ++i) {
                int _ = 0;
                time_t start = time(NULL);
                while (time(NULL) - start < 2) {
                    _ ^= 1;
                    
                    // for experiment 7
                    // set iterations count to low value!!!
                    // sched_yield();
                }
            }
        }
        
        reportCompletion(id);

        // for ps to catch up
        // since I use threads and not processes, after finishing the work, 
        // the thread will be completed and the information about it will be lost to ps
        sleep(1);

        return nullptr;
    }

    static void setParam(Param *param) {
        if (param->rt()) {
            struct sched_param sched_param;
            sched_param.sched_priority = param->pri();    
            pthread_setschedparam(pthread_self(), param->pol(), &sched_param);
        } else {
            setpriority(PRIO_PROCESS, 0, param->pri());
        }
    }
    
    static void reportCompletion(int id) {
        std::string cmd = "echo done thread with id: " + std::to_string(id) + " >> race.log";
        system(cmd.c_str());
    }
  
};