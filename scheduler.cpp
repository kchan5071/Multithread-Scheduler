#include "scheduler.h"
#include <iostream>
#include <unistd.h>

#define WAIT_FOR_THREAD 0

void initialize_queues(std::vector<Process> processes, std::vector<Process> &ready_queue, std::vector<Process> &blocked_queue) {
    for (int i = 0; i < processes.size(); i++) {
        ready_queue.push_back(processes[i]);
    }
}

void print_queue(std::vector<Process> queue, std::string name) {
    printf("PRINTING QUEUE %s\n", name.c_str());
    if (queue.size() == 0) {
        printf("Queue is empty\n");
    }
    for (Process process : queue) {
        print_process(process);
    }
}

std::vector<Process> sort_ready_queue(std::vector<Process> processes) {
    if (processes.size() <= 1) {
        return processes;
    }
    std::stable_sort(processes.begin(), processes.end(), [](Process a, Process b) {
        return a.estimated_cpu_burst_time < b.estimated_cpu_burst_time;
    });
    return processes;
}

std::vector<Process> sort_blocked_queue(std::vector<Process> processes) {
    if (processes.size() <= 1) {
        return processes;
    }
    std::stable_sort(processes.begin(), processes.end(), [](Process a, Process b) {
        return a.io_bursts[0] < b.io_bursts[0];
    });
    return processes;
}

std::vector<Process> remove_first_process(std::vector<Process> processes) {
    if (processes.size() == 0) {
        return processes;
    }
    for (int i = 0; i < processes.size() - 1; i++) {
        processes[i] = processes[i + 1];
    }
    processes.pop_back();
    return processes;
}

void remove_cpu_burst(Process &process) {
    pthread_t process_thread;
    process.executing = true;
    if (pthread_create(&process_thread, NULL, remove_cpu_burst, (void *)&process) != 0) {
        printf("Error creating thread\n");
    }
    if (pthread_detach(process_thread) != 0) {
        printf("Error detaching thread\n");
    }
    while (process.executing) {
        sleep(WAIT_FOR_THREAD);
    }
}

void remove_io_burst(Process &process) {
    pthread_t process_thread;
    process.executing = true;
    if (pthread_create(&process_thread, NULL, remove_io_burst, (void *)&process) != 0) {
        printf("Error creating thread\n");
    }
    if (pthread_detach(process_thread) != 0) {
        printf("Error detaching thread\n");
    }
    while (process.executing) {
        sleep(WAIT_FOR_THREAD);
    }
}

void run_cpu_burst(Process &process) {
    pthread_t process_thread;
    process.executing = true;
    if (pthread_create(&process_thread, NULL, run_cpu_burst, (void *)&process) != 0) {
        printf("Error creating thread\n");
    }
    if (pthread_detach(process_thread) != 0) {
        printf("Error detaching thread\n");
    }
    while (process.executing) {
        sleep(WAIT_FOR_THREAD);
    }
}

void run_io_burst(Process &process) {
    if (process.io_bursts.size() == NULL) {
        return;
    }
    pthread_t process_thread;
    process.executing = true;
    if (pthread_create(&process_thread, NULL, run_io_burst, (void *)&process) != 0) {
        printf("Error creating thread\n");
    }
    if (pthread_detach(process_thread) != 0) {
        printf("Error detaching thread\n");
    }
    while (process.executing) {
        sleep(WAIT_FOR_THREAD);
    }
}

void calculate_estimated_bursts(std::vector<Process> &processes, float alpha) {
    for (int i = 0; i < processes.size(); i++) {
        calculate_estimated_bursts(&processes[i], alpha);
    }
}

void print_estimated_bursts(std::vector<Process> processes) {
    for (int i = 0; i < processes.size(); i++) {
        printf("Process %d estimated burst time: %f\n", processes[i].pid, processes[i].estimated_cpu_burst_time);
    }
}

void* run_scheduler(void *ptr) {
    // Get arguments
    int total_elapsed_time = 0;
    SchedulerArgs *args = (SchedulerArgs *) ptr;
    std::vector<std::vector<int>> lines = args->lines;
    bool exponential = args->exponential;
    float option_argument = args->option_argument;

    // Log process bursts
    for (int i = 0; i < lines.size(); i++) {
        log_process_bursts(i, reinterpret_cast<unsigned int*>(lines[i].data()), lines[i].size());
    }

    // Throw all processes into ready queue and sort by next burst
    // std::vector<Process> processes = create_processes(lines);

    //initialize queues
    std::vector<Process> ready_queue = std::vector<Process>();
    std::vector<Process> blocked_queue = std::vector<Process>();
    std::vector<Process> completed_processes = std::vector<Process>();


    std::vector<Process> processes = create_processes(lines);
    calculate_estimated_bursts(processes, option_argument);

    initialize_queues(processes, ready_queue, blocked_queue);
    
    // Run scheduler
    int total_time = 0;
    int current_burst_time = 0;
    ready_queue[0].cpu_bursts[0]++;
    while (!all_processes_finished(processes)) {

        calculate_estimated_bursts(ready_queue, option_argument);
        print_queue(ready_queue, "ready");
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
            //copy process 
            remove_cpu_burst(process);
            //check if process has completed all bursts
            if (all_bursts_completed(process)) {
                process.completion_time = total_time;
                completed_processes.push_back(process);
                log_cpuburst_execution(process.pid, current_burst_time, process.io_time, total_time, COMPLETED);
                log_process_completion(process.pid, process.completion_time, process.wait_time);
                ready_queue = remove_first_process(ready_queue);
                current_burst_time = 0;
            }
            else {
                log_cpuburst_execution(process.pid, current_burst_time, process.io_time, total_time, ENTER_IO);
                //move process to blocked queue
                blocked_queue.push_back(process);
                ready_queue = remove_first_process(ready_queue);
                current_burst_time = 0;
            }
        }
        //check if any process in blocked queue has completed its io burst
        for (int i = 0; i < blocked_queue.size(); i++) {
            Process &process = blocked_queue[i];
            if (process.io_bursts[0] == 0) {
                remove_io_burst(process);
                ready_queue.push_back(process);
                blocked_queue = remove_first_process(blocked_queue);
            }
            else if (process.io_bursts.size() == 0) {
                process.io_time += current_burst_time;
                ready_queue.push_back(process);
                blocked_queue = remove_first_process(blocked_queue);
            }
        }

        if (ready_queue.size() == 0 && blocked_queue.size() == 0) {
            break;
        }

        total_time++;
        current_burst_time++;
    }
    return NULL;
}

bool all_bursts_completed(Process process) {
    return process.cpu_bursts.size() == 0 && process.io_bursts.size() == 0;
}

bool all_processes_finished(std::vector<Process> processes) {
    for (int i = 0; i < processes.size(); i++) {
        if (get_remaining_bursts(processes[i]) > 0) {
            return false;
        }
    }
    return true;
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
    process.executing = false;
    process.io_time = 0;
    process.cpu_time = 0;
    process.wait_time = 0;
    return process;
}
