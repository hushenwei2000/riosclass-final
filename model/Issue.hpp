#pragma once
#include "parameters.hpp"
#include "ROB.hpp"
#include "Rename.hpp"
#include "LSU.hpp"
#include "ALU.hpp"
#include "CSR.hpp"

class Issue {
 public:
  bool iss_alu;
  bool iss_lsu;
  bool iss_csr;
  bool commit_directly;

  int iss_prt;
  ROB_LINE_S iss_rob_line;

  void evaluate(ROB* rob_obj);
  void ISS_CHECK(ROB* rob_obj, Rename* rename_obj, PRegFile* prf_obj, LSU* lsu_obj, ALU* alu_obj, CSR* csr_obj);

};