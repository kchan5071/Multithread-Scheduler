#include <string>
#include <vector>
#include <queue>
#include <string>

class Process {
public:
    int pid;
    bool completed;
    std::string status;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int start_time;
    int end_time;
    int wait_time;
    int turnaround_time;

    
    std::queue<int> cpu_bursts;
    std::queue<int> io_bursts;

    Process(int pid, int arrival_time, std::vector<int> bursts);

    void sort_into_queues() {};

    bool operator<(const Process &p) const {
        return this->arrival_time < p.arrival_time;
    }

    void print() {

    }



};