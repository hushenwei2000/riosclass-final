SIM_ROOT := $(shell pwd)
RV_ROOT := $(SIM_ROOT)/../rtl/lab_1/src

FILE_LIST := $(RV_ROOT)/../flist.f
FILE_LIST_BKP := $(RV_ROOT)/../flist_bkp.f
TOP_MODULE := hehe_tb

ELF_DIR := $(SIM_ROOT)/../rtl/elf
HEX_DIR := $(SIM_ROOT)/../rtl/elf2hex
HEX_FILE := "\""$(HEX_DIR)/rv64ui-p-lw.hex"\""
# "  \"  "

DEFINE:=SYNTHESIS+COMMIT_LOG

hehe: $(HEX_FILE)
	$(if $(shell ls simv.daidir | grep timestamp), \
  rm simv.daidir/.vcs.timestamp)
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
orig : $(HEX_FILE)
	$(if $(shell ls simv.daidir | grep timestamp), \
  rm simv.daidir/.vcs.timestamp)
	vcs \
  +vcs+lic+wait \
	-kdb					\
	-sverilog	\
	+vc+list  \
	+vpi			\
	+vcs+flush+all \
	+define+$(DEFINE)+HEX_FILE=$(HEX_FILE) \
	-o simv \
	+lint=TFIPC-L -full64 -F ${FILE_LIST_BKP} \
	-top $(TOP_MODULE) -timescale=1ns/1ps \
	-v2k_generate -debug_access+all \
	+incdir+$(RV_ROOT) \
	+error+10 +lint=PCWM-L -q

	./simv 2 >&1 



wave:
	Verdi-SX -ssf waves.fsdb

$(HEX_FILE):


