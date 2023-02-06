#include "LSU.hpp"

#include <string>

#include "Memory.hpp"
#include "PRegFile.hpp"
#include "Utils.hpp"
#include "parameters.hpp"
using namespace std;

void LSU::LSQ_SEND_TO_DCACHE(PRegFile* PRF, Rename* rename_obj,
                             Memory* mem_obj) {
  dcache_req_ready = 0;
  dcache_current_cycle = cycle;
  if (lsq.basic_use_rs1) {
    lsq.basic_rs1_data =
        PRF->Read_PRF_data(rename_obj->rename_rd[lsq.basic_rs1]);
  }
  if (lsq.basic_use_rs2) {
    lsq.basic_rs2_data =
        PRF->Read_PRF_data(rename_obj->rename_rd[lsq.basic_rs2]);
  }
  addr = lsq.basic_rs1_data + lsq.lsu_imm;
  string load_data_s;

  if (lsq.lsu_load)  // 0 1 2 3 4 5 6 7
  {
    if (lsq.lsu_load_store_size == 1 & lsq.lsu_sign) {
      for (int i = 0; i < 7; i++) {
        string s(1, mem_obj->mem[addr][i]);
        load_data_s = load_data_s + s;
      }
      // load_data = (-1) * (mem_obj->mem[addr][0]-48) * pow(2,7) +
      // Utils::toIntLittleEndian(load_data_s,7);
      if (mem_obj->mem[addr][7] == '1') {
        load_data_s = Utils::reverseString(load_data_s, 7);
        load_data = -(Utils::toIntLittleEndian(load_data_s, 7) + 1);
      } else {
        load_data = Utils::toIntLittleEndian(load_data_s, 7);
      }
    }
    if (lsq.lsu_load_store_size == 2 & lsq.lsu_sign) {
      for (int j = 0; j < 1; j++) {
        for (int i = 0; i <= 7; i++) {
          string s(1, mem_obj->mem[addr + j][i]);
          load_data_s = load_data_s + s;
        }
      }
      for (int i = 0; i < 7; i++) {
        string s(1, mem_obj->mem[addr + 1][i]);
        load_data_s = load_data_s + s;
      }
      // load_data = (-1) * (mem_obj->mem[addr+1][0]-48) * pow(2,15) +
      // Utils::toIntLittleEndian(load_data_s,15);
      if (mem_obj->mem[addr + 1][7] == '1') {
        load_data_s = Utils::reverseString(load_data_s, 15);
        load_data = -(Utils::toIntLittleEndian(load_data_s, 15) + 1);
      } else {
        load_data = Utils::toIntLittleEndian(load_data_s, 15);
      }
    }
    if (lsq.lsu_load_store_size == 4 & lsq.lsu_sign) {
      for (int j = 0; j < 3; j++) {
        for (int i = 0; i <= 7; i++) {
          string s(1, mem_obj->mem[addr + j][i]);
          load_data_s = load_data_s + s;
        }
      }
      for (int i = 0; i < 7; i++) {
        string s(1, mem_obj->mem[addr + 3][i]);
        load_data_s = load_data_s + s;
      }
      // load_data = (-1) * (mem_obj->mem[addr+3][0]-48) * pow(2,31) +
      // Utils::toIntLittleEndian(load_data_s,31);
      if (mem_obj->mem[addr + 3][7] == '1') {
        load_data_s = Utils::reverseString(load_data_s, 31);
        load_data = -(Utils::toIntLittleEndian(load_data_s, 31) + 1);
      } else {
        load_data = Utils::toIntLittleEndian(load_data_s, 31);
      }
    }
    if (lsq.lsu_load_store_size == 8 & lsq.lsu_sign) {
      for (int j = 0; j < 7; j++) {
        for (int i = 0; i <= 7; i++) {
          string s(1, mem_obj->mem[addr + j][i]);
          load_data_s = load_data_s + s;
        }
      }
      for (int i = 0; i < 7; i++) {
        string s(1, mem_obj->mem[addr + 7][i]);
        load_data_s = load_data_s + s;
      }
      // load_data = (-1) * (mem_obj->mem[addr+7][0]-48) * pow(2,63) +
      // Utils::toIntLittleEndian(load_data_s,63);
      if (mem_obj->mem[addr + 7][7] == '1') {
        load_data_s = Utils::reverseString(load_data_s, 63);
        load_data = -(Utils::toIntLittleEndian(load_data_s, 63) + 1);
      } else {
        load_data = Utils::toIntLittleEndian(load_data_s, 63);
      }
    }
    if (lsq.lsu_load_store_size == 1 & !lsq.lsu_sign) {
      for (int i = 0; i <= 7; i++) {
        string s(1, mem_obj->mem[addr][i]);
        load_data_s = load_data_s + s;
      }
      load_data = Utils::toIntLittleEndian(load_data_s, 8);
    }
    if (lsq.lsu_load_store_size == 2 & !lsq.lsu_sign) {
      for (int j = 0; j <= 1; j++) {
        for (int i = 0; i <= 7; i++) {
          string s(1, mem_obj->mem[addr + j][i]);
          load_data_s = load_data_s + s;
        }
      }
      load_data = Utils::toIntLittleEndian(load_data_s, 16);
    }
    if (lsq.lsu_load_store_size == 4 & !lsq.lsu_sign) {
      for (int j = 0; j <= 3; j++) {
        for (int i = 0; i <= 7; i++) {
          string s(1, mem_obj->mem[addr + j][i]);
          load_data_s = load_data_s + s;
        }
      }
      load_data = Utils::toIntLittleEndian(load_data_s, 32);
    }
  } else {
    if (lsq.lsu_load_store_size == 1) {
      STORE_DATA_BYTE st_data = Utils::toBinary_b(lsq.basic_rs2_data);
      for (int i = 0; i < 8; i++) {
        mem_obj->mem[addr][i] = st_data[i];
      }
    }
    if (lsq.lsu_load_store_size == 2) {
      STORE_DATA_HALF st_data = Utils::toBinary_h(lsq.basic_rs2_data);
      for (int j = 0; j < 2; j++) {
        for (int i = 0; i < 8; i++) {
          mem_obj->mem[addr + j][i] = st_data[j * 8 + i];
        }
      }
    }

    if (lsq.lsu_load_store_size == 4) {
      STORE_DATA_WORD st_data = Utils::toBinary_w(lsq.basic_rs2_data);
      for (int j = 0; j < 4; j++) {
        for (int i = 0; i < 8; i++) {
          mem_obj->mem[addr + j][i] = st_data[j * 8 + i];
        }
      }
    }
    if (lsq.lsu_load_store_size == 8) {
      STORE_DATA_DWORD st_data = Utils::toBinary_d(lsq.basic_rs2_data);
      for (int j = 0; j < 8; j++) {
        for (int i = 0; i < 8; i++) {
          mem_obj->mem[addr + j][i] = st_data[j * 8 + i];
        }
      }
    }
  }
}

void LSU::reset() {
  dcache_req_ready = 1;
  dcache_current_cycle = -10;
}