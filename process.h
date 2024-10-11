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

    Process(int pid, int arrival_time, std::queue<int> bursts);

    void sort_into_queues();

    void start(int time);
    void stop(int time);
    void resume(int time);
    void complete(int time);
    void print();

};