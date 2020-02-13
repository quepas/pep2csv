#!/bin/sh
# Package name: -package arrr_ay
java -cp ../third-party/antlr4-bin/antlr-4.8-complete.jar org.antlr.v4.Tool -Dlanguage=Cpp -listener -visitor -o ../src/parser/ pep_trace.g4