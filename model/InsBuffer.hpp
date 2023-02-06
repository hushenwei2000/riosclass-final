#pragma once
#include "parameters.hpp"

class InsBuffer {
 public:
  int ins_buffer_pc[INS_BUFFER_SIZE];
  long long ins_buffer_ins[INS_BUFFER_SIZE];
  bool ins_buffer_full;
  bool ins_buffer_empty;
  int ins_buffer_head;
  int ins_buffer_tail;
  int ins_buffer_to_decode_pc;
  long long ins_buffer_to_decode_ins;
  void CHECK_INS_BUFFER();
  void WRITE_INS_BUFFER(int ins_pc, long long ins);
  void READ_INS_BUFFER();
  void reset();
};