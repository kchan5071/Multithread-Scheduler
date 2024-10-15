#include "threaded.h"

//Kai Chan
//827673009

/**
 * Remove a cpu burst from a process
 * @param process the process to remove the burst from
 */
void remove_cpu_burst(Process &process) {
    pthread_t process_thread;
    process.executing = true;
    //create thread
    if (pthread_create(&process_thread, NULL, remove_cpu_burst, (void *)&process) != 0) {
        printf("Error creating thread\n");
    }
    //detach thread
    if (pthread_detach(process_thread) != 0) {
        printf("Error detaching thread\n");
    }
    //wait for thread to finish
    while (process.executing) {
        sleep(WAIT_FOR_THREAD);
    }
}

/**
 * Remove an io burst from a process
 * @param process the process to remove the burst from
 */
void remove_io_burst(Process &process) {
    pthread_t process_thread;
    process.executing = true;
    //create thread
    if (pthread_create(&process_thread, NULL, remove_io_burst, (void *)&process) != 0) {
        printf("Error creating thread\n");
    }
    //detach thread
    if (pthread_detach(process_thread) != 0) {
        printf("Error detaching thread\n");
    }
    //wait for thread to finish
    while (process.executing) {
        sleep(WAIT_FOR_THREAD);
    }
}

/**
 * Run a cpu burst for a process
 * @param process the process to run the burst for
 */
void run_cpu_burst(Process &process) {
    pthread_t process_thread;
    process.executing = true;
    //create thread
    if (pthread_create(&process_thread, NULL, run_cpu_burst, (void *)&process) != 0) {
        printf("Error creating thread\n");
    }
    //detach thread
    if (pthread_detach(process_thread) != 0) {
        printf("Error detaching thread\n");
    }
    //wait for thread to finish
    while (process.executing) {
        sleep(WAIT_FOR_THREAD);
    }
}

/**
 * Run an io burst for a process
 * @param process the process to run the burst for
 */
void run_io_burst(Process &process) {
    //check if process has io bursts
    if (process.io_bursts.size() == 0) {
        return;
    }
    pthread_t process_thread;
    process.executing = true;
    //create thread
    if (pthread_create(&process_thread, NULL, run_io_burst, (void *)&process) != 0) {
        printf("Error creating thread\n");
    }
    //detach thread
    if (pthread_detach(process_thread) != 0) {
        printf("Error detaching thread\n");
    }
    //wait for thread to finish
    while (process.executing) {
        sleep(WAIT_FOR_THREAD);
    }
}