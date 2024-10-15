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

//Kai Chan
//827673009


void* run_scheduler(void *ptr);

bool all_bursts_completed(Process process);

std::vector<Process> create_processes(std::vector< std::vector<int> > lines);

Process create_process(int pid, std::vector<int> bursts);

std::vector<Process> sort_processes(std::vector<Process> processes);

bool all_processes_finished(std::vector<Process> ready_queue , std::vector<Process> blocked_queue);

bool all_bursts_completed(Process process);

void initialize_queues(std::vector<Process> processes, std::vector<Process> &ready_queue, std::vector<Process> &blocked_queue);

void get_initial_estimation(Process &process);

void move_to_ready_from_blocked(Process &process, std::vector<Process> &ready_queue, std::vector<Process> &blocked_queue);

void move_to_blocked_from_ready(Process &process, std::vector<Process> &ready_queue, std::vector<Process> &blocked_queue);

void move_to_completed(Process &process, std::vector<Process> &completed_processes, std::vector<Process> &ready_queue);

struct SchedulerArgs {
    std::vector< std::vector<int> > lines;
    bool exponential;
    float option_argument;
    bool running;
};

#endif


