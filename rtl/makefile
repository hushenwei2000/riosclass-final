# SIM_ROOT := /work/stu/swhu/projects/riosclass_template2/verilog/dv
# RV_ROOT := /work/stu/swhu/projects/riosclass_template2/verilog/rtl/lab_1/src

# FILE_LIST := /work/stu/swhu/projects/riosclass_template2/co-sim/flist.f
# TOP_MODULE := hehe_tb
# TOP_MODULE_COSIM := hehe_tb_cosim

# HEX_DIR := /work/stu/swhu/projects/riosclass_template2/verilog/dv/isa/build/hex
# HEX_FILE := /work/stu/swhu/projects/riosclass_template2/verilog/dv/isa/build/hex/rv64ui/add.hex

RV_ROOT := $(shell pwd)

FILE_LIST := $(RV_ROOT)/flist.f
TOP_MODULE := hehe_tb
TOP_MODULE_COSIM := hehe_tb_cosim

HEX_DIR := $(RV_ROOT)/../test/hex/rv64ui
HEX_FILE := "\""$(HEX_DIR)/add.hex"\""

# "  \"  "

DEFINE:=SYNTHESIS+COMMIT_LOG

hehe: $(HEX_FILE)
	$(if $(shell ls -a simv.daidir | grep timestamp), \
  rm simv.daidir/.vcs.timestamp*)
	vcs \
  	+vcs+lic+wait \
	-kdb					\
	-sverilog	\
	+vc+list  \
	+vpi			\
	+vcs+flush+all \
	+define+$(DEFINE)+HEX_FILE=$(HEX_FILE) \
	-o simv \
	+lint=TFIPC-L -full64 -F ${FILE_LIST} \
	-top $(TOP_MODULE) -timescale=1ns/1ps \
	-v2k_generate -debug_access+all \
	+incdir+$(RV_ROOT) \
	+error+10 +lint=PCWM-L -q

	./simv 2 >&1 

cosim: $(HEX_FILE)
	$(if $(shell ls -a simv.daidir | grep timestamp), \
  rm simv.daidir/.vcs.timestamp*)
	vcs \
  	+vcs+lic+wait \
	-kdb					\
	-sverilog	\
	+vc+list  \
	+vpi			\
	+vcs+flush+all \
	+define+$(DEFINE)+COSIM+HEX_FILE=$(HEX_FILE) \
	-o simv_cosim \
	+lint=TFIPC-L -full64 -F ${FILE_LIST} \
	-top $(TOP_MODULE_COSIM) -timescale=1ns/1ps \
	-v2k_generate -debug_access+all \
	+incdir+$(RV_ROOT) \
	+error+10 +lint=PCWM-L -q

	./simv_cosim 2 >&1 


wave:
	./simv +dump
	Verdi-SX -ssf waves.fsdb

wave-cosim:
	./simv_cosim +dump
	Verdi-SX -ssf waves_cosim.fsdb

$(HEX_FILE):


