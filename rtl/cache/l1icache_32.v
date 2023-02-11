module  l1icache_32 
#(
    parameter VIRTUAL_ADDR_LEN = 32,
    parameter WB_DATA_LEN = 32
)
(
    `ifdef USE_POWER_PINS
        inout vccd1,	// User area 1 1.8V supply
        inout vssd1,	// User area 1 digital ground
    `endif

    /* verilator lint_off UNUSED */
    input clk,
    input rstn,
    
    //req
    input  req_valid_i,
    output req_ready_o,
    input [VIRTUAL_ADDR_LEN - 1 : 0] req_addr_i,
    
    //sram 这里的output是输出到sram, input读取sram的结果
    output tag_chip_en,
    output tag_write_en,
    output [3:0] write_tag_mask,
    output [7:0] tag_index,
    output [31:0] tag_data_in,
    input  [31:0] tag_out, 

    // first 32 bits
    output data_chip_en_0,
    output data_write_en_0,
    output [3:0] write_data_mask_0,
    output [7:0] data_index_0,
    output [31:0] data_in_0,
    input  [31:0] data_out_0, 

    // second 32 bits
    output data_chip_en_1,
    output data_write_en_1,
    output [3:0] write_data_mask_1,
    output [7:0] data_index_1,
    output [31:0] data_in_1,
    input  [31:0] data_out_1,

    //resp
    output resp_valid_o,
    output [31:0] ld_data_o,
    input  resp_ready_i,
    output [31:0] resp_addr_o,
    //memory
    output wb_cyc_o,
    output wb_stb_o,
    output wb_we_o,
    output [VIRTUAL_ADDR_LEN - 1 : 0] wb_adr_o,
    output [9:0] wb_bl_o,
    output wb_bry_o,
    input  wb_ack_i,
    input  [WB_DATA_LEN -1:0] wb_dat_i
);
reg [31:0] req_addr;
wire req_hsk;
always @(posedge clk) begin
    if(req_hsk) begin
        req_addr <= req_addr_i;
    end
end

assign resp_addr_o = req_addr;


// wire refill_hsk;
reg refill_hsk;
wire req_hsk_q;
reg mshr_is_full = '0;
reg [VIRTUAL_ADDR_LEN - 1 : 0] addr_save;
reg [VIRTUAL_ADDR_LEN - 1 : 0] addr_save_plus4;
reg [VIRTUAL_ADDR_LEN - 1 : 0] addr_save_align8;
reg [VIRTUAL_ADDR_LEN - 1 : 0] addr_save_align8_q;
assign addr_save_align8 = req_addr_i % 8 == 0 ? req_addr_i : req_addr_i - 4;
std_dffr #(.WIDTH(32)) ADDR_SAVE_ALIGN8 (.clk(clk),.rstn(rstn),.d(addr_save_align8),.q(addr_save_align8_q));
wire [31:0] ld_data_cache;
wire [31:0] ld_data_refill;
wire ld_tag_is_hit;
wire ld_tag_is_miss;
wire ld_tag_is_hit_q;
wire ld_tag_is_miss_q;
wire burst;
wire burst_q;
wire [7:0] index;
reg [31:0] refill_buffer;
wire [31:0] refill_buffer_q;
// wire [31:0] refill_data;
wire resp_valid_q;
wire resp_hsk;
wire resp_hsk_q;
wire [31:0] ld_data_q;
wire [31:0] tag_dout1;
wire [31:0] data_dout1;
/* verilator lint_off PINCONNECTEMPTY */

 
/////////clean sram
reg [7:0] counter = 8'b00000000;
reg reset = '1;
reg start;
always @(posedge clk) begin

    if(rstn) begin
    reset <= '1;
    if(counter != 8'b11111111) begin
        counter <= counter +1;
    end else if(counter == 8'b11111111) begin
        counter <= 8'b00000000;
    end

    end else begin
        reset <= '0;
    end
end
assign tag_index = index;
assign data_index_0 = index;
assign data_index_1 = index;

assign tag_chip_en = ~(req_hsk | reset | refill_hsk);
assign data_chip_en_0 = ~(req_hsk | reset | refill_hsk);
assign data_chip_en_1 = ~(req_hsk | reset | refill_hsk);
// mshr: 向dram传的地址； refill_hsk：从mem读回了数据，要往addr所在的cache里写
// req_hsk ? req_addr_i[9:2] : 应该是个bug，改成

assign index = reset ? counter :
               refill_hsk ? addr_save_plus4[9:2] :
               req_hsk ? req_addr_i[9:2] :
               8'b0;


assign data_write_en_0 = ~(reset | (refill_hsk & ~burst));
assign data_write_en_1 = ~(reset | (refill_hsk & ~burst));
assign tag_write_en = ~(reset | refill_hsk);


assign write_data_mask_0 = reset ? 4'b1111 :
                         refill_hsk ? 4'b1111 :
                         4'b0;
assign write_data_mask_1 = reset ? 4'b1111 :
                         refill_hsk ? 4'b1111 :
                         4'b0;

assign write_tag_mask = reset ? 4'b1111 :
                        refill_hsk ? 4'b1111 :
                        4'b0;

assign data_in_0 = reset ? 32'hFFFFFFFF:
                 refill_hsk ? refill_buffer :
                 32'b0;
assign data_in_1 = reset ? 32'hFFFFFFFF:
                 refill_hsk ? wb_dat_i :
                 32'b0;                 
                
assign tag_data_in = reset ? 32'hFFFFFFFF :// F
                     refill_hsk ? {10'b0,addr_save[31:10]} :
                     32'b0;                       
///////////////save tag data out
reg [31:0] tag_out_save;
reg [31:0] data_out_save_0;
reg [31:0] data_out_save_1;

  always @(posedge clk) begin
    // $display("-------");
    // $display("burst %h", burst);
    // $display("wb_ack_i %h", wb_ack_i);
    // $display("wb_cyc_o %h",  wb_cyc_o);
    // $display("refill_hsk %h", refill_hsk);
    // $display("wb_adr_o %h", wb_adr_o);
    // $display("refill_buffer %h", refill_buffer);
    // if (wb_ack_i) begin
    //   $display("wb_ack_i == 1");
    // end else if (!wb_ack_i) begin
    //   $display("wb_ack_i == 0");
    // end else begin
    //   $display("wb_ack_i != 0 and != 1");
    // end
    // if (!tag_write_en) begin
    //   $display("write %h %h %h", index, data_in_0, data_in_1 );
    // end
    // if (req_hsk) begin
    //   $display("req_addr_i %h ", req_addr_i );
    // end
    // if(refill_hsk) begin
    //   $display("refill_hsk %h %h ", refill_buffer ,wb_dat_i );
    //   $display("data_in_0 %h  data_in_1 %h", data_in_0, data_in_1);
    //   $display("addr_save_plus4 %h", addr_save_plus4);
    // end
    // if (resp_valid_o) begin
    //     $display("resp_valid_o %h %h", resp_addr_o, ld_data_o);
    // end
    // if (ld_tag_is_hit) begin
    //     $display("ld_tag_is_hit %h %h",  data_out_0, data_out_1);
    // end
  end

always @(posedge clk) begin
    if(req_hsk_q) begin
        tag_out_save <= tag_out;
        data_out_save_0 <= data_out_0;
        data_out_save_1 <= data_out_1;
    end
end

///////////////save req 



always @(posedge clk) begin
    if (req_hsk) begin
        addr_save <= req_addr_i;
        if (req_addr_i % 8 == 0) begin
            addr_save_plus4 <= req_addr_i;
            // addr_save_align8 <= req_addr_i;
        end else begin
            addr_save_plus4 <= req_addr_i - 4;
            // addr_save_align8 <= req_addr_i - 4;
        end
    // end else if (wb_ack_i & burst) begin
    end else if (refill_hsk) begin
        addr_save_plus4 <= addr_save_plus4 + 4;

    end
end
always @(posedge clk) begin
    if(~rstn) begin
        start <= 1'b1;
    end  
    if(rstn) begin
        start <= 1'b0;
    end
end
assign req_hsk = req_valid_i & req_ready_o & start;
assign ld_tag_is_hit = req_hsk_q & (addr_save_align8[31:10] == tag_out[21:0]) & (tag_out[30] == 0) & (data_out_0 != 32'hFFFFFFFF);

assign ld_tag_is_miss = req_hsk_q ? ~ld_tag_is_hit : 1'b0;
std_dffr #(.WIDTH(1)) REQ_HSK_1 (.clk(clk),.rstn(rstn),.d(req_hsk),.q(req_hsk_q));
std_dffr #(.WIDTH(1)) S1_LD_TAG_IS_HIT (.clk(clk),.rstn(rstn),.d(ld_tag_is_hit),.q(ld_tag_is_hit_q));
std_dffr #(.WIDTH(1)) S1_LD_TAG_IS_MISS (.clk(clk),.rstn(rstn),.d(ld_tag_is_miss),.q(ld_tag_is_miss_q));


/////miss
always @(posedge clk) begin
    if(rstn) begin
        mshr_is_full <= '0;
    end
    if(ld_tag_is_miss & ~mshr_is_full) begin
        mshr_is_full <= '1;
    end 
    if(refill_hsk & ~burst) begin
        mshr_is_full <= '0;
    end

end

// assign refill_data = wb_ack_i ? wb_dat_i : 32'b0;

wire cache_mem_is_write;
assign cache_mem_is_write = 1'b0;

assign wb_stb_o = wb_cyc_o;
assign wb_cyc_o = 
                  (refill_hsk) ? 1'b0:
                  mshr_is_full ? 1'b1 :
                  1'b0;
assign wb_bl_o = wb_stb_o ? 10'b0000000001 : 10'b0000000000;
assign wb_bry_o = wb_stb_o;


assign wb_we_o = cache_mem_is_write;
assign wb_adr_o = mshr_is_full ? addr_save_plus4 :  
                   32'b0;                                      

///refill
always @(posedge clk) begin
    refill_hsk <= wb_ack_i;
end
// 这里memory令ack上升是先于时钟上升沿的，所以要进行打拍，若ack是同时上升则应该使用assign
//但是若模拟器采用这种行为，则dcache的refill_hsk信号行为会不稳定（vcs中稳定，目测是verilator模拟问题）
//这个要接上Soc后进一步确定
// FIXME
// assign refill_hsk = wb_ack_i;

//req_ready_o
assign req_ready_o = rstn ? 1'b1:
                     refill_hsk ? 1'b0:
                     req_hsk_q ? 1'b0:
                     burst ? 1'b0 :
                     resp_valid_o & ~resp_ready_i ? 1'b0:
                     mshr_is_full ? 1'b0:
                     1'b1;
// 不reset 且 没有从mem取回结果 且 没有require 且 

std_dffr #(.WIDTH(32)) REFILL_BUFFER (.clk(clk),.rstn(rstn),.d(refill_buffer),.q(refill_buffer_q));

assign refill_buffer = (wb_ack_i & burst) ? wb_dat_i :
                       refill_buffer_q;

// 第一次wb则1，第二次wb则此次读取完成并置为0
assign burst = (wb_ack_i & ~burst_q) ? 1'b1:
               (wb_ack_i & burst_q) ? 1'b0:
               burst_q;
std_dffr #(.WIDTH(1)) BURST (.clk(clk),.rstn(rstn),.d(burst),.q(burst_q));


/////resp

assign ld_data_cache = ld_tag_is_hit_q ? (req_addr_i % 8 == 0 ? data_out_save_1 : data_out_save_0) :
                       64'b0;

assign ld_data_refill = refill_hsk ? (addr_save % 8 == 0 ? refill_buffer : wb_dat_i ) :
                        32'b0;

assign ld_data_o =   refill_hsk ? ld_data_refill :
                     ld_tag_is_hit_q ? ld_data_cache :
                     resp_valid_o ? ld_data_q :
                     32'b0;
std_dffr #(.WIDTH(32)) LD_DATA (.clk(clk),.rstn(rstn),.d(ld_data_o),.q(ld_data_q));

assign resp_valid_o = resp_hsk_q ? 1'b0:
                      (refill_hsk & ~burst) ? 1'b1 :
                      ld_tag_is_hit_q ? 1'b1 :
                      resp_valid_q;


assign resp_hsk = resp_valid_o & resp_ready_i;
std_dffr #(.WIDTH(1)) RESP_VALID (.clk(clk),.rstn(rstn),.d(resp_valid_o),.q(resp_valid_q));
std_dffr #(.WIDTH(1)) RESP_HSK (.clk(clk),.rstn(rstn),.d(resp_hsk),.q(resp_hsk_q));
/* verilator lint_on PINCONNECTEMPTY */
    /* verilator lint_on UNUSED */

reg [31:0] numRead = 0;
reg [31:0] numWrite = 0;
reg [31:0] numHit = 0;
reg [31:0] numMiss = 0;

always @(posedge clk) begin
    if (ld_tag_is_hit) begin
        numHit <= numHit + 1;
    end
    if(ld_tag_is_miss) begin
        numMiss <= numMiss + 1;
    end
    if (!tag_chip_en) begin
        numRead <= numRead + 1;
    end
    if (!data_write_en_0) begin
        numWrite <= numWrite + 1;
    end
end

`ifdef COSIM
import "DPI-C" function void icache_print(input int numRead, input int numWrite, input int numHit, input int numMiss);

always @(resp_valid_o) begin
    icache_print(numRead, numWrite, numHit, numMiss);
end
`endif

endmodule