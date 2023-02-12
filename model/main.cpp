#include <iostream>
#include <fstream>

#include "parameters.hpp"
#include "ALU.hpp"
#include "BTB.hpp"
#include "CSR.hpp"
#include "Decode.hpp"
#include "Fetch.hpp"
#include "InsBuffer.hpp"
#include "Issue.hpp"
#include "LSU.hpp"
#include "Memory.hpp"
#include "ICache.hpp"
#include "PRegFile.hpp"
#include "Rename.hpp"
#include "ROB.hpp"
#include "Utils.hpp"

using namespace std;
char * hexfile;
ofstream cache_performance_file("cache_performance.csv", ofstream::app);

ALU* alu_obj;
BTB* btb_obj;
CSR* csr_obj;
Decode* decode_obj;
Fetch* fetch_obj;
InsBuffer* insBuffer_obj;
Issue* issue_obj;
LSU* lsu_obj;
Memory* memory_obj;
ICache* icache_obj;
PRegFile* pRegFile_obj;
Rename* rename_obj;
ROB* rob_obj;
int pc;
long long cycle = 0;
FILE* Issue_Log_File = NULL;
FILE* Cache_Log_File = NULL;
FILE* Commit_Log_File = NULL;
FILE* GShare_Log_File = NULL;

void FLUSH() {
  cout << " FLUSH ";
  Utils::dumpPC(rob_obj->rob[lsu_obj->lsq.rob_index].basic_pc);
  icache_obj->icache_current_cycle = -10;
  icache_obj->icache_req_ready = 1;
  icache_obj->icache_ins_buffer_current_cycle = -10;
  insBuffer_obj->ins_buffer_head = insBuffer_obj->ins_buffer_tail = 0;
  decode_obj->decode_receive_ins_ready = 1;
  insBuffer_obj->CHECK_INS_BUFFER();
}
void RESET() {
  alu_obj->reset();
  decode_obj->reset();
  insBuffer_obj->reset();
  memory_obj->reset();
  icache_obj->reset();
  csr_obj->reset();
  pRegFile_obj->reset();
  btb_obj->reset();
  lsu_obj->reset();
  rename_obj->reset();

  pc = RESET_VECTOR;
}

void READ_ELF( char * elfPath) {
  cout << "READ_ELF" << endl;
  ifstream hexfile;
  int ins_line = 0;
  char* buffer = new char[999999];
  hexfile.open(elfPath, ios::in);  // 0-46 47 48 49-95 96 97
  hexfile.seekg(0, std::ios::end);
  int fileLength = hexfile.tellg();
  hexfile.seekg(0, std::ios::beg);
  hexfile.read(buffer, fileLength);
  string a, b;
  long long base = RESET_VECTOR;
  int line_i;
  cout << fileLength << endl;
  for (int i = 0; i < fileLength; i++) {
    if (i % 49 == 0) {
      line_i = 0;
    }
    if (line_i != 48) {
      if ((line_i % 3) == 0) {
        switch (buffer[i]) {
          case '0':
            a = "0000";
            break;
          case '1':
            a = "0001";
            break;
          case '2':
            a = "0010";
            break;
          case '3':
            a = "0011";
            break;
          case '4':
            a = "0100";
            break;
          case '5':
            a = "0101";
            break;
          case '6':
            a = "0110";
            break;
          case '7':
            a = "0111";
            break;
          case '8':
            a = "1000";
            break;
          case '9':
            a = "1001";
            break;
          case 'A':
            a = "1010";
            break;
          case 'B':
            a = "1011";
            break;
          case 'C':
            a = "1100";
            break;
          case 'D':
            a = "1101";
            break;
          case 'E':
            a = "1110";
            break;
          case 'F':
            a = "1111";
            break;
          default:
            cout << "wrong: " << buffer[i] << " ";
            break;
        }
      }
      if ((line_i % 3) == 1) {
        switch (buffer[i]) {
          case '0':
            b = "0000";
            break;
          case '1':
            b = "0001";
            break;
          case '2':
            b = "0010";
            break;
          case '3':
            b = "0011";
            break;
          case '4':
            b = "0100";
            break;
          case '5':
            b = "0101";
            break;
          case '6':
            b = "0110";
            break;
          case '7':
            b = "0111";
            break;
          case '8':
            b = "1000";
            break;
          case '9':
            b = "1001";
            break;
          case 'A':
            b = "1010";
            break;
          case 'B':
            b = "1011";
            break;
          case 'C':
            b = "1100";
            break;
          case 'D':
            b = "1101";
            break;
          case 'E':
            b = "1110";
            break;
          case 'F':
            b = "1111";
            break;
          default:
            cout << "wrong" << buffer[i] << " ";
            break;
        }
        memory_obj->mem[ins_line][0] = a[0];
        memory_obj->mem[ins_line][1] = a[1];
        memory_obj->mem[ins_line][2] = a[2];
        memory_obj->mem[ins_line][3] = a[3];
        memory_obj->mem[ins_line][4] = b[0];
        memory_obj->mem[ins_line][5] = b[1];
        memory_obj->mem[ins_line][6] = b[2];
        memory_obj->mem[ins_line][7] = b[3];
        ins_line += 1;
      }
      line_i = line_i + 1;
    }
  }
}

void print_iss_log() {
  fprintf(Issue_Log_File, "Issue\tcore 0: 0x00000000%08X     \n", rob_obj->rob[(rob_obj->rob_head_iss) % ROB_DEPTH].basic_pc);
  fprintf(Issue_Log_File, "\tcycles:          %d     \n", cycle);
  fprintf(Issue_Log_File, "\trob_size:        %d     \n", (rob_obj->rob_tail - rob_obj->rob_head_commit + ROB_DEPTH) % ROB_DEPTH);
  // fprintf(Issue_Log_File, "\twrite_rob_line:  %d     \n", rob_obj->rob_tail);
  // fprintf(Issue_Log_File, "\tissue_rob_line:  %d     \n", rob_obj->rob_head_iss);
  // fprintf(Issue_Log_File, "\tcommit_rob_line: %d     \n", rob_obj->rob_head_commit);
  fprintf(Issue_Log_File, "\trob list (unfinish):\n");
  for(int i = (rob_obj->rob_tail + ROB_DEPTH) % ROB_DEPTH; i > (rob_obj->rob_head_commit + ROB_DEPTH) % ROB_DEPTH; i--) {
    fprintf(Issue_Log_File, "\t\t%d: 0x00000000%08X %s \n", i, rob_obj->rob[i].basic_pc, rob_obj->getType(i).c_str());
  }
}

void print_rcu_commit_log() {
  fprintf(Issue_Log_File, "Commit\tcore 0: 0x00000000%08X     \n", rob_obj->rob[(rob_obj->rob_head_commit + 1) % ROB_DEPTH].basic_pc);
  fprintf(Issue_Log_File, "\tcycles:          %d     \n", cycle);
  fprintf(Issue_Log_File, "\trob_size:        %d     \n", ((rob_obj->rob_tail - rob_obj->rob_head_commit - 1) + ROB_DEPTH) % ROB_DEPTH);
  // fprintf(Issue_Log_File, "\twrite_rob_line:  %d     \n", rob_obj->rob_tail);
  // fprintf(Issue_Log_File, "\tissue_rob_line:  %d     \n", rob_obj->rob_head_iss);
  // fprintf(Issue_Log_File, "\tcommit_rob_line: %d     \n", rob_obj->rob_head_commit);
  fprintf(Issue_Log_File, "\trob list (unfinish):\n");
  for(int i = (rob_obj->rob_tail + ROB_DEPTH) % ROB_DEPTH; i > (rob_obj->rob_head_commit + 1 + ROB_DEPTH) % ROB_DEPTH; i--) {
    fprintf(Issue_Log_File, "\t\t%d: 0x00000000%08X %s \n", i, rob_obj->rob[i].basic_pc, rob_obj->getType(i).c_str());
  }
}

void print_commit_log() {
  fprintf(Commit_Log_File, "Alu_Update_Prf: core 0: 0x00000000%08X     x%d <- %016lX\n", rob_obj->rob[alu_obj->alu.rob_index].basic_pc, alu_obj->alu.basic_rename_rd, alu_obj->alu.alu_result);
}

int main(int argc, char * argv[]) {
  if (argc != 2){
    printf("Please input hex file name as parameter");
    return 1;
  }
  
  char * elfPath = argv[1];
  hexfile = elfPath;
  cycle = 0;
  Issue_Log_File = fopen("cosim-issue.log", "w+");
  Cache_Log_File = fopen("cosim-cache.log", "w+");
  GShare_Log_File = fopen("cosim-btb.log", "w+");
  Commit_Log_File = fopen("cosim-commit.log", "w+");
  alu_obj = new ALU();
  btb_obj = new BTB(GShare_Log_File);
  csr_obj = new CSR();
  decode_obj = new Decode();
  fetch_obj = new Fetch();
  insBuffer_obj = new InsBuffer();
  issue_obj = new Issue();
  lsu_obj = new LSU();
  memory_obj = new Memory();
  Cache::Policy l1Policy;
  l1Policy.cacheSize = 32;
  l1Policy.blockSize = 4;
  l1Policy.blockNum = l1Policy.cacheSize / l1Policy.blockSize;
  l1Policy.associativity = 1;
  l1Policy.hitLatency = 2;
  l1Policy.missLatency = 8;
  icache_obj = new ICache(memory_obj, l1Policy, Cache_Log_File);
  pRegFile_obj = new PRegFile();
  rename_obj = new Rename();
  rob_obj = new ROB();
  READ_ELF(elfPath);
  RESET();
  for (int i = 0; i < Maxcycle; i++) {
    cout << "cycle ========================" << cycle << endl;
    // cout << "prf" << endl;
    // for (int i = 0; i < REG_SIZE; i++) {
    //   cout << "x" << i << ": " << pRegFile_obj->registers[i] << ", " << pRegFile_obj->registers_p[i] << endl;
    // }
    // cout << "rename" << endl;
    // for (int i = 0; i < 33; i++) {
    //   cout << i << ": " << rename_obj->rename_rd[i] << endl;
    // }
    if (rob_obj->rob[(rob_obj->rob_head_commit + 1) % ROB_DEPTH].finish == 1) {
      print_rcu_commit_log();
      cout << "commit pc ";
      Utils::dumpPC(rob_obj->rob[(rob_obj->rob_head_commit + 1) % ROB_DEPTH].basic_pc);
      rob_obj->COMMIT();
      cout << "after commit" << endl;
      for (int j = 0; j < ROB_DEPTH; j++) {
        cout << j << " " << rob_obj->rob[j].finish << endl;
      }
    }

    if (cycle == icache_obj->icache_current_cycle + icache_latency ) {
      cout << "icache_latency" << icache_latency << endl;
      cout << "fetch ins from icache " << icache_obj->ins_from_icache << endl;
      icache_obj->icache_to_ins_buffer = Utils::toIntBigEndian(icache_obj->ins_from_icache, 32);
      icache_obj->icache_ins_buffer_current_cycle = cycle;
      icache_obj->icache_to_ins_buffer_pc = icache_obj->ins_pc;
      if (icache_obj->ins_pc == 64) {
        cout << "pass" << endl;
        long long numHit, numMiss;
        numHit = icache_obj->statistics.numHit;
        numMiss = icache_obj->statistics.numMiss;
        printf("numHit: %d, numMIss:%d\n", numHit, numMiss );
        double ipc =1.0 *  (icache_obj->statistics.numHit + icache_obj->statistics.numMiss) / cycle;
        cache_performance_file << hexfile << "," << ipc << std::endl;
        cache_performance_file.close();
        return 0;
      }
    }

    if (lsu_obj->dcache_current_cycle + DCACHE_DELAY == cycle) {
      cout << "lsu done" << endl;
      lsu_obj->dcache_req_ready = 1;
      rob_obj->rob[lsu_obj->lsq.rob_index].finish = 1;
      if (lsu_obj->lsq.basic_use_rd) {
        pRegFile_obj->Lsu_Update_Prf(lsu_obj->lsq.basic_rename_rd, lsu_obj->load_data);
      }
    }
    if (csr_obj->csr_current_cycle + CSR_DELAY == cycle) {
      cout << "csr done" << endl;
      csr_obj->csr_req_ready = 1;
      rob_obj->rob[csr_obj->csr.rob_index].finish = 1;
      if (csr_obj->csr.basic_use_rd) {
        rename_obj->rename_rd[csr_obj->csr.basic_rd] = csr_obj->csr.basic_rename_rd;
      }
      if (csr_obj->csr.is_ecall | csr_obj->csr.is_mret) {
        pc = csr_obj->csr_pc;
        FLUSH();
        decode_obj->decode_receive_ins_ready = 1;
      }
    }
    if (alu_obj->alu_current_cycle + ALU_DELAY == cycle) {
      cout << "alu done" << endl;
      alu_obj->alu_ready = 1;
      rob_obj->rob[alu_obj->alu.rob_index].finish = 1;
      if (alu_obj->alu.basic_use_rd) {
        if (alu_obj->alu.basic_rd != 0) {
          print_commit_log();
          pRegFile_obj->Alu_Update_Prf(alu_obj->alu.basic_rename_rd, alu_obj->alu.alu_result);
          rename_obj->rename_rd[alu_obj->alu.basic_rd] = alu_obj->alu.basic_rename_rd;
        }
        if (alu_obj->alu.basic_last_rename_rd != 0) {
          cout << "release free list" << endl;
          pRegFile_obj->registers_p[alu_obj->alu.basic_last_rename_rd] = 0;
          rename_obj->free_list[rename_obj->free_list_tail] = alu_obj->alu.basic_last_rename_rd;
          rename_obj->free_list_tail = (rename_obj->free_list_tail + 1) % FREE_LIST;
        }
      }
      if (alu_obj->alu.is_branch | alu_obj->alu.is_jump) {
        if (alu_obj->alu.basic_pc + 4 == alu_obj->alu.basic_pc_next) {
          // not taken
            btb_obj->gshare.prev_taken = false;
        }else {
            btb_obj->gshare.prev_taken = true;
        }
        if (insBuffer_obj->ins_buffer_pc[(insBuffer_obj->ins_buffer_head + 1) % INS_BUFFER_SIZE] ==
            alu_obj->alu.basic_pc_next) {
          btb_obj->UPDATE_BTB_TAKEN(alu_obj->alu.basic_pc);
          cout << "预测成功" << endl;
          decode_obj->decode_receive_ins_ready = 1;
        } else { // 预测失败 predict wrong
          FLUSH();
          decode_obj->decode_receive_ins_ready = 1;
          pc = alu_obj->alu.basic_pc_next;
          btb_obj->UPDATE_BTB(alu_obj->alu.basic_pc, pc);
        }
      }
    }

    if (!insBuffer_obj->ins_buffer_full & icache_obj->icache_req_ready) {
      cout << "fetch ins pc ";
      Utils::dumpPC(pc);
      icache_obj->Fetch_INS_FROM_ICACHE(pc);
      btb_obj->check_btb = btb_obj->CHECK_BTB(pc);
      if (btb_obj->check_btb.btb_hit == 1) {
        cout << "btb hit" << endl;
        pc = btb_obj->check_btb.btb_pc;
      } else {
        cout << "btb not hit" << endl;
        pc = pc + 4;
      }
    }
    if (cycle == icache_obj->icache_ins_buffer_current_cycle + ICACHE_INS_BUFFER_DELAY) {
      cout << "write ins_buffer pc ";
      Utils::dumpPC(icache_obj->icache_to_ins_buffer_pc);
      icache_obj->icache_req_ready = 1;
      insBuffer_obj->WRITE_INS_BUFFER(icache_obj->icache_to_ins_buffer_pc, icache_obj->icache_to_ins_buffer);
      insBuffer_obj->CHECK_INS_BUFFER();
    }
    if (decode_obj->decode_receive_ins_ready & !insBuffer_obj->ins_buffer_empty) {
      decode_obj->decode_to_rcu_cycle = cycle;
      decode_obj->decode_receive_ins_ready = 0;
    }
    if (cycle == decode_obj->decode_to_rcu_cycle + DECODE_RCU_DELAY) {
      decode_obj->decode_receive_ins_ready = 1;
      insBuffer_obj->READ_INS_BUFFER();
      insBuffer_obj->CHECK_INS_BUFFER();
      ROB_LINE_S ins_after_decode =
          decode_obj->evaluate(insBuffer_obj->ins_buffer_to_decode_pc, insBuffer_obj->ins_buffer_to_decode_ins);
      rename_obj->evaluate(ins_after_decode);
      rob_obj->IN_ROB(rename_obj->ins_after_rename);
      if (rob_obj->rob_full) {
        decode_obj->decode_receive_ins_ready = 0;
      }
      if (ins_after_decode.is_jump | ins_after_decode.is_branch |
          ins_after_decode.is_mret | ins_after_decode.is_ecall) {
        decode_obj->decode_receive_ins_ready = 0;
      }
    }
    
    if (!rob_obj->rob_empty) {
      issue_obj->ISS_CHECK(rob_obj, rename_obj, pRegFile_obj, lsu_obj, alu_obj, csr_obj);
      if (issue_obj->iss_lsu) {
        cout << "iss_lsu" << endl;
        lsu_obj->lsq = issue_obj->iss_rob_line;
        lsu_obj->LSQ_SEND_TO_DCACHE(pRegFile_obj, rename_obj, memory_obj);
        issue_obj->evaluate(rob_obj);
        print_iss_log();
      }
      if (issue_obj->iss_alu) {
        cout << "iss_alu" << endl;
        alu_obj->alu = issue_obj->iss_rob_line;
        alu_obj->evaluate(pRegFile_obj, rename_obj);
        issue_obj->evaluate(rob_obj);
        print_iss_log();
      }
      if (issue_obj->commit_directly) {
        cout << "iss directly" << endl;
        issue_obj->evaluate(rob_obj);
        print_iss_log();
      }
      if (issue_obj->iss_csr) {
        cout << "iss csr" << endl;
        csr_obj->csr = issue_obj->iss_rob_line;
        csr_obj->evaluate(pRegFile_obj, rename_obj);
        issue_obj->evaluate(rob_obj);
        print_iss_log();
      }
    }

    cycle++;
  }
  long long numHit, numMiss;
  numHit = icache_obj->statistics.numHit;
  numMiss = icache_obj->statistics.numMiss;
  double ipc =1.0 *  (icache_obj->statistics.numHit + icache_obj->statistics.numMiss) / cycle;
  cache_performance_file << hexfile << "," << ipc << std::endl;
  cache_performance_file.close();
  fclose(Issue_Log_File);
  return 0;
}