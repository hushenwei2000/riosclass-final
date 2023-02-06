/*
 * Created by He, Hao at 2019-3-11
 */

#include "MemoryManager.h"

#include <cstdio>
#include <string>

#include "Utils.h"


MemoryManager::MemoryManager() {
  this->cache = nullptr;
  this->memory = new Memory();
}

MemoryManager::~MemoryManager() {
  delete this->cache;
  delete this->memory;
}

bool MemoryManager::writeSingleByte(int addr, char* chars, uint32_t *cycles = nullptr) {
  std::string val = Utils::charArrayToString(chars, 8);

  if (this->cache != nullptr) {
    this->cache->setByte(addr, val, cycles);
    return true;
  }
  this->memory->writeSingleByte(addr, chars);
  return true;
}

bool MemoryManager::write(int addr, char* chars, int length, uint32_t *cycles = nullptr) {
  for (int i = 0; i < length / 8; i++) {
    this->writeSingleByte(addr + i, chars + i * 8);
  }
}

bool MemoryManager::setByteNoCache(uint32_t addr, std::string val) {
  if (!this->isAddrExist(addr)) {
    dbgprintf("Byte write to invalid addr 0x%x!\n", addr);
    return false;
  }
  this->memory->writeSingleByte(addr, chars);

  return true;
}

std::string MemoryManager::readSingleByte(int addr, char* chars, uint32_t *cycles = nullptr) {
  if (this->cache != nullptr) {
    return this->cache->getByte(addr, cycles);
  }
  return this->memory->readSingleByte(addr);
}

std::string read(int addr, int length) {
  string ans = "";
  // length must in [8, 16, 32, 64]
  for (int i = 0; i < length / 8; i++) {
    ans = ans + this->readSingleByte(addr + i);
  }
  return ans;
}

uint8_t MemoryManager::getByteNoCache(uint32_t addr) {
  return this->memory->readSingleByte(addr);
}


void MemoryManager::printInfo() {
  // TODO
}

void MemoryManager::printStatistics() {
  printf("---------- CACHE STATISTICS ----------\n");
  this->cache->printStatistics();
}

std::string MemoryManager::dumpMemory() {
  // TODO
  std::string dump;
  return dump;
}

uint32_t MemoryManager::getFirstEntryId(uint32_t addr) {
  return (addr >> 22) & 0x3FF;
}

uint32_t MemoryManager::getSecondEntryId(uint32_t addr) {
  return (addr >> 12) & 0x3FF;
}

uint32_t MemoryManager::getPageOffset(uint32_t addr) { return addr & 0xFFF; }

bool MemoryManager::isAddrExist(uint32_t addr) {
  // TODO
  return true;
}

void MemoryManager::setCache(Cache *cache) { this->cache = cache; }