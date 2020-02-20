//
// Created by quepas on 20/02/2020.
//

#include "FastGrammarlessParser.hpp"

#include <string>
#include <iostream>
#include <regex>

using std::cerr;
using std::endl;
using std::getline;
using std::ifstream;
using std::move;
using std::regex;
using std::sregex_token_iterator;
using std::string;
using std::vector;
using std::to_string;

FastGrammarlessParser::FastGrammarlessParser(ifstream &input_file, const string& csv_file_path) : input_file(
        input_file) {
    csv_file.open(csv_file_path);
    if (!csv_file.is_open()) {
        cerr << "Failed to open the CSV file=" << csv_file_path << endl;
    }
    csv_writer = new csv::CSVWriter<std::ofstream>(csv_file);
}

bool FastGrammarlessParser::ParseAndSave() {
    (*csv_writer) << vector{
            "trace_id",
            "matlab",
            "threads",
            "process",
            "benchmark",
            "version",
            "N",
            "in_process",
            "metrics",
            "time",
            "value"};
    current_trace_properties.trace_id = 0;
    string line;
    while (getline(input_file, line)) {
        if (line.empty()) continue;
        if (std::isdigit(line[0])) {
            ParseValuesRow(line);
        } else if (line[0] == '@') {
            ParseHeader(line);
        }
    }
    return true;
}

void FastGrammarlessParser::ParseHeader(const string& header_str) {
    if (header_str.rfind("@trace_start:", 0) == 0) {
        current_trace_properties.num_row_values = 0;
        current_trace_properties.trace_id++;
        current_trace_properties.has_address_value = false;
        auto chunks = SplitBy(header_str.substr(13), ':');

        if (chunks.size() != 7) { //
            std::cerr << "Wrong number of properties=" << chunks.size() << ". Should be 7" << std::endl;
            return;
        }
        current_trace_properties.matlab_version = chunks[0];
        current_trace_properties.num_threads = std::stoi(chunks[1]);
        current_trace_properties.num_process = std::stoi(chunks[2]);
        current_trace_properties.benchmark = chunks[3];
        current_trace_properties.version = chunks[4];
        current_trace_properties.N = std::stoi(chunks[5]);
        current_trace_properties.num_in_process = std::stoi(chunks[6]);
    } else if (header_str.rfind("@perf_events:", 0) == 0) {
        current_trace_properties.performance_events.clear();
        auto chunks = SplitBy(header_str.substr(13), ',');
        current_trace_properties.performance_events = chunks;
        if (chunks[chunks.size()-1] == "address") {
            current_trace_properties.has_address_value = true;
        }
        current_trace_properties.prev_pep_values = vector<long long int>(chunks.size(), 0);
    }
}

void FastGrammarlessParser::ParseValuesRow(const string& values_str) {
    auto chunks = SplitBy(values_str, ',');
    current_trace_properties.num_row_values++;
    // Gather current pep values
    vector<long long int> curr_pep_values;
    curr_pep_values.reserve(chunks.size());
    for (auto &value : chunks) {
        curr_pep_values.push_back(std::stoll(value));
    }
    auto diff_pep_values = curr_pep_values;
    // Compute the difference (except for the "address" column)
    size_t max_diff_idx = current_trace_properties.has_address_value ? curr_pep_values.size() - 1 : curr_pep_values.size();
    for (size_t idx = 1; idx < max_diff_idx; ++idx) {
        diff_pep_values[idx] = curr_pep_values[idx] - current_trace_properties.prev_pep_values[idx];
    }
    // First value (idx=0) is the "time" !
    for (size_t idx = 1; idx < curr_pep_values.size(); ++idx) {
        (*csv_writer) << vector{
                to_string(current_trace_properties.trace_id),
                current_trace_properties.matlab_version,
                to_string(current_trace_properties.num_threads),
                to_string(current_trace_properties.num_process),
                current_trace_properties.benchmark,
                current_trace_properties.version,
                to_string(current_trace_properties.N),
                to_string(current_trace_properties.num_in_process),
                current_trace_properties.performance_events[idx],
                to_string(curr_pep_values[0]),
                to_string(diff_pep_values[idx])
        };
    }
    // Copy current pep values as previous values
    current_trace_properties.prev_pep_values.clear();
    for (auto &value : curr_pep_values) {
        current_trace_properties.prev_pep_values.push_back(value);
    }
}

vector<string> FastGrammarlessParser::SplitBy(const string& str, char delimeter) {
    vector<string> chunks;
    string word;
    for (auto chr : str) {
        if (chr == delimeter) {
            chunks.push_back(word);
            word.clear();
        } else {
            word += chr;
        }
    }
    chunks.push_back(word);
    return chunks;
}

FastGrammarlessParser::~FastGrammarlessParser() {
    delete csv_writer;
    csv_file.close();
}
