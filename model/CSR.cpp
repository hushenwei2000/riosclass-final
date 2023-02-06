#include "CSR.hpp"
#include "PRegFile.hpp"
#include "Rename.hpp"

void CSR::evaluate(PRegFile* PRF, Rename* rename) {
  csr_req_ready = 0;
  csr_current_cycle = cycle;
  if (csr.basic_ins == 873631859) {
    mepc = PRF->Read_PRF_data(rename->rename_rd[5]);
  }
  if (csr.basic_ins == 810717299) {
    mtvec = PRF->Read_PRF_data(rename->rename_rd[5]);
  }
  if (csr.basic_ins == 874524531) {
    PRF->registers[csr.basic_rename_rd] = mcause;
    PRF->registers_p[csr.basic_rename_rd] = 1;
  }
  if (csr.is_mret) {
    csr_pc = mepc;
  }
  if (csr.is_ecall) {
    csr_pc = mtvec;
  }
}

void CSR::reset() {
  csr_req_ready = 1;
  csr_current_cycle = -10;

}