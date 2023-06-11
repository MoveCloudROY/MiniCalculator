module adder_carry_ahead(
    rst_n,//
    clk,//
    data1,
    data2,
    cin,
    sum
);
input rst_n;
input clk;
input [3 : 0] data1;
input [3 : 0] data2;
input cin;

output [4 : 0] sum;

reg [3 : 0] data1_r;
reg [3 : 0] data2_r;
reg cin_r;

wire [3 : 0] G;
wire [3 : 0] P;

wire [4: 0] sum_tmp;
wire [3 : 0] cout_tmp;

reg [4 : 0] sum;

always@(posedge clk)
    if(!rst_n)
        begin
            data1_r <= 4'd0;
            data2_r <= 4'd0;
            cin_r <= 1'b0;
            sum <= 5'd0;
        end
    else
        begin
            data1_r <= data1;
            data2_r <= data2;
            cin_r <= cin;
            sum <= sum_tmp;
        end
assign G[0] =  data1_r[0] & data2_r[0];        //第0bit加法
assign P[0] =  data1_r[0] | data2_r[0];
assign sum_tmp[0] = data1_r[0] ^ data2_r[0] ^ cin_r;
assign cout_tmp[0] =  G[0] | P[0] & cin_r;

assign G[1] =  data1_r[1] & data2_r[1];        //第1bit加法
assign P[1] =  data1_r[1] | data2_r[1];
assign sum_tmp[1] = data1_r[1] ^ data2_r[1] ^ cout_tmp[0];
assign cout_tmp[1] =  G[1] | P[1] & cout_tmp[0];

assign G[2] =  data1_r[2] & data2_r[2];        //第2bit加法
assign P[2] =  data1_r[2] | data2_r[2];
assign sum_tmp[2] = data1_r[2] ^ data2_r[2] ^ cout_tmp[1];
assign cout_tmp[2] =  G[2] | P[2] & cout_tmp[1];

assign G[3] =  data1_r[3] & data2_r[3];        //第3bit加法
assign P[3] =  data1_r[3] | data2_r[3];
assign sum_tmp[3] = data1_r[3] ^ data2_r[3] ^ cout_tmp[2];
assign cout_tmp[3] =  G[3] | P[3] & cout_tmp[2];

assign sum_tmp[4] = data1_r[3] ^ data2_r[3] ^ cout_tmp[3];

endmodule