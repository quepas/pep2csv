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
    for (const auto& event : context->EVENT()) {
        current_trace_properties.performance_events.push_back(event->getText());
    }
}

void PEPTracesListener::enterRow_values(pep_traceParser::Row_valuesContext *context) {
    // First value (idx=0) is the "time" !
    for (int idx = 1; idx < context->INT().size(); ++idx) {
        (*csv_writer)  << vector {
            to_string(current_trace_properties.trace_id),
            current_trace_properties.matlab_version,
            to_string(current_trace_properties.num_threads),
            to_string(current_trace_properties.num_process),
            current_trace_properties.benchmark,
            current_trace_properties.version,
            to_string(current_trace_properties.N),
            to_string(current_trace_properties.num_in_process),
            current_trace_properties.performance_events[idx],
            context->INT(0)->toString(),
            context->INT(idx)->toString()
        };
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
    current_trace_properties.trace_id++;
}

PEPTracesListener::PEPTracesListener(const std::string &csv_file_path) {
    input_file.open(csv_file_path);
    csv_writer = new csv::CSVWriter<std::ofstream>(input_file);
}

PEPTracesListener::~PEPTracesListener() {
    delete csv_writer;
    input_file.close();
}
