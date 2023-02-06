#pragma once
#include "Memory.hpp"
#include "PRegFile.hpp"
#include "Rename.hpp"
#include "parameters.hpp"
class LSU {
public:
  bool dcache_req_ready;
  long long dcache_current_cycle;
  int addr;
  long long load_data;
  ROB_LINE_S lsq;
  void LSQ_SEND_TO_DCACHE(PRegFile* PRF, Rename* rename_obj, Memory* mem);
  void reset();
};