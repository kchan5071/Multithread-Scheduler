#ifndef SORTER_H
#define SORTER_H

#include <vector>
#include <algorithm>
#include "process.h"

//Kai Chan
//827673009

bool compare_ready_processes(const Process &a, const Process &b);

bool compare_blocked_processes(const Process &a, const Process &b);

bool compare_completion_times(const Process &a, const Process &b);

void sort_ready_queue(std::vector<Process>& processes);

void sort_completed_processes(std::vector<Process> &processes);

void sort_blocked_queue(std::vector<Process> &processes);

#endif