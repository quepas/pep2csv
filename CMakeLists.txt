cmake_minimum_required( VERSION 3.0 )

project( pep2csv-project VERSION 1.0 )

set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_BUILD_TYPE Debug )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/ )

# Add and build ANTLR parser generator
set( ANTLR_CPP_DIR third-party/antlr4/runtime/Cpp/ )
add_subdirectory( ${ANTLR_CPP_DIR} )
# Generate grammar
execute_process(
        COMMAND bash -c "${PROJECT_SOURCE_DIR}/grammar/generate.sh"
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/grammar
        COMMAND_ECHO STDOUT
)

# Add csv-parser
add_subdirectory( third-party/csv-parser )

include_directories( ${ANTLR_CPP_DIR}/runtime/src third-party/csv-parser/include )
# Group generated sources
set( PEP_TRACE_PARSER_DIR ${PROJECT_SOURCE_DIR}/src/parser )
set( PEP_TRACE_SOURCES
        ${PEP_TRACE_PARSER_DIR}/
        ${PEP_TRACE_PARSER_DIR}/pep_traceBaseVisitor.cpp
        ${PEP_TRACE_PARSER_DIR}/pep_traceLexer.cpp
        ${PEP_TRACE_PARSER_DIR}/pep_traceListener.cpp
        ${PEP_TRACE_PARSER_DIR}/pep_traceParser.cpp
        ${PEP_TRACE_PARSER_DIR}/pep_traceVisitor.cpp )


add_executable( pep2csv
        ${PEP_TRACE_SOURCES}
        src/main.cpp
        src/PEPTracesListener.cpp )
target_link_libraries( pep2csv antlr4_static csv )
