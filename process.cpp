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
    printf("pid: %d\n", process.pid);
    printf("remaining_bursts: %d\n", process.remaining_bursts);
    printf("number_of_cpu_bursts: %d\n", process.executed_cpu_bursts);
    printf("number_of_io_bursts: %d\n", process.executed_io_bursts);
    printf("completion_time: %d\n", process.completion_time);
    printf("cpu_bursts: ");
    for (int i = 0; i < process.cpu_bursts.size(); i++) {
        printf("%d ", process.cpu_bursts[i]);
    }
    printf("\nio_bursts: ");
    for (int i = 0; i < process.io_bursts.size(); i++) {
        printf("%d ", process.io_bursts[i]);
    }
    printf("\n");
}

int get_remaining_bursts(Process process) {
    return process.executed_cpu_bursts + process.executed_io_bursts;
}

void* run_cpu_burst(void* ptr) {
    Process process = *((Process*)ptr);
    int burst = process.cpu_bursts[0];
    process.remaining_bursts--;
    process.completion_time += burst;
    //remove burst from cpu
    if (process.executed_cpu_bursts > 0) {
        process.cpu_bursts.erase(process.cpu_bursts.begin());
        process.executed_cpu_bursts--;
    }
    return NULL;
}

void* run_io_burst(void* ptr) {
    Process process = *((Process*)ptr);
    int burst_length = process.io_bursts[0];
    process.io_bursts[0] -= burst_length;
    process.completion_time += burst_length;
    //remove burst from io if it is finished
    if (process.io_bursts[0] <= 0) {
        process.io_bursts.erase(process.io_bursts.begin());
        process.executed_io_bursts--;
    }
    return NULL;
}
