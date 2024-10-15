#include "sorter.h"

//Kai Chan
//827673009

/**
 * Compare two processes by their estimated cpu burst time
 * @param a the first process
 * @param b the second process
 * @return true if a is less than b, false otherwise
 */
bool compare_ready_processes(const Process &a, const Process &b) {
    return a.estimated_cpu_burst_time < b.estimated_cpu_burst_time;
}

/**
 * Compare two processes by their io burst time
 * @param a the first process
 * @param b the second process
 * @return true if a is less than b, false otherwise
 */
bool compare_blocked_processes(const Process &a, const Process &b) {
    return a.io_bursts[0] < b.io_bursts[0];
}

/**
 * Compare two processes by their completion time
 * @param a the first process
 * @param b the second process
 * @return true if a is less than b, false otherwise
 */
bool compare_completion_times(const Process &a, const Process &b) {
    return a.completion_time < b.completion_time;
}

/**
 * Sort the ready queue by estimated cpu burst time
 * @param processes the ready queue
 */
void sort_ready_queue(std::vector<Process>& processes) {
    if (processes.size() <= 1) {
        return;
    }
    std::stable_sort(processes.begin(), processes.end(), compare_ready_processes);
}

/**
 * Sort the completed processes by completion time
 * @param processes the completed processes
 */
void sort_completed_processes(std::vector<Process> &processes) {
    if (processes.size() <= 1) {
        return;
    }
    std::stable_sort(processes.begin(), processes.end(), compare_completion_times);
}

/**
 * Sort the blocked queue by io burst time
 * @param processes the blocked queue
 */
void sort_blocked_queue(std::vector<Process> &processes) {
    if (processes.size() <= 1) {
        return;
    }
    std::stable_sort(processes.begin(), processes.end(), compare_blocked_processes);
}