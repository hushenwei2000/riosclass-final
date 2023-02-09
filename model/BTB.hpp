#pragma once
#include "parameters.hpp"
#include "GShare.hpp"
class BTB {
 public:
  BTB(FILE* f);
  GShare gshare;
  int btb_pc[BTB_SIZE];
  int btb_pc_next[BTB_SIZE];
  int btb_update_count;
  CHECK_BTB_S check_btb;
  CHECK_BTB_S CHECK_BTB(int pc);
  void UPDATE_BTB(int update_pc, int update_pc_target);
  void UPDATE_BTB_TAKEN(int pc);
  void reset();
};