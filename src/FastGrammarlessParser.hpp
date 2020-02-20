//
// Created by quepas on 20/02/2020.
//

#ifndef PEP2CSV_PROJECT_FASTGRAMMARLESSPARSER_HPP
#define PEP2CSV_PROJECT_FASTGRAMMARLESSPARSER_HPP

#include <fstream>
#include <csv.hpp>
#include "TraceProperties.hpp"

class FastGrammarlessParser {

public:
    FastGrammarlessParser(std::ifstream &input_file, const std::string& output_file_path);

    virtual ~FastGrammarlessParser();

    bool ParseAndSave();

private:
    std::ifstream & input_file;
    std::ofstream csv_file;
    TraceProperties current_trace_properties;
    csv::CSVWriter<std::ofstream>* csv_writer;

    void ParseHeader(const std::string& header_str);
    void ParseValuesRow(const std::string& values_str);

    std::vector<std::string> SplitBy(const std::string& str, char delimeter);

};


#endif //PEP2CSV_PROJECT_FASTGRAMMARLESSPARSER_HPP
