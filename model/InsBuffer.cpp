#include "InsBuffer.hpp"

#include <string>
#include <iostream>

#include "parameters.hpp"
using namespace std;

void InsBuffer::CHECK_INS_BUFFER() {
  if (ins_buffer_tail == ins_buffer_head) {
    ins_buffer_empty = 1;
  } else {
    ins_buffer_empty = 0;
  }
  if (((ins_buffer_tail + 1) % INS_BUFFER_SIZE) == ins_buffer_head) {
    ins_buffer_full = 1;
  } else {
    ins_buffer_full = 0;
  }
}

void InsBuffer::WRITE_INS_BUFFER(int ins_pc, long long ins) {
  ins_buffer_tail = (ins_buffer_tail + 1) % INS_BUFFER_SIZE;
  ins_buffer_pc[ins_buffer_tail] = ins_pc;
  ins_buffer_ins[ins_buffer_tail] = ins;
  this->dump();
}

void InsBuffer::READ_INS_BUFFER() {
  ins_buffer_head = (ins_buffer_head + 1) % INS_BUFFER_SIZE;
  ins_buffer_to_decode_pc = ins_buffer_pc[ins_buffer_head];
  ins_buffer_to_decode_ins = ins_buffer_ins[ins_buffer_head];
}

void InsBuffer::reset() {
  for (int i = 0; i < INS_BUFFER_SIZE; i++) {
    ins_buffer_pc[i] = 0;
    ins_buffer_ins[i] = 0;
  }
  ins_buffer_full = 0;
  ins_buffer_empty = 1;
  ins_buffer_head = 0;
  ins_buffer_tail = 0;
}

void InsBuffer::dump() {
  cout << "-------- InsBuffer size = " << (ins_buffer_tail - ins_buffer_head + INS_BUFFER_SIZE) % INS_BUFFER_SIZE << " --------" << endl;
}