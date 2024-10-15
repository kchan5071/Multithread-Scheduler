#include "process.h"

//Kai Chan
//827673009

/**
 * Get the remaining number of bursts for a process
 * @param process the process
 * @return the number of remaining bursts
 */
int get_remaining_bursts(Process process) {
    return process.executed_cpu_bursts + process.executed_io_bursts;
}

/**
 * Get the average burst time for a process
 * @param process the process
 * @return the average burst time
 */
float get_average_burst_time(Process process) {
    float sum = 0;
    for (int i = 0; i < process.cpu_bursts.size(); i++) {
        sum += process.cpu_bursts[i];
    }
    return sum / process.cpu_bursts.size();
}

/**
 * Convert a process to a float array
 * @param process the process
 * @return the float array
 */
float calculate_exponential_averaging(Process* process, float alpha, int new_time, bool exponential) {
    float estimated_burst = 0;
    //calculate estimated burst time
    if (exponential) {
        estimated_burst = alpha * process->previous_burst + (1 - alpha) * process->previous_prediction;
    } else {
        estimated_burst = new_time;
    }
    //update process
    process->previous_burst = new_time;
    process->previous_prediction = estimated_burst;
    process->estimated_cpu_burst_time = estimated_burst;
    return estimated_burst;
}

/**
 * THREADED
 * Remove a cpu burst from a process
 * @param ptr the process to remove the burst from
 * @return NULL
 */
void* remove_cpu_burst(void* ptr) {
    //remove burst
    Process* process = static_cast<Process*>(ptr);
    process->cpu_bursts.erase(process->cpu_bursts.begin());
    //update process
    process->executed_cpu_bursts++;
    process->remaining_bursts--;
    process->executing = false;
    return NULL;
}

/**
 * THREADED
 * Remove an io burst from a process
 * @param ptr the process to remove the burst from
 * @return NULL
 */
void* remove_io_burst(void* ptr) {
    //remove burst
    Process* process = static_cast<Process*>(ptr);
    process->io_bursts.erase(process->io_bursts.begin());
    //update process
    process->executed_io_bursts++;
    process->remaining_bursts--;
    process->executing = false;
    return NULL;
}


/**
 * THREADED
 * Run a cpu burst for a process
 * @param ptr the process to run the burst for
 * @return NULL
 */
void* run_cpu_burst(void* ptr) {
    //run burst
    Process* process = static_cast<Process*>(ptr);
    process->cpu_bursts[0]--;
    process->completion_time++;
    process->cpu_time++;
    process->executing = false;
    return NULL;
}

/**
 * THREADED
 * Run an io burst for a process
 * @param ptr the process to run the burst for
 * @return NULL
 */
void* run_io_burst(void* ptr) {
    //run burst
    Process* process = static_cast<Process*>(ptr);
    process->io_bursts[0]--;
    process->io_time++;
    process->completion_time++;
    process->executing = false;
    return NULL;
}

/**
 * Create a process with the given pid and bursts
 * @param pid the process id
 * @param bursts the cpu and io bursts
 * @return the process
 */
Process create_process(int pid, std::vector<int> bursts) {
    //divide bursts into cpu and io
    std::vector<int> cpu_bursts = std::vector<int>();
    std::vector<int> io_bursts = std::vector<int>();
    for (int i = 0; i < bursts.size(); i++) {
        if (i % 2 == 0) {
            cpu_bursts.push_back(bursts[i]);
        } else {
            io_bursts.push_back(bursts[i]);
        }
    }

    //create process struct
    Process process;
    process.pid = pid;
    process.remaining_bursts = bursts.size();
    process.executed_cpu_bursts = cpu_bursts.size();
    process.executed_io_bursts = io_bursts.size();
    process.completion_time = 0;
    process.cpu_bursts = cpu_bursts;
    process.io_bursts = io_bursts;
    process.executing = false;
    process.io_time = 0;
    process.cpu_time = 0;
    process.wait_time = 0;
    process.previous_burst = 0;
    process.previous_prediction = 0;
    process.all_burst_times = std::vector<float>();

    return process;
}