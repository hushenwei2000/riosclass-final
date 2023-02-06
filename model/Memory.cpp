#include "Memory.hpp"

#include <string>
using namespace std;

void Memory::writeSingleBit(int addr, int bitIndex, char bit) {
  this->mem[addr][bitIndex] = bit;
}

void Memory::writeSingleByte(int addr, char* chars) {
  for (int i = 0; i < 8; i++) {
    this->mem[addr][i] = chars[i];
  }
}

void Memory::writeSingleByte(int addr, string chars) {
  for (int i = 0; i < 8; i++) {
    this->mem[addr][i] = chars[i];
  }
}

void Memory::write(int addr, char* chars, int length) {
  // length must in [8, 16, 32, 64]
  for (int i = 0; i < length / 8; i++) {
    this->writeSingleByte(addr + i, chars + i * 8);
  }
}

char Memory::readSingleBit(int addr, int bitIndex) {
  return this->mem[addr][bitIndex];
}

string Memory::readSingleByte(int addr) {
  string ans = "";
  for (int i = 0; i < 8; i++) {
    ans = ans + this->mem[addr][i];
  }
  return ans;
}

string Memory::read(int addr, int length) {
  string ans = "";
  // length must in [8, 16, 32, 64]
  for (int i = 0; i < length / 8; i++) {
    ans = ans + this->readSingleByte(addr + i);
  }
  return ans;
}


void Memory::reset() {
}