#include <cstring>
#include <svdpi.h>
#include <stdint.h>
#include <vector>
#include <cstdlib>
#include <stdio.h>
#include <svdpi.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>

int preg [64] = {0};
FILE* cosim_log = NULL;
FILE* cosim_decode_log = NULL;
FILE* cosim_rcu_log = NULL;
FILE* cosim_btb_log = NULL;
FILE* cosim_icache_log = NULL;
long cycles = 0;
int ROB_SIZE = 4;
int gshare_hit = 0;
int gshare_not_hit = 0;
extern "C"{
    #define coprint(...) fprintf(cosim_log, __VA_ARGS__)
    #define codecodeprint(...) fprintf(cosim_decode_log, __VA_ARGS__)
    #define rcu_co_print(...) fprintf(cosim_rcu_log, __VA_ARGS__)
    #define btb_co_print(...) fprintf(cosim_btb_log, __VA_ARGS__)
    #define icache_co_print(...) fprintf(cosim_icache_log, __VA_ARGS__)

    extern void preg_sync(svLogic alu_valid, svLogic lsu_valid, long long alu_data_in, long long lsu_data_in, int alu_address, int lsu_address){
        if(alu_valid & (alu_address != 0)){
            preg[alu_address] = alu_data_in;
        }
        if(lsu_valid & (lsu_address != 0)){
            preg[lsu_address] = lsu_data_in;
        }
        preg[0] = 0;
    }

    extern void get_log_handler(){
        cosim_log = fopen("cosim.log", "w+");
        cosim_decode_log = fopen("cosim-decode.log", "w+");
        cosim_rcu_log = fopen("cosim-rcu.log", "w+");
        cosim_btb_log = fopen("cosim-btb.log", "w+");
        cosim_icache_log = fopen("cosim-icache.log", "w+");
    }
    extern void close_log(){
        fclose(cosim_log);
        fclose(cosim_decode_log);
        fclose(cosim_rcu_log);
        fclose(cosim_btb_log);
        fclose(cosim_icache_log);
    }




    bool csr_monitor_read = false;  
    bool csr_need_print = false; 
    char const *csr_name;
    long long csr_value;



    extern void log_decode_print(int pc,int ins, svLogic valid_in, svLogic alu_a_0, svLogic alu_a_1, svLogic alu_b_0 ,svLogic alu_b_1, svLogic half, svLogic branch_out_w, svLogic jump_out_w,svLogic lui,svLogic slli)
    {
        if(valid_in)
        {
            codecodeprint("core 0: 0x00000000%08X     ", pc);
            if(alu_a_0 == 0 & alu_a_1 == 0 & alu_b_0 == 1 & alu_b_1 == 0 & half == 1 & branch_out_w == 0 & jump_out_w == 0)
            {
                codecodeprint("addiw\n");
            }else if(alu_a_0 == 0 & alu_a_1 == 0 & alu_b_0 == 0 & alu_b_1 == 0 & half == 0 & branch_out_w == 0 & jump_out_w == 0)
            {
                codecodeprint("add\n");
            }else if(alu_a_0 == 0 & alu_a_1 == 0 & alu_b_0 == 1 & alu_b_1 == 0 & half == 0 & branch_out_w == 0 & jump_out_w == 0)
            {
                codecodeprint("li\n");
            }else if(alu_a_0 == 0 & alu_a_1 == 1 & alu_b_0 == 1 & alu_b_1 == 0 & half == 0 & branch_out_w == 1 & jump_out_w == 0)
            {
                codecodeprint("bne\n");
            }else if(lui ==1)
            {
                codecodeprint("lui\n");
            }else if(slli ==1)
            {
                codecodeprint("slli\n");
            }else {
                codecodeprint("\n");
            }
        }
    }


    extern void log_print(svLogic co_commit, int co_pc_in, svLogic co_store_in, svLogic co_fence, svLogic co_mret, svLogic co_wfi,  svLogic co_uses_csr, int co_rob_rd, svLogic co_csr_iss_ctrl, int co_prf_name, int co_csr_address){
        if(co_commit){
            coprint("core 0: 0x00000000%08X     ", co_pc_in);
            if(co_uses_csr){  //Zicsr
                if(csr_need_print){
                    coprint("CSR %s <- 0x%016lX\n", csr_name, csr_value);
                    csr_need_print = false;
                } else{
                    coprint("\n");
                }
                if(co_rob_rd && csr_monitor_read){
                    coprint("x%d <- 0x%016lX\n", co_rob_rd, preg[co_prf_name]);
                }
                csr_monitor_read = false;
            }else {
                if(co_rob_rd){
                    coprint("x%d <- 0x%016lX\n", co_rob_rd, preg[co_prf_name]);
                } else {
                    coprint("\n");
                }
            }
        }

    }

    extern void rob_iss_print(svLogic co_issue, svLogic co_commit, int co_iss_pc, int co_commit_pc, int co_cm_rob_line, int co_iss_rob_line, int co_wr_rob_line){
        cycles++;
        if(co_issue) {
            rcu_co_print("Issue\tcore 0: 0x00000000%08X     \n", co_iss_pc);
            rcu_co_print("\tcycles:          %ld    \n", cycles);
            rcu_co_print("\trob_size:        %d     \n", (co_wr_rob_line - co_cm_rob_line + ROB_SIZE) % ROB_SIZE);
            // rcu_co_print("\twrite_rob_line:  %d     \n", co_wr_rob_line);
            // rcu_co_print("\tissue_rob_line:  %d     \n", co_iss_rob_line);
            // rcu_co_print("\tcommit_rob_line: %d     \n", co_cm_rob_line);
        }else if(co_commit) {
            rcu_co_print("Commit\tcore 0: 0x00000000%08X     \n", co_commit_pc);
            rcu_co_print("\tcycles:          %ld    \n", cycles);
            rcu_co_print("\trob_size:        %d     \n", (co_wr_rob_line - co_cm_rob_line + ROB_SIZE) % ROB_SIZE);
        }
    }

    extern void btb_update_print(svLogic prev_taken, int PHT0, int PHT1, int PHT2, int PHT3, int PHT4, int PHT5, int PHT6, int PHT7, int PHT8, int PHT9, int PHT10, int PHT11, int PHT12, int PHT13, int PHT14, int PHT15, int GHR) {
        if(prev_taken) {
            btb_co_print("GShare Hit\n");
            gshare_hit++;
        }else {
            gshare_not_hit++;
        }
        btb_co_print("-------- GShare dump --------\n");
        btb_co_print("\tGHR: %d\n", GHR);
        btb_co_print("%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", PHT0, PHT1, PHT2, PHT3, PHT4, PHT5, PHT6, PHT7, PHT8, PHT9, PHT10, PHT11, PHT12, PHT13, PHT14, PHT15);
        btb_co_print("\tHit: %d, Not Hit: %d\n", gshare_hit, gshare_not_hit);
        btb_co_print("------ GShare dump Over------\n");
    }

    extern void icache_print(int numRead, int numWrite, int numHit, int numMiss) {
        icache_co_print("-------- CACHE STATISTICS ----------\n");
        icache_co_print("Num Read: %d\n", numRead);
        icache_co_print("Num Write: %d\n", numWrite);
        icache_co_print("Num Hit: %d\n", numHit);
        icache_co_print("Num Miss: %d\n", numMiss);
        icache_co_print("Total Cycles: %d\n", cycles);
        icache_co_print("------ CACHE STATISTICS DONE--------\n");
    }

}


