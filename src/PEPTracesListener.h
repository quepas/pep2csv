//
// Created by quepas on 13/02/2020.
//

#ifndef PEP2CSV_PROJECT_PEPTRACESLISTENER_H
#define PEP2CSV_PROJECT_PEPTRACESLISTENER_H

#include "parser/pep_traceBaseListener.h"

class PEPTracesListener : public pep_traceBaseListener {
public:
    struct TraceProperties {
        int trace_id;
        std::string matlab_version;
        int num_threads;
        int num_process;
        std::string benchmark;
        std::string version;
        int N;
        int num_in_process;
        std::vector<std::string> performance_events;
    } current_trace_properties;

    void enterPep(pep_traceParser::PepContext *context) override;

    void enterTrace(pep_traceParser::TraceContext *context) override;

    void enterTrace_properties(pep_traceParser::Trace_propertiesContext *context) override;

    void enterEvents_list(pep_traceParser::Events_listContext *context) override;

    void enterRow_values(pep_traceParser::Row_valuesContext *context) override;

};


#endif //PEP2CSV_PROJECT_PEPTRACESLISTENER_H
