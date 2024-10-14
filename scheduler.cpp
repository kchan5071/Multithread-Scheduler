#include "scheduler.h"

void* run_scheduler(void *ptr) {
    // Get arguments
    SchedulerArgs *args = (SchedulerArgs *) ptr;
    std::vector<std::vector<int>> lines = args->lines;
    bool exponential = args->exponential;
    std::string option_argument = args->option_argument;

    for (int i = 0; i < lines.size(); i++) {
        log_process_bursts(i, reinterpret_cast<unsigned int*>(lines[i].data()), lines[i].size());
    }

    // Throw all processes into ready queue and sort by next burst
    std::vector<Process> processes = sort_processes(create_processes(lines));
    std::vector<Process> ready_queue;
    std::vector<Process> blocked_queue;
    std::vector<Process> completed_processes;
    int total_elapsed_time = 0;

    for (int i = 0; i < processes.size(); i++) {
        ready_queue.push_back(processes[i]);
    }

    // Run scheduler
    while (!all_processes_finished(processes)) {
        // Get next process
        if (ready_queue.size() == 0) {
            break;
        }
        Process process = ready_queue[0];
        ready_queue.erase(ready_queue.begin());

        // Get burst time
        int burst = get_next_burst(process);

        // Run CPU burst
        pthread_t process_thread;
        pthread_create(&process_thread, NULL, run_cpu_burst, (void *)&process);
        pthread_join(process_thread, NULL);

        // Increment executed CPU bursts and total elapsed time
        process.executed_cpu_bursts += burst;
        total_elapsed_time += burst;

        // Update blocked queue
        for (int i = 0; i < blocked_queue.size(); i++) {
            Process &blocked_process = blocked_queue[i];
            int io_burst = get_next_io_burst(blocked_process);

            if (io_burst <= burst) {
                // Move to ready queue
                blocked_process.executed_io_bursts += io_burst;
                blocked_queue.erase(blocked_queue.begin() + i);
                ready_queue.push_back(blocked_process);
                i--; // Adjust index after removal
            } else {
                // Update IO burst
                blocked_process.io_bursts[blocked_process.executed_io_bursts] -= burst;
                blocked_process.executed_io_bursts += burst;
            }
        }

        // Sort ready queue by next CPU burst
        std::stable_sort(ready_queue.begin(), ready_queue.end(), [](const Process &a, const Process &b) {
            return get_next_burst(a) < get_next_burst(b);
        });

        // Sort blocked queue by next IO burst
        std::stable_sort(blocked_queue.begin(), blocked_queue.end(), [](const Process &a, const Process &b) {
            return get_next_io_burst(a) < get_next_io_burst(b);
        });

        // Check if the process has completed all bursts
        if (all_bursts_completed(process)) {
            completed_processes.push_back(process);
        } else {
            // Move to blocked queue
            blocked_queue.push_back(process);
        }

        // Log CPU burst execution
        log_cpuburst_execution(process.pid, burst, 0, total_elapsed_time, COMPLETED);

    }
    // Log process completion
    for (int i = 0; i < completed_processes.size(); i++) {
        Process process = completed_processes[i];
        log_process_completion(process.pid, process.completion_time, 0);
    }

    return NULL;
}

bool all_bursts_completed(Process process) {
    return process.executed_cpu_bursts >= process.cpu_bursts.size() && 
           process.executed_io_bursts >= process.io_bursts.size();
}

bool all_processes_finished(std::vector<Process> processes) {
    for (int i = 0; i < processes.size(); i++) {
        if (get_remaining_bursts(processes[i]) > 0) {
            return false;
        }
    }
    return true;
}

std::vector<Process> sort_processes(std::vector<Process> processes) {
    std::sort(processes.begin(), processes.end(), [](Process a, Process b) {
        return get_next_burst(a) < get_next_burst(b);
    });
    return processes;
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

    //create process
    Process process;
    process.pid = pid;
    process.remaining_bursts = bursts.size();
    process.executed_cpu_bursts = cpu_bursts.size();
    process.executed_io_bursts = io_bursts.size();
    process.completion_time = 0;
    process.cpu_bursts = cpu_bursts;
    process.io_bursts = io_bursts;
    return process;
}
