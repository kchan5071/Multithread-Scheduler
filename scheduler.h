#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include "process.h"

class Scheduler {
public:

    Scheduler(std::vector<std::vector<int>> bursts, bool exponential, std::string option_argument);

    void start() {}

    int get_number_of_bursts() {return -1;}

private:
    std::vector<Process> processes;
    std::vector<Process> ready_queue;
    std::vector<Process> blocked_queue;
    int time;
    bool exponential;
    int current_burst;
    std::string option_argument;
    int number_of_bursts;

    int number_of_processes;

    unsigned int cpu_time_executed;
    unsigned int io_time_executed;

    unsigned int start_time;
    unsigned int end_time;
    unsigned int current_time;

    void sort_into_queues();

    void sort_ready_queue();

    void sort_blocked_queue() {}

    void update_queues() {}

    void print();
};