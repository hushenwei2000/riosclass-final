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

module dpram64 #(
parameter SIZE=0, // byte
parameter mem_clear = 0,
parameter memfile = ""
) (
input wire clk,
input wire we,
input wire [31:0] 		 din,
input wire [31:0]        waddr,
input wire [31:0]        raddr,
output reg [31:0] 		 dout
);

    localparam AW = $clog2(SIZE);

    reg [7:0] mem [0:SIZE/8-1] /* verilator public */;

    integer i;
    always @(posedge clk) begin
        if (we) begin
            for(i = 0; i < 4; i = i + 1)
                mem[waddr+i] <= din[i*8 +: 8];
        end
        dout <= {mem[raddr+3], mem[raddr+2], mem[raddr+1], mem[raddr]}; //little-end
    end

generate
initial begin
    if (mem_clear)
        for (i=0;i< SIZE/(32/8) ;i=i+2) begin
        //mem[i] = {AXI_DATA_WIDTH{1'b1}};
        mem[i] = {32'h0000_0013};
        mem[i+1] = {32'h0000_0013};
        end
    if(|memfile) begin
        $display("Preloading %m from %s", memfile);
        $readmemh(memfile, mem);
    end
end
endgenerate

endmodule
