//
// Created by quepas on 20/02/2020.
//

#ifndef PEP2CSV_PROJECT_TRACEPROPERTIES_HPP
#define PEP2CSV_PROJECT_TRACEPROPERTIES_HPP

#include <string>
#include <vector>

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
};

#endif //PEP2CSV_PROJECT_TRACEPROPERTIES_HPP
