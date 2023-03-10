module gshare 
#(
  parameter GSHARE_WIDTH = 4,
  parameter PHT_LEN = 16
)
(
  input clk,
  input reset,

  // from fetch: current instr
  /* verilator lint_off UNUSED */
  input [31:0] pc,

  // from execute: last instr
  input [31:0] prev_pc,  // prev pc
  input prev_branch_in, // whether prev instr is branch
  input prev_taken, // whether prev instr taken
  // input prev_pred, // prev instr pred result
  // output prev_mispred, // whether prev instr mispred

  // to out: taken or not taken
  output cur_pred// cur instr pred
);

integer i =0;

// pattern history table 
reg [1:0] PHT [0:PHT_LEN-1];

// global history register
reg [GSHARE_WIDTH-1:0] GHR = 0;

// reg [GSHARE_WIDTH-1:0] prev_idx = 0;

// previous instr's index into PHT; aliasing issue exists
wire [GSHARE_WIDTH-1:0] prev_idx = prev_pc[GSHARE_WIDTH+1:2] ^ GHR;

wire [GSHARE_WIDTH-1:0] cur_idx = pc[GSHARE_WIDTH+1:2] ^ GHR;

// cur pred result
assign cur_pred = PHT[cur_idx][1];

// whether last instr is mispred
// assign prev_mispred = (prev_taken != prev_pred);

always @(posedge clk) begin
  if (reset) begin
    // prev_idx <= 0;
    GHR <= 0;
    for (i = 0; i < PHT_LEN; i = i + 1) begin
      PHT[i] <= 2'b10; // initial: weak taken
    end
  end

  // update GHR using last pc from execute
  if (prev_branch_in) begin
    GHR <= {GHR[GSHARE_WIDTH-2:0], prev_taken};
  end

  // update PHT using previous taken or not taken result
  if (prev_branch_in && prev_taken) begin
    case (PHT[prev_idx])
      2'b00:
        PHT[prev_idx] <= 2'b01;
      2'b01:
        PHT[prev_idx] <= 2'b10;
      2'b10:
        PHT[prev_idx] <= 2'b11;
      2'b11:
        PHT[prev_idx] <= 2'b11;
    endcase
  end else if (prev_branch_in && !prev_taken) begin
    case (PHT[prev_idx])
      2'b00:
        PHT[prev_idx] <= 2'b00;
      2'b01:
        PHT[prev_idx] <= 2'b00;
      2'b10:
        PHT[prev_idx] <= 2'b01;
      2'b11:
        PHT[prev_idx] <= 2'b10;
    endcase
  end

  /* verilator lint_on UNUSED */
end

`ifdef COSIM
import "DPI-C" function void btb_update_print(input prev_taken, input int PHT0, input int PHT1, input int PHT2, input int PHT3, input int PHT4, input int PHT5, input int PHT6, input int PHT7, input int PHT8, input int PHT9, input int PHT10, input int PHT11, input int PHT12, input int PHT13, input int PHT14, input int PHT15, input int GHR);
always@(negedge clk) begin
  if (prev_branch_in) begin
    btb_update_print(prev_taken, PHT[0], PHT[1], PHT[2],PHT[3],PHT[4],PHT[5],PHT[6],PHT[7],PHT[8],PHT[9],PHT[10],PHT[11],PHT[12],PHT[13],PHT[14],PHT[15], GHR);
  end;
end
`endif

endmodule
