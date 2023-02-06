#pragma once
#include "PRegFile.hpp"
#include "Rename.hpp"
#include "parameters.hpp"

class ALU {
public:
  long long alu_current_cycle;
  bool alu_ready;
  ROB_LINE_S alu;
  void evaluate(PRegFile* PRF, Rename* rename);
  void reset();
};