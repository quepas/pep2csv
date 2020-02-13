//
// Created by quepas on 13/02/2020.
//

#include "PEPTracesListener.h"

void PEPTracesListener::enterTrace_properties(pep_traceParser::Trace_propertiesContext *context) {
    size_t num_properties = context->TRACE_PROPERTY().size();
    if (num_properties != 7) {
        std::cerr << "Wrong number of properties=" << num_properties << ". Should be 7" << std::endl;
        return;
    }
    current_trace_properties.matlab_version = context->TRACE_PROPERTY(0)->getText();
    current_trace_properties.num_threads = std::stoi(context->TRACE_PROPERTY(1)->getText().erase(0, 1));
    current_trace_properties.num_process = std::stoi(context->TRACE_PROPERTY(2)->getText().erase(0, 1));
    current_trace_properties.benchmark = context->TRACE_PROPERTY(3)->getText();
    current_trace_properties.version = context->TRACE_PROPERTY(4)->getText();
    current_trace_properties.N = std::stoi(context->TRACE_PROPERTY(5)->getText().erase(0, 1));
    current_trace_properties.num_in_process = std::stoi(context->TRACE_PROPERTY(6)->getText().erase(0, 1));
}

void PEPTracesListener::enterEvents_list(pep_traceParser::Events_listContext *context) {
    current_trace_properties.performance_events.clear();
    current_trace_properties.performance_events.reserve(context->EVENT().size());
    for (const auto& event : context->EVENT()) {
        std::cout << "event=" << event->getText() << std::endl;
        current_trace_properties.performance_events.push_back(event->getText());
    }
}

void PEPTracesListener::enterRow_values(pep_traceParser::Row_valuesContext *context) {
    context->INT()
}

void PEPTracesListener::enterPep(pep_traceParser::PepContext *context) {
    current_trace_properties.trace_id = 0;
}

void PEPTracesListener::enterTrace(pep_traceParser::TraceContext *context) {
    current_trace_properties.trace_id++;
}
