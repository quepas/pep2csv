//
// Created by quepas on 13/02/2020.
//

#include <iostream>
#include <filesystem>
#include "PEPTracesListener.h"
#include "tree/IterativeParseTreeWalker.h"
#include "parser/pep_traceLexer.h"

int main(int argc, char **argv) {
    std::ifstream trace_file;
    if (argc > 1) {
        std::string trace_file_path = argv[1];
        std::cout << "Input trace file=" << trace_file_path << std::endl;
        if (!std::filesystem::exists(trace_file_path)) {
            std::cerr << "Trace file doesn't exist" << std::endl;
            return EXIT_FAILURE;
        }
        trace_file.open(trace_file_path);
        if (!trace_file.is_open()) {
            std::cerr << "Couldn't open file" << std::endl;
            return EXIT_FAILURE;
        }
    }

    // Parse the input trace
    antlr4::ANTLRInputStream input(trace_file);
    pep_traceLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    tokens.fill();

    pep_traceParser parser(&tokens);
    antlr4::tree::ParseTree *tree = parser.pep();

    antlr4::tree::IterativeParseTreeWalker tree_walker;
    PEPTracesListener pep_listener;
    tree_walker.walk(&pep_listener, tree);

    return EXIT_SUCCESS;
}