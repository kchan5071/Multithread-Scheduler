#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <string>
#include <vector>
#include "process.h"
#include "sorter.h"
#include "threaded.h"
extern "C" {
    #include "log.h"
}

void* run_scheduler(void *ptr);

bool all_bursts_completed(Process process);

std::vector<Process> create_processes(std::vector< std::vector<int> > lines);

Process create_process(int pid, std::vector<int> bursts);

std::vector<Process> sort_processes(std::vector<Process> processes);

bool all_processes_finished(std::vector<Process> ready_queue , std::vector<Process> blocked_queue);

struct SchedulerArgs {
    std::vector< std::vector<int> > lines;
    bool exponential;
    float option_argument;
    bool running;
};

#endif