#include "process.h"

int get_next_burst(Process process) {
    int burst = 0;
    if (process.remaining_bursts % 2 == 0) {
        burst = process.io_bursts[0];
    } else {
        burst = process.cpu_bursts[0];
    }
    return burst;
}

int get_next_io_burst(Process process) {
    if (process.executed_io_bursts < process.io_bursts.size()) {
        return process.io_bursts[process.executed_io_bursts];
    }
    return 0; // No more IO bursts
}

void print_estimated_bursts_process(Process process) {
    printf("Estimated Bursts: ");
    for (int i = 0; i < process.estimated_bursts.size(); i++) {
        printf("%f\t", process.estimated_bursts[i]);
    }
    printf("\n");
}

std::vector<float> convert_to_float_array(Process process) {
    //return a mix between cpu and io bursts
    std::vector<float> estimated_bursts = std::vector<float>();
    for (int i = 0; i < process.cpu_bursts.size(); i++) {
        estimated_bursts.push_back(process.cpu_bursts[i]);
        if (i < process.io_bursts.size()) {
            estimated_bursts.push_back(process.io_bursts[i]);
        }
    }
    return estimated_bursts;
}

void print_process(Process process) {
    printf("pid: %d\t", process.pid);
    printf("cpu_bursts: ");
    for (int i = 0; i < process.cpu_bursts.size(); i++) {
        printf("%d ", process.cpu_bursts[i]);
    }
    printf("\tio_bursts: ");
    for (int i = 0; i < process.io_bursts.size(); i++) {
        printf("%d ", process.io_bursts[i]);
    }
    printf("Estimated CPU Burst Time: %f\n", process.estimated_cpu_burst_time);
    printf("\n");
    print_estimated_bursts_process(process);
    printf("\n");
}

int get_remaining_bursts(Process process) {
    return process.executed_cpu_bursts + process.executed_io_bursts;
}

void calculate_estimated_bursts(Process* process, float alpha, int new_time) {
    float estimated_burst = 0;
    if (process->estimated_bursts.size() == 0) {
        estimated_burst = process->estimated_cpu_burst_time;
    } 
    else {
        estimated_burst = alpha * new_time + (1 - alpha) * process->estimated_bursts[process->estimated_bursts.size() - 1];
    }
    process->estimated_bursts.push_back(estimated_burst);
}

void* remove_cpu_burst(void* ptr) {
    Process* process = static_cast<Process*>(ptr);
    process->cpu_bursts.erase(process->cpu_bursts.begin());
    process->executed_cpu_bursts++;
    process->remaining_bursts--;
    process->executing = false;
    return NULL;
}

void* remove_io_burst(void* ptr) {
    Process* process = static_cast<Process*>(ptr);
    process->io_bursts.erase(process->io_bursts.begin());
    process->executed_io_bursts++;
    process->remaining_bursts--;
    process->executing = false;
    return NULL;
}


void* run_cpu_burst(void* ptr) {
    Process* process = static_cast<Process*>(ptr);
    process->cpu_bursts[0]--;
    process->completion_time++;
    process->cpu_time++;
    process->executing = false;
    return NULL;
}

void* run_io_burst(void* ptr) {
    Process* process = static_cast<Process*>(ptr);
    process->io_bursts[0]--;
    process->io_time++;
    process->completion_time++;
    process->executing = false;
    return NULL;
}
