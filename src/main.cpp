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
    std::string pep_file_path, csv_file_path = "./output.csv";
    if (argc > 1) {
        pep_file_path = argv[1];
        if (!std::filesystem::exists(pep_file_path)) {
            std::cerr << "PEP file doesn't exist: " << pep_file_path << std::endl;
            return EXIT_FAILURE;
        }
        trace_file.open(pep_file_path);
        if (!trace_file.is_open()) {
            std::cerr << "Couldn't open PEP file: " << pep_file_path << std::endl;
            return EXIT_FAILURE;
        }
    } else {
        std::cerr << "Missing path to the PEP file." << std::endl;
        return EXIT_FAILURE;
    }
    if (argc > 2) {
        csv_file_path = argv[2];
    } else {
        std::cout << "Using default CSV output path: ./output.csv" << std::endl;
    }

    std::cout << "pep2csv: " << pep_file_path << " --> " << csv_file_path << std::endl;

    // Parse the input trace
    antlr4::ANTLRInputStream input(trace_file);
    pep_traceLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    tokens.fill();

    pep_traceParser parser(&tokens);
    antlr4::tree::ParseTree *tree = parser.pep();

    antlr4::tree::IterativeParseTreeWalker tree_walker;
    PEPTracesListener pep_listener(csv_file_path);
    tree_walker.walk(&pep_listener, tree);

    return EXIT_SUCCESS;
}