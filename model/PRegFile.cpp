#include "PRegFile.hpp"
#include "stdio.h"
#include "parameters.hpp"

long long PRegFile::Read_PRF_data(int addr) { return registers[addr]; }
bool PRegFile::Read_PRF_P(int addr) { return registers_p[addr]; }
void PRegFile ::Alu_Update_Prf(int alu_write_prf_addr,
                               long long alu_write_prf_data) {
  registers[alu_write_prf_addr] = alu_write_prf_data;
  registers_p[alu_write_prf_addr] = 1;
}

void PRegFile ::Lsu_Update_Prf(int lsu_write_prf_addr,
                               long long lsu_write_prf_data) {
  registers[lsu_write_prf_addr] = lsu_write_prf_data;
  registers_p[lsu_write_prf_addr] = 1;
}
void PRegFile ::Commit_Update_Prf(int commit_update_prf) {
  registers_p[commit_update_prf] = 0;
}
void PRegFile::reset() {
  registers_p[0] = 1;
  registers[0] = 0;
  for (int i = 1; i < REG_SIZE; i++) {
    registers[i] = 0;
    registers_p[i] = 0;
  }
}