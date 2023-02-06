#include <iostream>
#include "ROB.hpp"
#include "Utils.hpp"

using namespace std;

void ROB::CHECK_ROB() {
  if (rob_head_iss == rob_tail) {
    rob_empty = 1;
  } else {
    rob_empty = 0;
  }
  if (((rob_tail + 1) % ROB_DEPTH) == rob_head_commit) {
    rob_full = 1;
  } else {
    rob_full = 0;
  }
}

void ROB::IN_ROB(ROB_LINE_S rob_line) {
  cout << " in rob " << rob_line.basic_pc;
  Utils::dumpPC(rob_line.basic_pc);
  rob_tail = (rob_tail + 1) % ROB_DEPTH;
  rob_line.rob_index = rob_tail;
  rob[rob_tail] = rob_line;
  CHECK_ROB();
}

void ROB::COMMIT() {
  rob[(rob_head_commit + 1) % ROB_DEPTH].finish = 0;
  rob_head_commit = (rob_head_commit + 1) % ROB_DEPTH;
  CHECK_ROB();
}