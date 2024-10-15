#include "threaded.h"

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
    if (process.io_bursts.size() == 0) {
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