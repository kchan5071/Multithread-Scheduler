#include <string>
#include <iostream>
#include <unistd.h>
#include <getopt.h>
#include <vector>
#include <pthread.h>
#include "scheduler.h"

//Kai Chan
//827673009

#define NORMAL_EXIT 0 

/**
 * Open a file with the given filename
 * @param filename the name of the file to open
 * @return the file pointer
 */
FILE* open_file(std::string filename) {
    //open file
    FILE *file = fopen(filename.c_str() , "r");
    //check if file was opened successfully
    if (file == NULL) {
        fprintf(stderr, "Unable to open %s\n", filename.c_str());
        exit(1);
    }
    return file;
}

/**
 * Read a line from a file
 * @param file the file to read from
 * @return the line read from the file
 */
std::string read_line(FILE *file) {
    //initialize variables
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    //read line from file
    read = getline(&line, &len, file);
    if (read == -1) {
        return "";
    }
    return std::string(line);
}

/**
 * Convert a line to a vector of integers
 * @param line the line to convert
 * @return the vector of integers
 */
std::vector<int> convert_line_to_ints(std::string line) {
    std::vector<int> ints = std::vector<int>();
    std::string num = "";

    //iterate through each character in the line
    for (int i = 0; i < line.length(); i++) {
        //if the character is a space, convert the number to an int and add it to the vector
        if (line[i] == ' ') {
            ints.push_back(std::stoi(num));
            num = "";
        } else {
            num += line[i];
        }
    }
    //add the last number to the vector
    ints.push_back(std::stoi(num));
    return ints;
}

/**
 * Check if the number of bursts for each process is odd
 * @param lines the lines to check
 */
void check_if_odd(std::vector< std::vector<int> > lines) {
    for (int i = 0; i < lines.size(); i++) {
        //if the number of bursts is not odd, print an error message and exit
        if (lines[i].size() % 2 != 1) {
            fprintf(stderr, "There must be an odd number of bursts for each process\n");
            exit(NORMAL_EXIT);
        }
    }
}

/**
 * Check if the alpha value is valid (0 < alpha < 1)
 * @param alpha the alpha value to check
 */
void check_for_valid_alpha(float alpha) {
    if (alpha <= 0 || alpha >= 1) {
        fprintf(stderr, "Alpha for exponential averaging must be within (0.0, 1.0)\n");
        exit(NORMAL_EXIT);
    }
}

/**
 * Check if any burst is 0 or negative from a 2D vector of integers
 * @param lines the lines to check
 */
void check_contains_zero_or_negative(std::vector< std::vector<int> > lines) {
    //iterate through each line
    for (int i = 0; i < lines.size(); i++) {
        //iterate through each number in the line
        for (int j = 0; j < lines[i].size(); j++) {
            //if the number is 0 or negative, print an error message and exit
            if (lines[i][j] <= 0) {
                fprintf(stderr, "A burst number must be bigger than 0\n");
                exit(NORMAL_EXIT);
            }
        }
    }
}

/**
 * Main function
 * @param argc the number of arguments
 * @param argv the arguments
 * @return the exit status
 */
int main(int argc, char **argv) {

    //parse command line arguments
    bool exponential = false;
    int option;
    std::string option_argument;
    while ((option = getopt(argc, argv, "a:")) != -1) {
        switch (option) {
            case 'a':
                exponential = true;
                option_argument = optarg;
                break;
            default:
                break;
        }
    }
    //get filename from first argument
    std::string filename = argv[optind];

    //open file
    FILE *file = open_file(filename.c_str());

    //read lines from file
    std::vector< std::vector<int> > lines = std::vector< std::vector<int> >();
    std::string line;
    for (int i = 0; (line = read_line(file)) != ""; i++) {
        lines.push_back(convert_line_to_ints(line));
        for (int j = 0; j < lines[i].size(); j++) {
        }
    }

    //close file
    fclose(file);

    //check if valid input
    check_if_odd(lines);
    check_contains_zero_or_negative(lines);

    //create scheduler in new thread
    pthread_t scheduler_thread;
    SchedulerArgs args;
    args.lines = lines;
    args.exponential = exponential;
    args.running = true;
    //parse alpha value
    try {
        args.option_argument = std::stof(option_argument);
    } catch (std::invalid_argument e) {
        args.option_argument = 0.5;
    }
    //check if alpha is valid
    check_for_valid_alpha(args.option_argument);

    //create scheduler thread
    if (pthread_create(&scheduler_thread, NULL, run_scheduler, (void *)&args) != 0) {
        fprintf(stderr, "Error creating thread\n");
    }

    //wait for scheduler to finish
    if (pthread_detach(scheduler_thread) != 0) {
        fprintf(stderr, "Error detaching thread\n");
    }

    while (args.running) {
        sleep(1);
    }

    return 0;
}