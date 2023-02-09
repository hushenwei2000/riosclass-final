#include "Cache.hpp"

class ICache : public Cache {
public:
    ICache(Memory *memory, Policy policy, FILE* f, Cache *lowerCache = nullptr,
        bool writeBack = true, bool writeAllocate = true);
    ~ICache();
    int ins_pc;

    bool icache_req_ready;
    bool icache_resp_valid;
    string ins_from_icache;
    long long icache_to_ins_buffer;
    long long icache_current_cycle;
    int icache_to_ins_buffer_pc;
    void Fetch_INS_FROM_ICACHE(int pc);
    long long icache_ins_buffer_current_cycle;
    void reset();
};