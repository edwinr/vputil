#ifndef VPUTIL_SCHEDULER_H__
#define VPUTIL_SCHEDULER_H__

#include <chrono>
#include <deque>
#include <functional>
#include <VPSDK/VP.h>

namespace vputil {

/**
 *  Task scheduler that runs the VP SDK message loop when idle
 */
class Scheduler {
    typedef std::function<void()> HandlerFunction;
    struct Task {
        std::chrono::time_point<std::chrono::system_clock> when;
        std::function<void()> handler;
        
        bool operator<(const Task& other) const;
    };
    
    vp_instance_t vpInstance;
    std::deque<Task> tasks;
    bool running;
    
    std::chrono::milliseconds timeUntilNextTask() const;
    void insertTask(const Task& task);
public:
    Scheduler(vp_instance_t vpInstance);
    
    void run();
    void schedule(std::chrono::milliseconds delay, HandlerFunction handler);
                  
};
} //namespace vputil

#endif
