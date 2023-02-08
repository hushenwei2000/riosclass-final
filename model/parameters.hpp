#pragma once
#include <string>
#include <array>

// PARAM
#define Maxcycle 2200
#define BTB_SIZE 512
#define MEM (2 << 20)
#define RESET_VECTOR 0x000005c
#define INS_BUFFER_SIZE 9
#define REG_SIZE 40
#define ICACHE_DELAY 2
#define DCACHE_DELAY 3
#define ICACHE_INS_BUFFER_DELAY 1
#define INS_BUFFER_DECODE_DELAY 1
#define ALU_DELAY 1
#define DECODE_RCU_DELAY 1
#define CSR_DELAY 1
#define LSQ_DEPTH 3
#define ROB_DEPTH 5
#define FREE_LIST 39
#define GSHARE_WIDTH 4
#define PHT_LEN 16

// typedef
// Each array index 0 is the lowest bit of number
typedef std::array<char, 8> STORE_DATA_BYTE;
typedef std::array<char, 16> STORE_DATA_HALF;
typedef std::array<char, 32> STORE_DATA_WORD;
typedef std::array<char, 64> STORE_DATA_DWORD;
typedef std::array<char, 32> INSTRUCTION;

extern int pc;

// struct
struct CHECK_BTB_S
{
    int btb_pc;
    bool btb_hit;
};
struct ROB_LINE_S
{
    int basic_pc = 0;
    int basic_pc_next = 0;
    bool basic_use_rs1 = 0;
    bool basic_use_rs2 = 0;
    bool basic_use_rd = 0;
    int basic_rs1 = 0;
    int basic_rs2 = 0;
    int basic_rd = 0;
    int basic_rename_rd = 0;
    int basic_last_rename_rd = 0;
    long long basic_rs1_data = 0;
    long long basic_rs2_data = 0;
    bool basic_is_half = 0;
    long long basic_ins = 0;
    bool is_lsu = 0;
    bool lsu_load = 0;
    bool lsu_store = 0;
    int  lsu_load_store_size = 0;
    bool lsu_sign = 0;
    int  lsu_imm = 0;

    bool is_alu = 0;
    bool is_add = 0;
    bool is_sub = 0;
    long long add_sub_alu_1 = 0;
    long long add_sub_alu_2 = 0;

    bool is_fence = 0;

    bool is_jump = 0;
    bool j_b_success = 0;

    bool is_lui = 0;
    long long lui_imm = 0;

    bool is_branch = 0;
    int branch_imm = 0;
    std::string branch_func = "";
    
    bool is_xor = 0;
    bool is_or = 0;
    bool is_and = 0;
    int op_imm = 0;

    bool is_shift = 0;
    int shift_type = 0;//0sll 1srl 2sra
    int shift = 0;

    bool is_slt = 0;
    int slt_imm = 0;
    int slt_result = 0;

    long long alu_result = 0;

    int rob_index = 0;
    bool finish = 0;

    bool is_mret = 0;
    bool is_ecall = 0;
    bool is_nop = 0;
    bool is_csr = 0;
    bool csr_no_use = 0;

};

// Global
extern long long cycle;
