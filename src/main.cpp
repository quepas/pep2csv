//
// Created by quepas on 13/02/2020.
//

#include <iostream>
#include <fstream>

int main(int argc, char **argv) {
    std::ifstream trace_file;
    if (argc > 1) {
        std::string trace_file_path = argv[1];
        std::cout << "Input trace file=" << trace_file_path << std::endl;
        trace_file.open (trace_file_path);
    }

    if (!trace_file.is_open()) {
        std::cerr << "Wrong path to the trace file" << std::endl;
        return EXIT_FAILURE;
    }
    //execute(easy_jit, argv[1]);
    std::cout << "pep2csv" << std::endl;
    return EXIT_SUCCESS;
}