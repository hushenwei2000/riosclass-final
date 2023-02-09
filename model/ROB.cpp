#include <iostream>
#include "ROB.hpp"
#include "Utils.hpp"
#include<string>
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

string ROB::getType(int index) {
  ROB_LINE_S line = rob[index];
  string ans = "";
  if(line.lsu_load) {
    ans = "load";
  }else if(line.lsu_store) {
    ans = "store";
  }else if(line.is_add) {
    ans = "add";
  }else if(line.is_sub) {
    ans = "sub";
  }else if(line.is_fence) {
    ans = "fence";
  }else if(line.is_jump) {
    ans = "jump";
  }else if(line.is_lui) {
    ans = "lui";
  }else if(line.is_branch) {
    ans = line.branch_func;
  }else if(line.is_xor) {
    ans = "xor";
  }else if(line.is_or) {
    ans = "or";
  }else if(line.is_and) {
    ans = "and";
  }else if(line.is_shift) {
    ans = "shift";
  }else if(line.is_slt) {
    ans = "slt";
  }
  return ans;
}