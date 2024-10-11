#include "scheduler.h"

Scheduler::Scheduler(std::vector<std::vector<int>> bursts, bool exponential, std::string option_argument) {
    this->time = 0;
    this->exponential = exponential;
    this->option_argument = option_argument;
    this->number_of_bursts = 0;
    this->cpu_time_executed = 0;
    this->io_time_executed = 0;
    this->start_time = 0;
    this->end_time = 0;
    this->current_time = 0;
    this -> number_of_processes = bursts.size();

    //create processes
    for (int i = 0; i < number_of_processes; i++) {
        // Process process = Process(i, 0, bursts[i]);
        // this->processes.push_back(process);
    }

    //put processes in ready queue
    for (int i = 0; i < this->processes.size(); i++) {
        // this->processes[i].sort_into_queues();
        // this->ready_queue.push_back(this->processes[i]);
    }

    //sort ready queue
    // this->sort_ready_queue();
}

void Scheduler::sort_into_queues() {
    for (int i = 0; i < this->processes.size(); i++) {
        this->processes[i].sort_into_queues();
    }
}

void Scheduler::sort_ready_queue() {
    //take out into vector
    std::vector<Process> temp;
    for (int i = 0; i < this->ready_queue.size(); i++) {
        if (this->ready_queue[i].arrival_time <= this->time) {
            temp.push_back(this->ready_queue[i]);
        }
    }
    //sort
    std::sort(temp.begin(), temp.end());

    //place back in queue
    for (int i = 0; i < this->ready_queue.size(); i++) {
        if (this->ready_queue[i].arrival_time <= this->time) {
            this->ready_queue[i] = temp[0];
            temp.erase(temp.begin());
        }
    }
    delete &temp;
}

void Scheduler::print() {
    for (int i = 0; i < this->processes.size(); i++) {
        this->processes[i].print();
    }
}

