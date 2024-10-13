#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <unistd.h>
#include <getopt.h>
#include <vector>
#include "scheduler.h"

#define NORMAL_EXIT 0 

FILE* open_file(std::string filename) {
    FILE *file = fopen(filename.c_str() , "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file: %s\n", filename.c_str());
        exit(1);
    }
    return file;
}

std::string read_line(FILE *file) {
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    read = getline(&line, &len, file);
    if (read == -1) {
        return "";
    }
    return std::string(line);
}

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

void check_if_odd(std::vector< std::vector<int> > lines) {
    for (int i = 0; i < lines.size(); i++) {
        if (lines[i].size() % 2 != 1) {
            fprintf(stderr, "odd number of burst in line %i\n", i + 1);
            exit(NORMAL_EXIT);
        }
    }
}

void check_contains_zero(std::vector< std::vector<int> > lines) {
    for (int i = 0; i < lines.size(); i++) {
        for (int j = 0; j < lines[i].size(); j++) {
            if (lines[i][j] == 0) {
                fprintf(stderr, "zero burst in line %i\n", i + 1);
                exit(NORMAL_EXIT);
            }
        }
    }
}

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

    printf("Exponential: %d\n", exponential);
    std::cout << "string optarg: " << option_argument << std::endl;
    std::cout << "filename: " << filename << std::endl;

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
    check_contains_zero(lines);

    for (int i = 0; i < lines.size(); i++) {
        for (int j = 0; j < lines[i].size(); j++) {
            printf("%d ", lines[i][j]);
        }
        printf("\n");
    }

    //create scheduler in new thread
    Scheduler scheduler = Scheduler(lines, exponential, option_argument);
    return 0;
}