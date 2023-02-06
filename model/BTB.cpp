#include "BTB.hpp"

CHECK_BTB_S BTB::CHECK_BTB(int pc) {
  check_btb.btb_hit = 0;
  for (int i = 0; i < BTB_SIZE; i++) {
    if (btb_pc[i] == pc) {
      check_btb.btb_hit = 1;
      check_btb.btb_pc = btb_pc_next[i];
    }
  }
  return check_btb;
}

void BTB::UPDATE_BTB(int update_pc, int update_pc_target) {
  bool btb_hit;
  int btb_hit_pos;
  btb_hit = 0;
  btb_hit_pos = 0;
  for (int i = 0; i < BTB_SIZE; i++) {
    if (btb_pc[i] == update_pc) {
      btb_hit = 1;
      btb_hit_pos = i;
    }
  }
  if (btb_hit == 1) {
    btb_pc_next[btb_hit_pos] = update_pc_target;
  } else {
    btb_pc[btb_update_count % BTB_SIZE] = update_pc;
    btb_pc_next[btb_update_count % BTB_SIZE] = update_pc_target;
    btb_update_count++;
  }
}

void BTB::reset() {
   btb_update_count = 0;
  for (int i = 0; i < BTB_SIZE; i++) {
    btb_pc[i] = -1;
    btb_pc_next[i] = 0;
  }
}