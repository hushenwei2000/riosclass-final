#include "GShare.hpp"
#include "Utils.hpp"

#include <string>
#include <iostream>

#include "parameters.hpp"
using namespace std;

// get pc[l:r] in verilog
int getSlice(int pc, int l, int r) {
    return ((pc >> r) & ((1 << (l - r + 1)) - 1)) & ((1 << GSHARE_WIDTH) - 1);
}

GShare::GShare(FILE* f) {
    logFile = f;
}

void GShare::reset() {
    for(int i = 0; i < PHT_LEN; i++) {
        PHT[i] = 2;
    }
    GHR = 1;
    prev_taken = false;
    hitNum = 0;
    notHitNum = 0;
}

bool GShare::GSahre_predict(int pc) {
    int index = getSlice(pc, GSHARE_WIDTH + 1, 2) ^ GHR;
    cout << "GShare predict: " << index << ", " << (PHT[index] >= 2 ? "True" : "False") << endl; 
    return PHT[index] >= 2 ? true : false;
}

void GShare::GHR_update() {
    cout << "GHR_update! GHR: " << GHR << ", prev_taken: " << prev_taken << endl;
    GHR = (getSlice(GHR, GSHARE_WIDTH - 2, 0) << 1) | prev_taken;
}

void GShare::PHT_update(int pc) {
    cout << "PHT_update! prev_taken = " << prev_taken << endl;
    PHT[getSlice(pc, GSHARE_WIDTH + 1, 2) ^ GHR] += prev_taken;
    this->dump();
}

void GShare::dump() {
    fprintf(logFile, "-------- GShare dump --------\n");
    fprintf(logFile, "\t GHR: %d\n", GHR);
    for(int i = 0; i < PHT_LEN; i++) {
        fprintf(logFile, "%d, ", PHT[i]);
    }
    fprintf(logFile, "\n\tHit: %d, Not Hit: %d\n------ GShare dump Over------\n", hitNum, notHitNum);
}
