grammar pep_trace;

NEWLINE : '\r'?'\n';

fragment DIGIT : [0-9];
fragment LETTER : [a-zA-Z];
fragment COLON : ':';

INT : DIGIT+;
EVENT : LETTER (LETTER | '_' | COLON | DIGIT)*;
TRACE_PROPERTY : COLON (LETTER | DIGIT | '_')+;

// Grammar rules
pep: trace* ;

trace
    : '@trace_start' trace_properties NEWLINE
      '@perf_events:' events_list NEWLINE
      trace_values NEWLINE
      '@trace_end';

trace_properties: TRACE_PROPERTY (TRACE_PROPERTY)*;
events_list: EVENT (',' EVENT)*;
trace_values: row_values (NEWLINE row_values)*;

row_values: INT (',' INT)*;
