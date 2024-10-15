#ifndef THREADED_H
#define THREADED_H

#include <string>
#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include "process.h"

#define WAIT_FOR_THREAD 0

void remove_cpu_burst(Process &process);

void remove_io_burst(Process &process);

void run_cpu_burst(Process &process);

void run_io_burst(Process &process);

#endif