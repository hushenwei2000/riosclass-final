#include "ICache.hpp"

ICache::ICache(Memory *memory, Policy policy, FILE* f, Cache *lowerCache, bool writeBack, bool writeAllocate) : Cache(memory, policy, f, lowerCache, writeBack, writeAllocate) {}

ICache::~ICache() {};

void ICache::Fetch_INS_FROM_ICACHE(int pc) {
  icache_req_ready = 0;
  icache_current_cycle = cycle;
  ins_from_icache = "";
  // for (int j = pc + 3; j >= pc; j--) {
  //   for (int i = 7; i >= 0; i--) {
  //     string s(1, this->memory->mem[j][i]);
  //     ins_from_icache = ins_from_icache + s;
  //   }
  // }
  for (int j = pc + 3; j >= pc; j--) {
    ins_from_icache = ins_from_icache + this->getByte(j);
  }
  ins_pc = pc;
  this->printStatistics();
}

void ICache::reset() {
  icache_req_ready = 1;
  icache_ins_buffer_current_cycle = -10;
  icache_current_cycle = -10;
}
