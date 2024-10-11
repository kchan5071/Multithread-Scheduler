#include "process.h"

Process::Process(int pid, int arrival_time, std::vector<int> bursts) {
    this->pid = pid;
    this->arrival_time = arrival_time;
    this->completed = false;
    this->status = "ready";
    this->start_time = 0;
    this->end_time = 0;
    this->wait_time = 0;
    this->turnaround_time = 0;
    this->remaining_time = 0;
    for (int i = 0; i < bursts.size(); i++) {
        if (i % 2 == 0) {
            this->cpu_bursts.push(bursts[i]);
            this->remaining_time += bursts[i];
        } else {
            this->io_bursts.push(bursts[i]);
        }
    }
}