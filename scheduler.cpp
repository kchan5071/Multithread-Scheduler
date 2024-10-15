#include "scheduler.h"
#include <iostream>
#include <unistd.h>

void initialize_queues(std::vector<Process> processes, std::vector<Process> &ready_queue, std::vector<Process> &blocked_queue) {
    for (int i = 0; i < processes.size(); i++) {
        ready_queue.push_back(processes[i]);
    }
}

void get_initial_estimation(Process &process) {
    //average of all cpu bursts
    float sum = 0;
    for (int i = 0; i < process.cpu_bursts.size(); i++) {
        sum += process.cpu_bursts[i];
    }
    process.estimated_cpu_burst_time = sum / process.cpu_bursts.size();
    process.previous_burst = process.cpu_bursts[0];
    process.previous_prediction = process.estimated_cpu_burst_time;
    process.all_burst_times.push_back(process.estimated_cpu_burst_time);
}

void move_to_completed(Process &process, std::vector<Process> &completed_processes, std::vector<Process> &ready_queue) {
    completed_processes.push_back(process);
    ready_queue.erase(ready_queue.begin());
}

void move_to_ready_from_blocked(Process &process, std::vector<Process> &ready_queue, std::vector<Process> &blocked_queue) {
    ready_queue.push_back(process);
    blocked_queue.erase(blocked_queue.begin());
}

void move_to_blocked_from_ready(Process &process, std::vector<Process> &ready_queue, std::vector<Process> &blocked_queue) {
    blocked_queue.push_back(process);
    ready_queue.erase(ready_queue.begin());
}

void print_estimated_bursts(std::vector<Process> processes) {
    for (int i = 0; i < processes.size(); i++) {
        printf("Process %d estimated burst time: %f\n", processes[i].pid, processes[i].estimated_cpu_burst_time);
    }
}

void log_processes(std::vector<Process> completed_processes) {
    sort_completed_processes(completed_processes);
    for (Process process : completed_processes) {
        log_process_completion(process.pid, process.completion_time, process.wait_time);
    }

    //log estimated bursts
    for (Process process : completed_processes) {
        log_process_estimated_bursts(process.pid, reinterpret_cast<float*>(process.all_burst_times.data()), (size_t)process.all_burst_times.size());
    }
}

void* run_scheduler(void *ptr) {
    // Get arguments
    int total_elapsed_time = 0;
    SchedulerArgs *args = (SchedulerArgs *) ptr;
    std::vector<std::vector<int>> lines = args->lines;
    bool exponential = args->exponential;
    float option_argument = args->option_argument;
    bool running = args->running;

    // Log process bursts
    for (int i = 0; i < lines.size(); i++) {
        log_process_bursts(i, reinterpret_cast<unsigned int*>(lines[i].data()), lines[i].size());
    }

    //initialize queues
    std::vector<Process> ready_queue = std::vector<Process>();
    std::vector<Process> blocked_queue = std::vector<Process>();
    std::vector<Process> completed_processes = std::vector<Process>();


    std::vector<Process> processes = create_processes(lines);

    initialize_queues(processes, ready_queue, blocked_queue);
    
    // Run scheduler
    int total_time = 1;

    if (exponential) {
        for (int i = 0; i < ready_queue.size(); i++) {
            get_initial_estimation(ready_queue[i]);
        }
    }
    else {
        for (int i = 0; i < ready_queue.size(); i++) {
            ready_queue[i].all_burst_times.push_back(ready_queue[i].cpu_bursts[0]);
        }
    }

    sort_ready_queue(ready_queue);
    while (!all_processes_finished(ready_queue, blocked_queue)) {
        sort_ready_queue(ready_queue);
        sort_blocked_queue(blocked_queue);

        //update blocked queue, subtract 1 from each process's io burst
        for (int i = 0; i < blocked_queue.size(); i++) {
            run_io_burst(blocked_queue[i]);
        }
        //update ready queue, subtract 1 from the first process's cpu burst
        if (ready_queue.size() != 0) {
            run_cpu_burst(ready_queue[0]);
        }

        //update wait time for all processes in ready queue
        if (ready_queue.size() > 1) {
            for (int i = 1; i < ready_queue.size(); i++) {
                ready_queue[i].wait_time++;
            }
        }

        //check if first process in ready queue has completed its cpu burst
        if (ready_queue.size() != 0 && ready_queue[0].cpu_bursts[0] == 0) {
            Process &process = ready_queue[0];
            remove_cpu_burst(process);
            //check if process has completed all bursts
            if (all_bursts_completed(process)) {
                process.completion_time = total_time;
                // printf("PROCESS %d COMPLETED\n", process.pid);
                log_cpuburst_execution(process.pid, process.cpu_time, process.io_time, total_time, COMPLETED);
                move_to_completed(process, completed_processes, ready_queue);
            }
            else {
                // printf("PROCESS %d ENTERING IO\n", process.pid);
                process.all_burst_times.push_back(process.io_bursts[0]);
                log_cpuburst_execution(process.pid, process.cpu_time, process.io_time, total_time, ENTER_IO);
                move_to_blocked_from_ready(process, ready_queue, blocked_queue);
            }
        }
        //check if any process in blocked queue has completed its io burst
        for (int i = 0; i < blocked_queue.size(); i++) {
            Process &process = blocked_queue[i];
            if (process.io_bursts.size() == 0 || process.io_bursts[0] == 0) {
                // printf("CALCULATING ESTIMATED BURSTS FOR PROCESS %d\n", process.pid);
                int current_cpu_time = process.cpu_bursts[0];
                float next_estimate = calculate_exponential_averaging(&process, option_argument, current_cpu_time, exponential);
                process.all_burst_times.push_back(next_estimate);
                // printf("MOVING PROCESS %d TO READY QUEUE\n", process.pid);
                process.io_time = total_time - (process.cpu_time + process.wait_time);
                remove_io_burst(process);
                move_to_ready_from_blocked(process, ready_queue, blocked_queue);
                i--;
            }
        }
        total_time++;
    }
    args->running = false;

    // log process completions
    log_processes(completed_processes);
    return NULL;
}

bool all_bursts_completed(Process process) {
    return process.cpu_bursts.size() == 0 && process.io_bursts.size() == 0;
}

bool all_processes_finished(std::vector<Process> ready_queue , std::vector<Process> blocked_queue) {
    return ready_queue.size() == 0 && blocked_queue.size() == 0;
}

std::vector<Process> create_processes(std::vector< std::vector<int> > lines) {
    std::vector<Process> processes = std::vector<Process>();
    for (int i = 0; i < lines.size(); i++) {
        std::vector<int> bursts = lines[i];
        int pid = i;
        processes.push_back(create_process(pid, bursts));
    }
    return processes;
}