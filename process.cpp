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

void print_process(Process process) {
    printf("pid: %d\t", process.pid);
    //print remoaining bursts, cpu bursts, io bursts, in the same line
    // printf("remaining_bursts: %d\t", process.remaining_bursts);
    // printf("executed_cpu_bursts: %d\t", process.executed_cpu_bursts);
    // printf("executed_io_bursts: %d\t", process.executed_io_bursts);
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
}

int get_remaining_bursts(Process process) {
    return process.executed_cpu_bursts + process.executed_io_bursts;
}

void calculate_estimated_bursts(Process* process, float alpha) {
    if (alpha == 1) {
        process->estimated_cpu_burst_time = process->cpu_bursts[0];
    } 
    else {
        if (process->estimated_bursts.size() == 0) {
            process->estimated_bursts.push_back(process->cpu_bursts[0]);
        } else {
            process->estimated_bursts.push_back(alpha * process->cpu_bursts[process->estimated_bursts.size() - 1] + 
                                                (1 - alpha) * process->estimated_bursts[process->estimated_bursts.size() - 1]);
        }
        process->estimated_cpu_burst_time = process->estimated_bursts[process->estimated_bursts.size() - 1];
    }
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
