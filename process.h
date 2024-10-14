#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <vector>
#include <queue>
#include <string>

struct Process {
    int pid;
    int remaining_bursts;
    int executed_cpu_bursts;
    int executed_io_bursts;
    int completion_time;

    
    std::vector<int> cpu_bursts;
    std::vector<int> io_bursts;
};

int get_next_burst(Process process);

int get_next_io_burst(Process process);

int get_remaining_bursts(Process process);

void print_process(Process process);

void* run_cpu_burst(void* ptr);

void* run_io_burst(void* ptr);





#endif