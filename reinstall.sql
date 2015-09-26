\set ON_ERROR_STOP on

CREATE OR REPLACE LIBRARY OverflowOpsLib
	AS '/tmp/liboverflowops.so'
	LANGUAGE 'C++';

CREATE OR REPLACE FUNCTION overflowadd
	AS LANGUAGE 'C++'
	NAME 'OverflowAddFactory'
	LIBRARY OverflowOpsLib
	FENCED;

\pset expanded

SELECT *
FROM user_libraries
WHERE lib_name = 'OverflowOpsLib';

SELECT *
FROM user_functions
WHERE function_name = 'overflowadd';
