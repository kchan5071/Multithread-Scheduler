#ifndef THREADED_H
#define THREADED_H

#include <string>
#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include "process.h"

//Kai Chan
//827673009

#define WAIT_FOR_THREAD 0 //can be set to wait for thread to finish, 0 for no wait

//while not directly doing threading, it manages the functions that do
//those can be found in process.cpp

void remove_cpu_burst(Process &process);

void remove_io_burst(Process &process);

void run_cpu_burst(Process &process);

void run_io_burst(Process &process);

#endif