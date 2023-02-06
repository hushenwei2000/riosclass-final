#include "Issue.hpp"
#include "Utils.hpp"

#include <iostream>
using namespace std;

void Issue::evaluate(ROB* rob_obj) {
  rob_obj->rob_head_iss = (rob_obj->rob_head_iss + 1) % ROB_DEPTH;
  rob_obj->CHECK_ROB();
}

void Issue::ISS_CHECK(ROB* rob_obj, Rename* rename_obj, PRegFile* prf_obj, LSU* lsu_obj, ALU* alu_obj, CSR* csr_obj) {
  iss_prt = (rob_obj->rob_head_iss + 1) % ROB_DEPTH;
  iss_rob_line = rob_obj->rob[iss_prt];
  cout << "iss check pc ";
  Utils::dumpPC(iss_rob_line.basic_pc);
  iss_lsu = 0;
  iss_alu = 0;
  commit_directly = 0;
  iss_csr = 0;
  if (iss_rob_line.is_lsu) {
    if (iss_rob_line.basic_use_rs1 &
        (prf_obj->Read_PRF_P(rename_obj->rename_rd[iss_rob_line.basic_rs1]) == 1)) {
      iss_lsu = 1;
    } else {
      iss_lsu = 0;
    }
    if (iss_rob_line.basic_use_rs2 &
        (prf_obj->Read_PRF_P(rename_obj->rename_rd[iss_rob_line.basic_rs2]) == 1)) {
      iss_lsu = 1;
    } else {
      iss_lsu = 0;
    }
    if (iss_rob_line.lsu_store) {
      if (iss_prt != rob_obj->rob_tail) {
        iss_lsu = 0;
      }
    }
    if (!lsu_obj->dcache_req_ready) {
      iss_lsu = 0;
    }
  }
  if (iss_rob_line.is_alu) {
    iss_alu = 1;
    if (iss_rob_line.basic_use_rs1 &
        prf_obj->Read_PRF_P(rename_obj->rename_rd[iss_rob_line.basic_rs1]) != 1) {
      iss_alu = 0;
      cout << "rs1 stop" << rename_obj->rename_rd[iss_rob_line.basic_rs1] << endl;
    }

    if (iss_rob_line.basic_use_rs2 &
        prf_obj->Read_PRF_P(rename_obj->rename_rd[iss_rob_line.basic_rs2]) != 1) {
      iss_alu = 0;
      cout << "rs2 stop" << rename_obj->rename_rd[iss_rob_line.basic_rs2] << endl;
    }

    if (!alu_obj->alu_ready) {
      iss_alu = 0;
    }
  }

  if ((iss_rob_line.is_csr & iss_rob_line.csr_no_use) | iss_rob_line.is_nop |
      iss_rob_line.is_fence) {
    cout << "commit_directly" << endl;
    commit_directly = 1;
    rob_obj->rob[(rob_obj->rob_head_iss + 1) % ROB_DEPTH].finish = 1;
  }
  if ((iss_rob_line.is_mret) | (iss_rob_line.is_ecall)) {
    iss_csr = 1;
    commit_directly = 0;
    if (!csr_obj->csr_req_ready) {
      iss_csr = 0;
    }
  }
  if (iss_rob_line.is_csr & !iss_rob_line.csr_no_use) {
    iss_csr = 1;
    if (iss_rob_line.basic_use_rs1 &
        prf_obj->Read_PRF_P(rename_obj->rename_rd[iss_rob_line.basic_rs1]) != 1) {
      iss_csr = 0;
    }
    if (!csr_obj->csr_req_ready) {
      iss_csr = 0;
    }
  }
  if (rob_obj->rob_head_iss == rob_obj->rob_tail) {
    iss_alu = 0;
    iss_lsu = 0;
    iss_csr = 0;
  }
}