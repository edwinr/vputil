#include "scheduler.h"
#include <cassert>
#include <iostream>
#include <algorithm>

using std::chrono::milliseconds;
using std::chrono::duration_cast;
using std::is_sorted;
using std::upper_bound;

namespace vputil {

Scheduler::Scheduler(vp_instance_t vpInstance)
 : vpInstance(vpInstance),running(false) { }

void Scheduler::run() {
    running = true;
    while (running) {
        vp_wait(vpInstance, std::max(timeUntilNextTask().count(), 0LL));
        
        auto now = Clock::now();
        while (!tasks.empty()) {
            auto& task = tasks.front();
            if (task.when > now) {
                break;
            }
            task.handler();
            tasks.pop_front();
        }
    }
}

milliseconds Scheduler::timeUntilNextTask() const {
    auto result = milliseconds(10000);
    if (!tasks.empty()) {
        auto& task = tasks.front();
        result = duration_cast<milliseconds>(task.when - Clock::now());
    }
    return result;
}

void Scheduler::schedule(milliseconds delay, HandlerFunction handler) {
    scheduleAt(Clock::now() + delay, handler);
}

void Scheduler::scheduleAt(std::chrono::time_point<Clock> when, HandlerFunction handler) {
    Task task;
    task.when = when;
    task.handler = handler;
    insertTask(task);
}

bool Scheduler::Task::operator<(const Task& other) const {
    return when < other.when;
}

void Scheduler::insertTask(const Task& task) {
    auto position = upper_bound(tasks.begin(), tasks.end(), task);
    tasks.insert(position, task);
    assert(is_sorted(tasks.begin(), tasks.end()));
}

} //namespace vputil
