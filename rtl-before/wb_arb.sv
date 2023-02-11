module wb_arb(clk, rstn, req, gnt);

input		clk;
input		rstn;
input	[1:0]	req;	// Req input
output			gnt; 	// Grant output

///////////////////////////////////////////////////////////////////////
//
// Parameters
//


parameter	
                grant0 = 1'h0,
                grant1 = 1'h1;

///////////////////////////////////////////////////////////////////////
// Local Registers and Wires
//////////////////////////////////////////////////////////////////////

reg [1:0]	state, next_state;

///////////////////////////////////////////////////////////////////////
//  Misc Logic 
//////////////////////////////////////////////////////////////////////

assign	gnt = state;

always@(posedge clk or negedge rstn)
	if(!rstn)       state <= grant1;
	else		state <= next_state;

///////////////////////////////////////////////////////////////////////
//
// Next State Logic 
//   - implements round robin arbitration algorithm
//   - switches grant if current req is dropped or next is asserted
//   - parks at last grant
//////////////////////////////////////////////////////////////////////

always@(state or req )
   begin
      next_state = state;	// Default Keep State
      case(state)		
         grant0:
      	// if this req is dropped or next is asserted, check for other req's
      	if(!req[0] ) begin
      		if(req[1])	next_state = grant1;
      	end
         grant1:
      	// if this req is dropped or next is asserted, check for other req's
      	if(!req[1] ) begin
      		if(req[0])	next_state = grant0;
      	end
      endcase
   end

endmodule 

