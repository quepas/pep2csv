//
// Created by quepas on 13/02/2020.
//

#ifndef PEP2CSV_PROJECT_PEPTRACESLISTENER_H
#define PEP2CSV_PROJECT_PEPTRACESLISTENER_H

#include "csv.hpp"
#include "parser/pep_traceBaseListener.h"

class PEPTracesListener : public pep_traceBaseListener {
public:
    explicit PEPTracesListener(const std::string &csv_file_path);

    ~PEPTracesListener() override;

    void enterPep(pep_traceParser::PepContext *context) override;

    void enterTrace(pep_traceParser::TraceContext *context) override;

    void enterTrace_properties(pep_traceParser::Trace_propertiesContext *context) override;

    void enterEvents_list(pep_traceParser::Events_listContext *context) override;

    void enterRow_values(pep_traceParser::Row_valuesContext *context) override;

private:
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
        std::vector<long long int> prev_pep_values;
        int num_row_values;
        bool has_address_value;
    } current_trace_properties;

    std::ofstream input_file;
    csv::CSVWriter<std::ofstream>* csv_writer;

};


#endif //PEP2CSV_PROJECT_PEPTRACESLISTENER_H
