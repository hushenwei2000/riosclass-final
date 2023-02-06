#include "ALU.hpp"
#include "Utils.hpp"

#include <iostream>
using namespace std;
void ALU::evaluate(PRegFile* PRF, Rename* rename) {
  alu_ready = 0;
  alu_current_cycle = cycle;
  if (alu.basic_use_rs1) {
    alu.basic_rs1_data = PRF->Read_PRF_data(rename->rename_rd[alu.basic_rs1]);
    alu.add_sub_alu_1 = alu.basic_rs1_data;
  }
  if (alu.basic_use_rs2) {
    alu.basic_rs2_data = PRF->Read_PRF_data(rename->rename_rd[alu.basic_rs2]);
    alu.add_sub_alu_2 = alu.basic_rs2_data;
  }

  // AUI
  if (alu.is_lui) {
    alu.alu_result = alu.lui_imm;
  }
  // J
  if (alu.is_add & alu.is_jump) {
    alu.basic_pc_next = alu.add_sub_alu_1 + alu.add_sub_alu_2;
    alu.j_b_success = 1;
    alu.alu_result = alu.basic_pc + 4;
  }
  // B
  if (alu.is_add & alu.is_branch) {
    if (alu.branch_func == "000") {
      if (alu.basic_rs1_data == alu.basic_rs2_data) {
        alu.j_b_success = 1;
        alu.basic_pc_next = alu.basic_pc + alu.branch_imm;
      }
    }
    if (alu.branch_func == "001") {
      if (alu.basic_rs1_data != alu.basic_rs2_data) {
        alu.j_b_success = 1;
        alu.basic_pc_next = alu.basic_pc + alu.branch_imm;
      }
    }
    if (alu.branch_func == "100" | alu.branch_func == "110") {
      if (alu.basic_rs1_data < alu.basic_rs2_data) {
        alu.j_b_success = 1;
        alu.basic_pc_next = alu.basic_pc + alu.branch_imm;
      }
    }
    if (alu.branch_func == "101" | alu.branch_func == "111") {
      if (alu.basic_rs1_data >= alu.basic_rs2_data) {
        alu.j_b_success = 1;
        alu.basic_pc_next = alu.basic_pc + alu.branch_imm;
      }
    }
    cout << "rs1" << alu.basic_rs1_data << "rs2" << alu.basic_rs2_data << endl;
    cout << "alu branch b_success is " << alu.j_b_success << endl;
  }
  // ADD

  if (alu.is_add & !alu.is_branch & !alu.is_jump) {
    if (!alu.basic_is_half) {
      alu.alu_result = alu.add_sub_alu_1 + alu.add_sub_alu_2;
    }
    if (alu.basic_is_half) {
      alu.alu_result = alu.add_sub_alu_1 + alu.add_sub_alu_2;
      if (alu.alu_result < -2147483648) {
        alu.alu_result = -alu.add_sub_alu_1 + alu.add_sub_alu_2;
      }
    }
  }
  // SUB
  if (alu.is_sub & !alu.is_branch & !alu.is_jump) {
    if (!alu.basic_is_half) {
      alu.alu_result = alu.add_sub_alu_1 - alu.add_sub_alu_2;
    }
    if (alu.basic_is_half) {
      alu.alu_result = alu.add_sub_alu_1 - alu.add_sub_alu_2;

      STORE_DATA_DWORD add_64;
      add_64 = Utils::toBinary_d(alu.alu_result);

      string add_32;
      for (int i = 30; i >= 0; i--) {
        string s(1, add_64[i]);
        add_32 = add_32 + s;
      }
      // alu.alu_result = (-1) * (add_64.ins[31]-48) * pow(2,31) +
      // toInt(add_32,31);
      if (add_64[31] == '1') {
        add_32 = Utils::reverseString(add_32, 31);
        alu.alu_result = -(Utils::toIntLittleEndian(add_32, 31) + 1);
      } else {
        alu.alu_result = Utils::toIntLittleEndian(add_32, 31);
      }
    }
  }
  // AND
  if (alu.is_and == 1) {
    if (alu.basic_use_rs2) {
      alu.alu_result = alu.basic_rs1_data & alu.basic_rs2_data;
    } else {
      alu.alu_result = alu.basic_rs1_data & alu.op_imm;
    }
  }
  // OR
  if (alu.is_or == 1) {
    if (alu.basic_use_rs2) {
      alu.alu_result = alu.basic_rs1_data | alu.basic_rs2_data;
    } else {
      alu.alu_result = alu.basic_rs1_data | alu.op_imm;
    }
  }
  // XOR
  if (alu.is_xor == 1) {
    if (alu.basic_use_rs2) {
      alu.alu_result = alu.basic_rs1_data ^ alu.basic_rs2_data;
    } else {
      alu.alu_result = alu.basic_rs1_data ^ alu.op_imm;
    }
  }
  // SHIFT
  if (alu.is_shift) {
    if (alu.shift_type == 0 & alu.basic_use_rs2) {
      alu.alu_result = alu.basic_rs1_data << alu.basic_rs2_data;
    }
    if (alu.shift_type == 1 & alu.basic_use_rs2) {
      alu.alu_result =
          (unsigned long long)alu.basic_rs1_data >> alu.basic_rs2_data;
    }
    if (alu.shift_type == 2 & alu.basic_use_rs2) {
      alu.alu_result = alu.basic_rs1_data >> alu.basic_rs2_data;
    }
    if (alu.shift_type == 0 & !alu.basic_use_rs2) {
      cout << "alu slli" << endl;
      cout << alu.shift << endl;
      alu.alu_result = alu.basic_rs1_data << alu.shift;
    }
    if (alu.shift_type == 1 & !alu.basic_use_rs2) {
      alu.alu_result = (unsigned long long)alu.basic_rs1_data >> alu.shift;
    }
    if (alu.shift_type == 2 & !alu.basic_use_rs2) {
      alu.alu_result = alu.basic_rs1_data >> alu.shift;
    }
    if (alu.shift_type == 0 & alu.basic_use_rs2 & alu.basic_is_half) {
      alu.alu_result = alu.basic_rs1_data << alu.basic_rs2_data;
    }
    if (alu.shift_type == 1 & alu.basic_use_rs2 & alu.basic_is_half) {
      alu.alu_result =
          (unsigned long long)alu.basic_rs1_data >> alu.basic_rs2_data;
    }
    if (alu.shift_type == 2 & alu.basic_use_rs2 & alu.basic_is_half) {
      alu.alu_result = alu.basic_rs1_data >> alu.basic_rs2_data;
    }
    if (alu.shift_type == 0 & !alu.basic_use_rs2 & alu.basic_is_half) {
      alu.alu_result = alu.basic_rs1_data << alu.shift;
    }
    if (alu.shift_type == 1 & !alu.basic_use_rs2 & alu.basic_is_half) {
      alu.alu_result = (unsigned long long)alu.basic_rs1_data >> alu.shift;
    }
    if (alu.shift_type == 2 & !alu.basic_use_rs2 & alu.basic_is_half) {
      alu.alu_result = alu.basic_rs1_data >> alu.shift;
    }
    if (alu.basic_is_half) {
      STORE_DATA_DWORD shift_64;
      shift_64 = Utils::toBinary_d(alu.alu_result);
      string shift_32;
      for (int i = 30; i >= 0; i--) {
        string s(1, shift_64[i]);
        shift_32 = shift_32 + s;
      }
      // alu.alu_result = (-1) * (shift_64.ins[31]-48) * pow(2,31) +
      // toInt(shift_32,31);
      if (shift_64[31] == '1') {
        shift_32 = Utils::reverseString(shift_32, 31);
        alu.alu_result = -(Utils::toIntLittleEndian(shift_32, 31) + 1);
      } else {
        alu.alu_result = Utils::toIntLittleEndian(shift_32, 31);
      }
    }
  }
  // SLT
  if (alu.is_slt & alu.basic_use_rs2) {
    if (alu.basic_rs1_data < alu.basic_rs2_data) {
      alu.slt_result = 1;
    }
  }
  if (alu.is_slt & !alu.basic_use_rs2) {
    if (alu.basic_rs1_data < alu.slt_imm) {
      alu.slt_result = 1;
    }
  }
}

void ALU::reset() {
  alu_ready = 1;
  alu_current_cycle = -10;
}