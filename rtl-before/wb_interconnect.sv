module wb_interconnect  (
         input logic		clk_i, 
         input logic            rst_n,

         // Master 0 Interface
         input   logic	[31:0]	m0_wbd_dat_i,
         input   logic  [31:0]	m0_wbd_adr_i,
         input   logic  [3:0]	m0_wbd_sel_i,
         input   logic  	m0_wbd_we_i,
         input   logic  	m0_wbd_cyc_i,
         input   logic  	m0_wbd_stb_i,
         output  logic	[31:0]	m0_wbd_dat_o,
         output  logic		m0_wbd_ack_o,
         output  logic		m0_wbd_lack_o,
         output  logic		m0_wbd_err_o,
         
         // Master 1 Interface
         input	logic [31:0]	m1_wbd_dat_i,
         input	logic [31:0]	m1_wbd_adr_i,
         input	logic [3:0]	m1_wbd_sel_i,
         input	logic [2:0]	m1_wbd_bl_i,
         input	logic    	m1_wbd_bry_i,
         input	logic 	        m1_wbd_we_i,
         input	logic 	        m1_wbd_cyc_i,
         input	logic 	        m1_wbd_stb_i,
         output	logic [31:0]	m1_wbd_dat_o,
         output	logic 	        m1_wbd_ack_o,
         output	logic 	        m1_wbd_lack_o,
         output	logic 	        m1_wbd_err_o,

         // Slave 0 Interface
         input	logic [31:0]	s_wbd_dat_i,
         input	logic 	        s_wbd_ack_i,
         input	logic 	        s_wbd_lack_i,
         output	logic [31:0]	s_wbd_dat_o,
         output	logic [31:0]	s_wbd_adr_o,
         output	logic [3:0]	s_wbd_sel_o,
         output	logic [9:0]	s_wbd_bl_o,
         output	logic 	        s_wbd_bry_o,
         output	logic 	        s_wbd_we_o,
         output	logic 	        s_wbd_cyc_o,
         output	logic 	        s_wbd_stb_o

	);

// WishBone Wr Interface
typedef struct packed { 
  logic	[31:0]	wbd_dat;
  logic  [31:0]	wbd_adr;
  logic  [3:0]	wbd_sel;
  logic  [9:0]	wbd_bl;
  logic  	wbd_bry;
  logic  	wbd_we;
  logic  	wbd_cyc;
  logic  	wbd_stb;
  logic [3:0] 	wbd_tid; // target id
} type_wb_wr_intf;

// WishBone Rd Interface
typedef struct packed { 
  logic	[31:0]	wbd_dat;
  logic  	wbd_ack;
  logic  	wbd_lack;
  logic  	wbd_err;
} type_wb_rd_intf;


// Master Write Interface
type_wb_wr_intf  m0_wb_wr;
type_wb_wr_intf  m1_wb_wr;

// Master Read Interface
type_wb_rd_intf  m0_wb_rd;
type_wb_rd_intf  m1_wb_rd;

type_wb_wr_intf  m_bus_wr;  // Multiplexed Master I/F
type_wb_rd_intf  m_bus_rd;  // Multiplexed Slave I/F

wire m0_stb_i = m0_wbd_stb_i ;
wire m1_stb_i = m1_wbd_stb_i ;

//----------------------------------------
// Master Mapping
// -------------------------------------
assign m0_wb_wr.wbd_dat = m0_wbd_dat_i;
assign m0_wb_wr.wbd_adr = {m0_wbd_adr_i[31:2],2'b00};
assign m0_wb_wr.wbd_sel = m0_wbd_sel_i;
assign m0_wb_wr.wbd_bl  = 'h1;
assign m0_wb_wr.wbd_bry = 'b1;
assign m0_wb_wr.wbd_we  = m0_wbd_we_i;
assign m0_wb_wr.wbd_cyc = m0_wbd_cyc_i;
assign m0_wb_wr.wbd_stb = m0_wbd_cyc_i;

assign m1_wb_wr.wbd_dat = m1_wbd_dat_i;
assign m1_wb_wr.wbd_adr = {m1_wbd_adr_i[31:2],2'b00};
assign m1_wb_wr.wbd_sel = m1_wbd_sel_i;
assign m1_wb_wr.wbd_bl  = {7'b0,m1_wbd_bl_i};
assign m1_wb_wr.wbd_bry = m1_wbd_bry_i;
assign m1_wb_wr.wbd_we  = m1_wbd_we_i;
assign m1_wb_wr.wbd_cyc = m1_wbd_cyc_i;
assign m1_wb_wr.wbd_stb = m1_wbd_cyc_i;

assign m0_wbd_dat_o  =  m0_wb_rd.wbd_dat;
assign m0_wbd_ack_o  =  m0_wb_rd.wbd_ack;
assign m0_wbd_lack_o =  1'b0;
assign m0_wbd_err_o  =  m0_wb_rd.wbd_err;

assign m1_wbd_dat_o  =  m1_wb_rd.wbd_dat;
assign m1_wbd_ack_o  =  m1_wb_rd.wbd_ack;
assign m1_wbd_lack_o =  1'b0;
assign m1_wbd_err_o  =  m1_wb_rd.wbd_err;

//
// arbitor 
//
logic   gnt;
wb_arb	u_wb_arb(
	.clk(clk_i), 
	.rstn(rst_n),
	.req({	m1_stb_i & !m1_wbd_lack_o,
		      m0_stb_i & !m0_wbd_lack_o}),
	.gnt(gnt)
);

// Generate Multiplexed Master Interface based on grant
always_comb begin
     case(gnt)
        1'h0:	   m_bus_wr = m0_wb_wr;
        1'h1:	   m_bus_wr = m1_wb_wr;
        default:   m_bus_wr = m1_wb_wr;
     endcase			
end

// Stagging FF to break write and read timing path
assign m_bus_rd.wbd_dat  = s_wbd_dat_i;
assign m_bus_rd.wbd_ack  = s_wbd_ack_i;
assign m_bus_rd.wbd_lack = s_wbd_lack_i;
assign s_wbd_dat_o  = m_bus_wr.wbd_dat;
assign s_wbd_adr_o  = m_bus_wr.wbd_adr;
assign s_wbd_sel_o  = m_bus_wr.wbd_sel;
assign s_wbd_bl_o   = m_bus_wr.wbd_bl;
assign s_wbd_bry_o  = m_bus_wr.wbd_bry;
assign s_wbd_we_o   = m_bus_wr.wbd_we;
assign s_wbd_cyc_o  = m_bus_wr.wbd_cyc;
assign s_wbd_stb_o  = m_bus_wr.wbd_stb;

// Connect Slave to Master
assign  m0_wb_rd = (gnt == 2'b00) ? m_bus_rd : 'h0;
assign  m1_wb_rd = (gnt == 2'b01) ? m_bus_rd : 'h0;

endmodule
