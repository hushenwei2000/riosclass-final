#pragma once
#include <string>

#include "parameters.hpp"
using namespace std;
class Memory {
 private:

 public:
  // mem[addr][0] is the lowest bit of a number
  char mem[MEM][8];

  void writeSingleBit(int addr, int bitIndex, char bit);
  void writeSingleByte(int addr, char* chars);
  void writeSingleByte(int addr, string chars);
  void write(int addr, char* chars, int length);
  char readSingleBit(int addr, int bitIndex);
  string readSingleByte(int addr);
  string read(int addr, int length);
  void reset();
};