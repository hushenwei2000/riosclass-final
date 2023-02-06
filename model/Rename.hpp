#pragma once
#include <iostream>
#include "parameters.hpp"
class Rename {
public:
    // FREE_LIST
    int free_list[FREE_LIST];
    int free_list_head = 0;
    int free_list_tail = 0;
    // RENAME
    int rename_rd[REG_SIZE];
    ROB_LINE_S ins_after_rename;

    void evaluate(ROB_LINE_S ins_after_decode);
    void reset();
};