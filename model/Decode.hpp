#pragma once
#include "parameters.hpp"
class Decode {
 public:
  bool decode_receive_ins_ready;
  long long decode_to_rcu_cycle;
  ROB_LINE_S evaluate(int decode_pc, long long decode_ins);
  void reset();
};