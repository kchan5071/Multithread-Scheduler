#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <vector>
#include <queue>
#include <string>

//Kai Chan
//827673009

struct Process {
    int pid;
    int remaining_bursts;
    int executed_cpu_bursts;
    int executed_io_bursts;
    int completion_time;
    int wait_time;
    bool executing;
    int io_time;
    int cpu_time;
    float estimated_cpu_burst_time;
    float previous_burst;
    float previous_prediction;
    std::vector<int> burst_times;

    std::vector<float> all_burst_times;
    std::vector<float> cpu_burst_estimates;
    std::vector<int> cpu_bursts;
    std::vector<int> io_bursts;
};

float calculate_exponential_averaging(Process* process, float alpha, int new_time, bool exponential);

void* remove_cpu_burst(void* ptr);

void* remove_io_burst(void* ptr);

float get_average_burst_time(Process process);

int get_remaining_bursts(Process process);

std::vector<float> convert_to_float_array(Process process);

void* run_cpu_burst(void* ptr);

void* run_io_burst(void* ptr);

Process create_process(int pid, std::vector<int> bursts);



#endif