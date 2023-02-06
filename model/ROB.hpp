#pragma once
#include "parameters.hpp"
class ROB {
public:
    // ROB
    ROB_LINE_S rob[ROB_DEPTH];
    bool rob_full;
    bool rob_empty;
    int rob_head_commit;
    int rob_head_iss;
    int rob_tail;


    void CHECK_ROB();

    void IN_ROB(ROB_LINE_S rob_line);

    void COMMIT();
};