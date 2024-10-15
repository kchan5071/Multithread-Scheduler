#include "sorter.h"

bool compare_ready_processes(const Process &a, const Process &b) {
    return a.estimated_cpu_burst_time < b.estimated_cpu_burst_time;
}

bool compare_blocked_processes(const Process &a, const Process &b) {
    return a.io_bursts[0] < b.io_bursts[0];
}

bool compare_completion_times(const Process &a, const Process &b) {
    return a.completion_time < b.completion_time;
}

void sort_ready_queue(std::vector<Process>& processes) {
    if (processes.size() <= 1) {
        return;
    }
    std::stable_sort(processes.begin(), processes.end(), compare_ready_processes);
}

void sort_completed_processes(std::vector<Process> &processes) {
    if (processes.size() <= 1) {
        return;
    }
    std::stable_sort(processes.begin(), processes.end(), compare_completion_times);
}

void sort_blocked_queue(std::vector<Process> &processes) {
    if (processes.size() <= 1) {
        return;
    }
    std::stable_sort(processes.begin(), processes.end(), compare_blocked_processes);
}