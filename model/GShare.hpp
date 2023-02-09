#pragma once
#include "parameters.hpp"

class GShare {
public:
    int PHT[16];
    int GHR;
    bool prev_taken;
    int hitNum;
    int notHitNum;
    FILE* logFile;
    GShare(FILE* f);
    void reset();
    bool GSahre_predict(int pc);
    void PHT_update(int pc);
    void GHR_update();
    void dump();
};