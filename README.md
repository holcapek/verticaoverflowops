### Vertica oveflow operators (functions) 

In case you need to check for over- and underflow.
 
    CREATE OR REPLACE LIBRARY OverflowOpsLib
      AS '/tmp/liboverflowops.so'
      LANGUAGE 'C++';
   
    CREATE OR REPLACE FUNCTION add
      AS LANGUAGE 'C++'
      NAME 'OverflowAddFactory'
      LIBRARY OverflowOpsLib
      FENCED;
    
    \set MAX_VINT 9223372036854775807
    
    SELECT add(:MAX_VINT, 1) AS result;
    ERROR 3399:  Failure in UDx RPC call InvokeProcessBlock(): Error calling processBlock()
    in User Defined Object [add] at [overflowops.cpp:29], error code: 0, message: int out of range
    
    SELECT add(-:MAX_VINT, -1) AS result;
    ERROR 3399:  Failure in UDx RPC call InvokeProcessBlock(): Error calling processBlock()
    in User Defined Object [add] at [overflowops.cpp:29], error code: 0, message: int out of range

  
