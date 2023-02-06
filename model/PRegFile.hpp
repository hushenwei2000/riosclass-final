#pragma once
#include "parameters.hpp"

class PRegFile {
 public:
  long long registers[REG_SIZE];
  int rs1_read_addr;
  int rs2_read_addr;
  int alu_write_prf_addr;
  int lsu_write_prf_addr;
  long long alu_write_prf_data;
  long long lsu_write_prf_data;
  bool registers_p[REG_SIZE];
  int commit_update_prf;
  long long Read_PRF_data(int addr);
  bool Read_PRF_P(int addr);
  void Alu_Update_Prf(int alu_write_prf_addr, long long alu_write_prf_data);
  void Lsu_Update_Prf(int lsu_write_prf_addr, long long lsu_write_prf_data);
  void Commit_Update_Prf(int commit_update_prf);
  void reset();
};