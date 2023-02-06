#pragma once
#include "parameters.hpp"
#include "PRegFile.hpp"
#include "Rename.hpp"
class CSR {
 public:
  long long mepc = 0;
  long long mtvec = 0;
  int mcause = 8;
  long long csr_current_cycle;
  bool csr_req_ready = 0;
  ROB_LINE_S csr;
  int csr_pc;
  void evaluate(PRegFile* PRF, Rename* rename);
  void reset();
};