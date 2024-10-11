#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <unistd.h>
#include <getopt.h>
#include <vector>

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

std::string remove_endline(std::string line) {
    if (line[line.length() - 1] == '\n') {
        line = line.substr(0, line.length() - 1);
    }
    return line;
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
    std::vector<std::string> lines = std::vector<std::string>();
    std::string line;
    for (int i = 0; (line = read_line(file)) != ""; i++) {
        line = remove_endline(line);
        lines.push_back(line);
        std::cout << "line " << i << ": " << lines[i] << std::endl;
    }

    //close file
    fclose(file);


    return 0;
}