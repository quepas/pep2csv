//
// Created by quepas on 13/02/2020.
//

#include "PEPTracesListener.h"

using std::to_string;
using std::vector;

void PEPTracesListener::enterTrace_properties(pep_traceParser::Trace_propertiesContext *context) {
    size_t num_properties = context->TRACE_PROPERTY().size();
    if (num_properties != 7) {
        std::cerr << "Wrong number of properties=" << num_properties << ". Should be 7" << std::endl;
        return;
    }
    current_trace_properties.matlab_version = context->TRACE_PROPERTY(0)->getText().erase(0, 1);
    current_trace_properties.num_threads = std::stoi(context->TRACE_PROPERTY(1)->getText().erase(0, 1));
    current_trace_properties.num_process = std::stoi(context->TRACE_PROPERTY(2)->getText().erase(0, 1));
    current_trace_properties.benchmark = context->TRACE_PROPERTY(3)->getText().erase(0, 1);
    current_trace_properties.version = context->TRACE_PROPERTY(4)->getText().erase(0, 1);
    current_trace_properties.N = std::stoi(context->TRACE_PROPERTY(5)->getText().erase(0, 1));
    current_trace_properties.num_in_process = std::stoi(context->TRACE_PROPERTY(6)->getText().erase(0, 1));
}

void PEPTracesListener::enterEvents_list(pep_traceParser::Events_listContext *context) {
    current_trace_properties.performance_events.clear();
    current_trace_properties.performance_events.reserve(context->EVENT().size());
    for (const auto &event : context->EVENT()) {
        auto event_name = event->getText();
        if (event_name == "address") {
            current_trace_properties.has_address_value = true;
        }
        current_trace_properties.performance_events.push_back(event->getText());
    }
    current_trace_properties.prev_pep_values = vector<long long int>(context->EVENT().size(), 0);
}

void PEPTracesListener::enterRow_values(pep_traceParser::Row_valuesContext *context) {
    current_trace_properties.num_row_values++;
    // Gather current pep values
    vector<long long int> curr_pep_values;
    curr_pep_values.reserve(context->INT().size());
    for (auto &value : context->INT()) {
        curr_pep_values.push_back(std::stoll(value->getText()));
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

void PEPTracesListener::enterPep(pep_traceParser::PepContext *context) {
    // Write here the CSV file header
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
}

void PEPTracesListener::enterTrace(pep_traceParser::TraceContext *context) {
    current_trace_properties.num_row_values = 0;
    current_trace_properties.trace_id++;
    current_trace_properties.has_address_value = false;
}

PEPTracesListener::PEPTracesListener(const std::string &csv_file_path) {
    input_file.open(csv_file_path);
    csv_writer = new csv::CSVWriter<std::ofstream>(input_file);
}

PEPTracesListener::~PEPTracesListener() {
    delete csv_writer;
    input_file.close();
}
