// SPDX-License-Identifier: Apache-2.0
// Copyright 2019 Western Digital Corporation or its affiliates.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

//********************************************************************************
// $Id$
//
// Function: Basic RAM model with separate read/write ports and byte-wise write enable
// Comments:
//
//********************************************************************************

module dpram64
  #(parameter SIZE=0, // byte
    parameter AXI_DATA_WIDTH=520,
    parameter mem_clear = 0,
    parameter mem_simple_seq = 0,
    parameter memfile = "",
    parameter INDEX_NUM    = SIZE/(AXI_DATA_WIDTH/8),
    parameter INDEX_WIDTH  = $clog2(INDEX_NUM)
    )
  (input wire clk,
   input wire [AXI_DATA_WIDTH/8-1:0] 		 we_d,         //_d means communicating with data cache
   input wire [AXI_DATA_WIDTH-1:0] 		 din_d,           
   input wire [INDEX_WIDTH-1:0]        waddr_d,          
   input wire [INDEX_WIDTH-1:0]        raddr_d,
   output reg [AXI_DATA_WIDTH-1:0] 		 dout_d,
   
   input wire [INDEX_WIDTH-1:0]        raddr_instr,
   output reg [AXI_DATA_WIDTH-1:0] 		 dout_instr

   );


   reg [AXI_DATA_WIDTH-1:0] mem [0:INDEX_NUM-1] /* verilator public */;
   reg [AXI_DATA_WIDTH-1:0] small_mem [0:126];        //for test

   integer 	 i;
   wire [INDEX_WIDTH-$clog2(AXI_DATA_WIDTH/8)-1:0] wadd_d = waddr_d[INDEX_WIDTH-1:$clog2(AXI_DATA_WIDTH/8)];

// data
   always @(posedge clk) begin
      for(i = 0; i < AXI_DATA_WIDTH/8; i++) begin
         if (we_d[i]) mem[wadd_d][ i*8+:8] <= din_d[ i*8+:8];
      end
      dout_d <= mem[raddr_d[INDEX_WIDTH-1:$clog2(AXI_DATA_WIDTH/8)]];
   end

// instr
   always @(posedge clk) begin
      dout_instr <= mem[raddr_instr[INDEX_WIDTH-1:$clog2(AXI_DATA_WIDTH/8)]];
   end
   
   generate
   initial begin
      if (mem_clear)
         for (i=0;i< SIZE/(AXI_DATA_WIDTH/8) ;i=i+1)
         mem[i] = {AXI_DATA_WIDTH{1'b0}};
      if (mem_simple_seq)
         for (i=0;i< SIZE/(AXI_DATA_WIDTH/8) ;i=i+1)
         mem[i] = {{(AXI_DATA_WIDTH-32){1'b0}}, i[32-1:0]};
      if(|memfile) begin
         $display("Preloading %m from %s", memfile);
         $readmemh(memfile, mem);
         $readmemh(memfile, small_mem);
      end
   end
   endgenerate

endmodule
