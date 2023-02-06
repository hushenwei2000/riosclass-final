/*
 * A Memory Simulator Similar to X86 Virtual Memory
 * Created by He, Hao at 2019-3-11
 */

#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <cstdint>
#include <cstdio>
#include <string>

#include "Cache.hpp"
#include "Memory.hpp"

class Cache;
class Memory;

class MemoryManager {
 public:
  MemoryManager();
  ~MemoryManager();

  bool writeSingleByte(int addr, char* chars, uint32_t *cycles = nullptr);
  bool write(int addr, char* chars, int length, uint32_t *cycles = nullptr);
  std::string readSingleByte(int addr, uint32_t *cycles = nullptr);
  std::string read(int addr, int length, uint32_t *cycles = nullptr);

  void printInfo();
  void printStatistics();

  std::string dumpMemory();

  void setCache(Cache *cache);

 private:
  uint32_t getFirstEntryId(uint32_t addr);
  uint32_t getSecondEntryId(uint32_t addr);
  uint32_t getPageOffset(uint32_t addr);
  bool isAddrExist(uint32_t addr);

  Memory *memory;
  Cache *cache;
};

#endif