#include "Decode.hpp"
#include "Utils.hpp"

#include <string>
#include <iostream>

#include "parameters.hpp"
using namespace std;
ROB_LINE_S Decode::evaluate(int decode_pc, long long decode_ins) {
  ROB_LINE_S ins_after_decode;
  INSTRUCTION ins = Utils::toInstruction(decode_ins);

  string last_seven_ins;
  string function_three_bit;
  string rs1_s, rs2_s, rd_s;
  int rs1, rs2, rd;
  long long imm_u, imm_j, imm_i, imm_s, imm_b;
  string imm_u_s, imm_j_s, imm_s_s, imm_b_s, imm_i_s;
  for (int i = 6; i >= 0; i--) {
    string s(1, ins[i]);
    last_seven_ins = last_seven_ins + s;
  }
  for (int i = 14; i >= 12; i--) {
    string s(1, ins[i]);
    function_three_bit = function_three_bit + s;
  }
  for (int i = 11; i >= 7; i--) {
    string s(1, ins[i]);
    rd_s = rd_s + s;
  }
  for (int i = 19; i >= 15; i--) {
    string s(1, ins[i]);
    rs1_s = rs1_s + s;
  }
  for (int i = 24; i >= 20; i--) {
    string s(1, ins[i]);
    rs2_s = rs2_s + s;
  }
  for (int i = 24; i >= 20; i--) {
    string s(1, ins[i]);
    rs2_s = rs2_s + s;
  }

  rd = Utils::toIntBigEndian(rd_s, 5);
  rs1 = Utils::toIntBigEndian(rs1_s, 5);
  rs2 = Utils::toIntBigEndian(rs2_s, 5);
  for (int i = 30; i >= 12; i--) {
    string s(1, ins[i]);
    imm_u_s = imm_u_s + s;
  }
  imm_u_s = imm_u_s + "000000000000";
  // imm_u = (-1) * (ins[31]-48) * pow(2,31) + toInt(imm_u_s,31);
  if (ins[31] == '1') {
    imm_u_s = Utils::reverseString(imm_u_s, 31);
    imm_u = -(Utils::toIntBigEndian(imm_u_s, 31) + 1);
  } else {
    imm_u = Utils::toIntBigEndian(imm_u_s, 31);
  }
  cout << "imm_u" << imm_u << endl;
  for (int i = 19; i >= 12; i--) {
    string s(1, ins[i]);
    imm_j_s = imm_j_s + s;
  }
  string s(1, ins[20]);
  imm_j_s = imm_j_s + s;
  for (int i = 30; i >= 21; i--) {
    string s(1, ins[i]);
    imm_j_s = imm_j_s + s;
  }
  imm_j_s = imm_j_s + '0';
  // imm_j = (-1) * (ins[31]-48) * pow(2,20) + toInt(imm_j_s,20);
  if (ins[31] == '1') {
    imm_j_s = Utils::reverseString(imm_j_s, 20);
    imm_j = -(Utils::toIntBigEndian(imm_j_s, 20) + 1);
  } else {
    imm_j = Utils::toIntBigEndian(imm_j_s, 20);
  }
  for (int i = 30; i >= 20; i--) {
    string s(1, ins[i]);
    imm_i_s = imm_i_s + s;
  }

  // imm_i = (-1) * (ins[31]-48) * pow(2,11) + toInt(imm_i_s,11);
  if (ins[31] == '1') {
    imm_i_s = Utils::reverseString(imm_i_s, 11);
    imm_i = -(Utils::toIntBigEndian(imm_i_s, 11) + 1);
  } else {
    imm_i = Utils::toIntBigEndian(imm_i_s, 11);
  }
  cout << "imm_i" << imm_i << endl;
  for (int i = 30; i >= 25; i--) {
    string s(1, ins[i]);
    imm_s_s = imm_s_s + s;
  }
  for (int i = 11; i >= 7; i--) {
    string s(1, ins[i]);
    imm_s_s = imm_s_s + s;
  }
  // imm_s = (-1) * (ins[31]-48) * pow(2,11) + toInt(imm_s_s,11);
  if (ins[31] == '1') {
    imm_s_s = Utils::reverseString(imm_s_s, 11);
    imm_s = -(Utils::toIntBigEndian(imm_s_s, 11) + 1);
  } else {
    imm_s = Utils::toIntBigEndian(imm_s_s, 11);
  }
  string ss(1, ins[7]);
  imm_b_s = imm_b_s + ss;
  for (int i = 30; i >= 25; i--) {
    string s(1, ins[i]);
    imm_b_s = imm_b_s + s;
  }
  for (int i = 11; i >= 8; i--) {
    string s(1, ins[i]);
    imm_b_s = imm_b_s + s;
  }
  imm_b_s = imm_b_s + '0';
  // imm_b = (-1) * (ins[31]-48) * pow(2,12) + toInt(imm_b_s,12);
  if (ins[31] == '1') {
    imm_b_s = Utils::reverseString(imm_b_s, 12);
    imm_b = -(Utils::toIntBigEndian(imm_b_s, 12) + 1);
  } else {
    imm_b = Utils::toIntBigEndian(imm_b_s, 12);
  }
  ins_after_decode.basic_pc = decode_pc;
  ins_after_decode.basic_pc_next = decode_pc + 4;
  ins_after_decode.basic_ins = decode_ins;
  cout << hex << "decode core 0: 0x00000000" << decode_pc << dec << endl;

  // NOP
  if (decode_ins == 19) {
    cout << "decode nop" << endl;
    ins_after_decode.is_nop = 1;
  }
  // CSR
  if (last_seven_ins == "1110011") {
    cout << " csr ins" << decode_ins << endl;
    ins_after_decode.is_csr = 1;
    if ((decode_ins == 873631859) | (decode_ins == 810717299)) {
      cout << "decode csr use" << endl;
      ins_after_decode.csr_no_use = 0;
      ins_after_decode.basic_use_rs1 = 1;
      ins_after_decode.basic_rs1 = 5;
    } else if (decode_ins == 874524531) {
      cout << "decode csr use" << endl;
      ins_after_decode.csr_no_use = 0;
      ins_after_decode.basic_use_rd = 1;
      ins_after_decode.basic_rd = 30;
    } else {
      cout << "decode csr no use" << endl;
      ins_after_decode.csr_no_use = 1;
    }
  }
  // ECALL
  if (decode_ins == 115) {
    cout << "decode ecall" << endl;
    ins_after_decode.is_ecall = 1;
    ins_after_decode.is_csr = 0;
  }
  // MRET
  if (decode_ins == 807403635) {
    cout << "decode mret" << endl;
    ins_after_decode.is_mret = 1;
    ins_after_decode.is_csr = 0;
  }
  // FENCE
  if (decode_ins == 267386895) {
    cout << "decode fence" << endl;
    ins_after_decode.is_fence = 1;
  }
  // LD
  if (last_seven_ins == "0000011") {
    if (function_three_bit == "000") {
      ins_after_decode.basic_use_rd = 1;
      ins_after_decode.basic_use_rs1 = 1;
      ins_after_decode.basic_rd = rd;
      ins_after_decode.basic_rs1 = rs1;
      ins_after_decode.is_lsu = 1;
      ins_after_decode.lsu_load = 1;
      ins_after_decode.lsu_sign = 1;
      ins_after_decode.lsu_load_store_size = 1;
      ins_after_decode.lsu_imm = imm_i;
    }
    if (function_three_bit == "001") {
      ins_after_decode.basic_use_rd = 1;
      ins_after_decode.basic_use_rs1 = 1;
      ins_after_decode.basic_rd = rd;
      ins_after_decode.basic_rs1 = rs1;
      ins_after_decode.is_lsu = 1;
      ins_after_decode.lsu_load = 1;
      ins_after_decode.lsu_sign = 1;
      ins_after_decode.lsu_load_store_size = 2;
      ins_after_decode.lsu_imm = imm_i;
    }
    if (function_three_bit == "010") {
      ins_after_decode.basic_use_rd = 1;
      ins_after_decode.basic_use_rs1 = 1;
      ins_after_decode.basic_rd = rd;
      ins_after_decode.basic_rs1 = rs1;
      ins_after_decode.is_lsu = 1;
      ins_after_decode.lsu_load = 1;
      ins_after_decode.lsu_sign = 1;
      ins_after_decode.lsu_load_store_size = 4;
      ins_after_decode.lsu_imm = imm_i;
    }
    if (function_three_bit == "100") {
      ins_after_decode.basic_use_rd = 1;
      ins_after_decode.basic_use_rs1 = 1;
      ins_after_decode.basic_rd = rd;
      ins_after_decode.basic_rs1 = rs1;
      ins_after_decode.is_lsu = 1;
      ins_after_decode.lsu_load = 1;
      ins_after_decode.lsu_load_store_size = 1;
      ins_after_decode.lsu_imm = imm_i;
    }
    if (function_three_bit == "101") {
      ins_after_decode.basic_use_rd = 1;
      ins_after_decode.basic_use_rs1 = 1;
      ins_after_decode.basic_rd = rd;
      ins_after_decode.basic_rs1 = rs1;
      ins_after_decode.is_lsu = 1;
      ins_after_decode.lsu_load = 1;
      ins_after_decode.lsu_load_store_size = 2;
      ins_after_decode.lsu_imm = imm_i;
    }
    if (function_three_bit == "110") {
      ins_after_decode.basic_use_rd = 1;
      ins_after_decode.basic_use_rs1 = 1;
      ins_after_decode.basic_rd = rd;
      ins_after_decode.basic_rs1 = rs1;
      ins_after_decode.is_lsu = 1;
      ins_after_decode.lsu_load = 1;
      ins_after_decode.lsu_load_store_size = 4;
      ins_after_decode.lsu_imm = imm_i;
    }
    if (function_three_bit == "011") {
      ins_after_decode.basic_use_rd = 1;
      ins_after_decode.basic_use_rs1 = 1;
      ins_after_decode.basic_rd = rd;
      ins_after_decode.basic_rs1 = rs1;
      ins_after_decode.is_lsu = 1;
      ins_after_decode.lsu_load = 1;
      ins_after_decode.lsu_sign = 1;
      ins_after_decode.lsu_load_store_size = 8;
      ins_after_decode.lsu_imm = imm_i;
    }
  }
  // ST
  if (last_seven_ins == "0100011") {
    if (function_three_bit == "000") {
      ins_after_decode.basic_use_rd = 1;
      ins_after_decode.basic_use_rs1 = 1;
      ins_after_decode.basic_use_rs2 = 1;
      ins_after_decode.basic_rd = rd;
      ins_after_decode.basic_rs1 = rs1;
      ins_after_decode.basic_rs2 = rs2;
      ins_after_decode.is_lsu = 1;
      ins_after_decode.lsu_store = 1;
      ins_after_decode.lsu_sign = 1;
      ins_after_decode.lsu_load_store_size = 1;
      ins_after_decode.lsu_imm = imm_s;
    }
    if (function_three_bit == "001") {
      ins_after_decode.basic_use_rd = 1;
      ins_after_decode.basic_use_rs1 = 1;
      ins_after_decode.basic_use_rs2 = 1;
      ins_after_decode.basic_rd = rd;
      ins_after_decode.basic_rs1 = rs1;
      ins_after_decode.basic_rs2 = rs2;
      ins_after_decode.is_lsu = 1;
      ins_after_decode.lsu_store = 1;
      ins_after_decode.lsu_sign = 1;
      ins_after_decode.lsu_load_store_size = 2;
      ins_after_decode.lsu_imm = imm_s;
    }
    if (function_three_bit == "010") {
      ins_after_decode.basic_use_rd = 1;
      ins_after_decode.basic_use_rs1 = 1;
      ins_after_decode.basic_use_rs2 = 1;
      ins_after_decode.basic_rd = rd;
      ins_after_decode.basic_rs1 = rs1;
      ins_after_decode.basic_rs2 = rs2;
      ins_after_decode.is_lsu = 1;
      ins_after_decode.lsu_store = 1;
      ins_after_decode.lsu_sign = 1;
      ins_after_decode.lsu_load_store_size = 4;
      ins_after_decode.lsu_imm = imm_s;
    }
    if (function_three_bit == "011") {
      ins_after_decode.basic_use_rd = 1;
      ins_after_decode.basic_use_rs1 = 1;
      ins_after_decode.basic_use_rs2 = 1;
      ins_after_decode.basic_rd = rd;
      ins_after_decode.basic_rs1 = rs1;
      ins_after_decode.basic_rs2 = rs2;
      ins_after_decode.is_lsu = 1;
      ins_after_decode.lsu_store = 1;
      ins_after_decode.lsu_sign = 1;
      ins_after_decode.lsu_load_store_size = 8;
      ins_after_decode.lsu_imm = imm_s;
    }
  }
  // LUI / LUIPC
  if (last_seven_ins == "0110111") {
    cout << "decode lui" << endl;
    ins_after_decode.basic_use_rd = 1;
    ins_after_decode.basic_rd = rd;
    ins_after_decode.is_lui = 1;
    ins_after_decode.is_alu = 1;
    ins_after_decode.lui_imm = imm_u;
  }
  if (last_seven_ins == "0010111") {
    cout << "decode auipc" << endl;
    ins_after_decode.basic_use_rd = 1;
    ins_after_decode.basic_rd = rd;
    ins_after_decode.is_lui = 1;
    ins_after_decode.is_alu = 1;
    ins_after_decode.lui_imm = imm_u + decode_pc;
  }
  // J
  if (last_seven_ins == "1101111")  // jal
  {
    cout << "decode jal" << endl;
    ins_after_decode.basic_use_rd = 1;
    ins_after_decode.basic_rd = rd;
    ins_after_decode.is_jump = 1;
    ins_after_decode.is_alu = 1;
    ins_after_decode.is_add = 1;
    ins_after_decode.add_sub_alu_1 = imm_j;
    ins_after_decode.add_sub_alu_2 = decode_pc;
  }
  if (last_seven_ins == "1100111")  // jalr
  {
    ins_after_decode.basic_use_rd = 1;
    ins_after_decode.basic_rd = rd;
    ins_after_decode.is_jump = 1;
    ins_after_decode.is_alu = 1;
    ins_after_decode.is_add = 1;
    ins_after_decode.basic_use_rs1 = 1;
    ins_after_decode.basic_rs1 = rs1;
    ins_after_decode.add_sub_alu_2 = imm_i;
  }
  // B
  if (last_seven_ins == "1100011") {
    cout << "decode branch" << endl;
    ins_after_decode.basic_use_rd = 1;
    ins_after_decode.basic_use_rs1 = 1;
    ins_after_decode.basic_use_rs2 = 1;
    ins_after_decode.basic_use_rd = rd;
    ins_after_decode.basic_rs1 = rs1;
    ins_after_decode.basic_rs2 = rs2;
    ins_after_decode.is_add = 1;
    ins_after_decode.is_branch = 1;
    ins_after_decode.is_alu = 1;
    ins_after_decode.branch_imm = imm_b;
    ins_after_decode.branch_func = function_three_bit;
  }

  // ADD/SUB
  if (last_seven_ins == "0110011" & function_three_bit == "000")  // ADD / SUB
  {
    if (ins[30] == '1') {
      ins_after_decode.is_alu = 1;
      ins_after_decode.is_sub = 1;
      ins_after_decode.basic_use_rs1 = 1;
      ins_after_decode.basic_use_rs2 = 1;
      ins_after_decode.basic_use_rd = 1;
      ins_after_decode.basic_rd = rd;
      ins_after_decode.basic_rs1 = rs1;
      ins_after_decode.basic_rs2 = rs2;

    } else {
      ins_after_decode.is_alu = 1;
      ins_after_decode.is_add = 1;
      ins_after_decode.basic_use_rs1 = 1;
      ins_after_decode.basic_use_rs2 = 1;
      ins_after_decode.basic_use_rd = 1;
      ins_after_decode.basic_rd = rd;
      ins_after_decode.basic_rs1 = rs1;
      ins_after_decode.basic_rs2 = rs2;
    }
  }
  if (last_seven_ins == "0111011" & function_three_bit == "000")  // addw / subw
  {
    if (ins[30] == '1') {
      ins_after_decode.is_alu = 1;
      ins_after_decode.is_sub = 1;
      ins_after_decode.basic_is_half = 1;
      ins_after_decode.basic_use_rs1 = 1;
      ins_after_decode.basic_use_rs2 = 1;
      ins_after_decode.basic_use_rd = 1;
      ins_after_decode.basic_rd = rd;
      ins_after_decode.basic_rs1 = rs1;
      ins_after_decode.basic_rs2 = rs2;
    } else {
      ins_after_decode.is_alu = 1;
      ins_after_decode.is_add = 1;
      ins_after_decode.basic_is_half = 1;
      ins_after_decode.basic_use_rs1 = 1;
      ins_after_decode.basic_use_rs2 = 1;
      ins_after_decode.basic_use_rd = 1;
      ins_after_decode.basic_rd = rd;
      ins_after_decode.basic_rs1 = rs1;
      ins_after_decode.basic_rs2 = rs2;
    }
  }
  if (last_seven_ins == "0010011" & function_three_bit == "000" &
      (decode_ins != 19))  // addi
  {
    cout << "decode addi" << endl;
    ins_after_decode.is_alu = 1;
    ins_after_decode.is_add = 1;
    ins_after_decode.basic_use_rs1 = 1;
    ins_after_decode.basic_rs1 = rs1;
    ins_after_decode.add_sub_alu_2 = imm_i;
    ins_after_decode.basic_use_rd = 1;
    ins_after_decode.basic_rd = rd;
  }
  if (last_seven_ins == "0011011" & function_three_bit == "000")  // addiw
  {
    cout << "decode addiw" << endl;
    ins_after_decode.is_alu = 1;
    ins_after_decode.is_add = 1;
    ins_after_decode.basic_use_rs1 = 1;
    ins_after_decode.basic_rs1 = rs1;
    ins_after_decode.add_sub_alu_2 = imm_i;
    ins_after_decode.basic_use_rd = 1;
    ins_after_decode.basic_rd = rd;
    ins_after_decode.basic_is_half = 1;
  }

  // XOR OR AND
  if (last_seven_ins == "0110011" & function_three_bit == "100") {
    ins_after_decode.is_alu = 1;
    ins_after_decode.is_xor = 1;
    ins_after_decode.basic_use_rd = 1;
    ins_after_decode.basic_rd = rd;
    ins_after_decode.basic_use_rs1 = 1;
    ins_after_decode.basic_rs1 = rs1;
    ins_after_decode.basic_use_rs2 = 1;
    ins_after_decode.basic_rs2 = rs2;
  }
  if (last_seven_ins == "0110011" & function_three_bit == "110") {
    ins_after_decode.is_alu = 1;
    ins_after_decode.is_or = 1;
    ins_after_decode.basic_use_rd = 1;
    ins_after_decode.basic_rd = rd;
    ins_after_decode.basic_use_rs1 = 1;
    ins_after_decode.basic_rs1 = rs1;
    ins_after_decode.basic_use_rs2 = 1;
    ins_after_decode.basic_rs2 = rs2;
  }
  if (last_seven_ins == "0110011" & function_three_bit == "111") {
    ins_after_decode.is_alu = 1;
    ins_after_decode.is_and = 1;
    ins_after_decode.basic_use_rd = 1;
    ins_after_decode.basic_rd = rd;
    ins_after_decode.basic_use_rs1 = 1;
    ins_after_decode.basic_rs1 = rs1;
    ins_after_decode.basic_use_rs2 = 1;
    ins_after_decode.basic_rs2 = rs2;
  }
  if (last_seven_ins == "0010011" & function_three_bit == "100") {
    ins_after_decode.is_alu = 1;
    ins_after_decode.is_xor = 1;
    ins_after_decode.basic_use_rd = 1;
    ins_after_decode.basic_rd = rd;
    ins_after_decode.basic_use_rs1 = 1;
    ins_after_decode.basic_rs1 = rs1;
    ins_after_decode.op_imm = imm_i;
  }
  if (last_seven_ins == "0010011" & function_three_bit == "110") {
    ins_after_decode.is_alu = 1;
    ins_after_decode.is_or = 1;
    ins_after_decode.basic_use_rd = 1;
    ins_after_decode.basic_rd = rd;
    ins_after_decode.basic_use_rs1 = 1;
    ins_after_decode.basic_rs1 = rs1;
    ins_after_decode.op_imm = imm_i;
  }
  if (last_seven_ins == "0010011" & function_three_bit == "111") {
    ins_after_decode.is_alu = 1;
    ins_after_decode.is_and = 1;
    ins_after_decode.basic_use_rd = 1;
    ins_after_decode.basic_rd = rd;
    ins_after_decode.basic_use_rs1 = 1;
    ins_after_decode.basic_rs1 = rs1;
    ins_after_decode.op_imm = imm_i;
  }
  // SHIFT
  if (last_seven_ins == "0110011" & function_three_bit == "001" &
      ins[30] == '0') {
    ins_after_decode.is_alu = 1;
    ins_after_decode.is_shift = 1;
    ins_after_decode.shift_type = 0;
    ins_after_decode.basic_use_rd = 1;
    ins_after_decode.basic_rd = rd;
    ins_after_decode.basic_use_rs1 = 1;
    ins_after_decode.basic_rs1 = rs1;
    ins_after_decode.basic_use_rs2 = 1;
    ins_after_decode.basic_rs2 = rs2;
  }
  if (last_seven_ins == "0110011" & function_three_bit == "101" &
      ins[30] == '0') {
    ins_after_decode.is_alu = 1;
    ins_after_decode.is_shift = 1;
    ins_after_decode.shift_type = 1;
    ins_after_decode.basic_use_rd = 1;
    ins_after_decode.basic_rd = rd;
    ins_after_decode.basic_use_rs1 = 1;
    ins_after_decode.basic_rs1 = rs1;
    ins_after_decode.basic_use_rs2 = 1;
    ins_after_decode.basic_rs2 = rs2;
  }
  if (last_seven_ins == "0110011" & function_three_bit == "101" &
      ins[30] == '1') {
    ins_after_decode.is_alu = 1;
    ins_after_decode.is_shift = 1;
    ins_after_decode.shift_type = 2;
    ins_after_decode.basic_use_rd = 1;
    ins_after_decode.basic_rd = rd;
    ins_after_decode.basic_use_rs1 = 1;
    ins_after_decode.basic_rs1 = rs1;
    ins_after_decode.basic_use_rs2 = 1;
    ins_after_decode.basic_rs2 = rs2;
  }
  if (last_seven_ins == "0010011" & function_three_bit == "001" &
      ins[30] == '0') {
    ins_after_decode.is_alu = 1;
    ins_after_decode.is_shift = 1;
    ins_after_decode.shift_type = 0;
    ins_after_decode.basic_use_rd = 1;
    ins_after_decode.basic_rd = rd;
    ins_after_decode.basic_use_rs1 = 1;
    ins_after_decode.basic_rs1 = rs1;
    ins_after_decode.shift = rs2;
  }
  if (last_seven_ins == "0010011" & function_three_bit == "101" &
      ins[30] == '0') {
    ins_after_decode.is_alu = 1;
    ins_after_decode.is_shift = 1;
    ins_after_decode.shift_type = 1;
    ins_after_decode.basic_use_rd = 1;
    ins_after_decode.basic_rd = rd;
    ins_after_decode.basic_use_rs1 = 1;
    ins_after_decode.basic_rs1 = rs1;
    ins_after_decode.shift = rs2;
  }
  if (last_seven_ins == "0010011" & function_three_bit == "101" &
      ins[30] == '1') {
    ins_after_decode.is_alu = 1;
    ins_after_decode.is_shift = 1;
    ins_after_decode.shift_type = 2;
    ins_after_decode.basic_use_rd = 1;
    ins_after_decode.basic_rd = rd;
    ins_after_decode.basic_use_rs1 = 1;
    ins_after_decode.basic_rs1 = rs1;
    ins_after_decode.shift = rs2;
  }
  if (last_seven_ins == "0111011" & function_three_bit == "001" &
      ins[30] == '0') {
    ins_after_decode.is_alu = 1;
    ins_after_decode.is_shift = 1;
    ins_after_decode.shift_type = 0;
    ins_after_decode.basic_use_rd = 1;
    ins_after_decode.basic_rd = rd;
    ins_after_decode.basic_use_rs1 = 1;
    ins_after_decode.basic_rs1 = rs1;
    ins_after_decode.basic_use_rs2 = 1;
    ins_after_decode.basic_rs2 = rs2;
    ins_after_decode.basic_is_half = 1;
  }
  if (last_seven_ins == "0111011" & function_three_bit == "101" &
      ins[30] == '0') {
    ins_after_decode.is_alu = 1;
    ins_after_decode.is_shift = 1;
    ins_after_decode.shift_type = 1;
    ins_after_decode.basic_use_rd = 1;
    ins_after_decode.basic_rd = rd;
    ins_after_decode.basic_use_rs1 = 1;
    ins_after_decode.basic_rs1 = rs1;
    ins_after_decode.basic_use_rs2 = 1;
    ins_after_decode.basic_rs2 = rs2;
    ins_after_decode.basic_is_half = 1;
  }
  if (last_seven_ins == "0111011" & function_three_bit == "101" &
      ins[30] == '1') {
    ins_after_decode.is_alu = 1;
    ins_after_decode.is_shift = 1;
    ins_after_decode.shift_type = 2;
    ins_after_decode.basic_use_rd = 1;
    ins_after_decode.basic_rd = rd;
    ins_after_decode.basic_use_rs1 = 1;
    ins_after_decode.basic_rs1 = rs1;
    ins_after_decode.basic_use_rs2 = 1;
    ins_after_decode.basic_rs2 = rs2;
    ins_after_decode.basic_is_half = 1;
  }
  if (last_seven_ins == "0011011" & function_three_bit == "001" &
      ins[30] == '0') {
    ins_after_decode.is_alu = 1;
    ins_after_decode.is_shift = 1;
    ins_after_decode.shift_type = 0;
    ins_after_decode.basic_use_rd = 1;
    ins_after_decode.basic_rd = rd;
    ins_after_decode.basic_use_rs1 = 1;
    ins_after_decode.basic_rs1 = rs1;
    ins_after_decode.shift = rs2;
    ins_after_decode.basic_is_half = 1;
  }
  if (last_seven_ins == "0011011" & function_three_bit == "101" &
      ins[30] == '0') {
    ins_after_decode.is_alu = 1;
    ins_after_decode.is_shift = 1;
    ins_after_decode.shift_type = 1;
    ins_after_decode.basic_use_rd = 1;
    ins_after_decode.basic_rd = rd;
    ins_after_decode.basic_use_rs1 = 1;
    ins_after_decode.basic_rs1 = rs1;
    ins_after_decode.shift = rs2;
    ins_after_decode.basic_is_half = 1;
  }
  if (last_seven_ins == "0011011" & function_three_bit == "101" &
      ins[30] == '1') {
    ins_after_decode.is_alu = 1;
    ins_after_decode.is_shift = 1;
    ins_after_decode.shift_type = 2;
    ins_after_decode.basic_use_rd = 1;
    ins_after_decode.basic_rd = rd;
    ins_after_decode.basic_use_rs1 = 1;
    ins_after_decode.basic_rs1 = rs1;
    ins_after_decode.shift = rs2;
    ins_after_decode.basic_is_half = 1;
  }
  // SLT
  if (last_seven_ins == "0110011") {
    ins_after_decode.is_alu = 1;
    ins_after_decode.is_slt = 1;
    ins_after_decode.basic_use_rd = 1;
    ins_after_decode.basic_rd = rd;
    ins_after_decode.basic_use_rs1 = 1;
    ins_after_decode.basic_rs1 = rs1;
    ins_after_decode.basic_use_rs2 = 1;
    ins_after_decode.basic_rs2 = rs2;
  }
  if (last_seven_ins == "0010011" & !(decode_ins == 19)) {
    ins_after_decode.is_alu = 1;
    ins_after_decode.is_slt = 1;
    ins_after_decode.basic_use_rd = 1;
    ins_after_decode.basic_rd = rd;
    ins_after_decode.basic_use_rs1 = 1;
    ins_after_decode.basic_rs1 = rs1;
    ins_after_decode.slt_imm = imm_i;
  }
  return ins_after_decode;
}
// ALU

void Decode::reset() {
  decode_to_rcu_cycle = -10;
  decode_receive_ins_ready = 1;

}