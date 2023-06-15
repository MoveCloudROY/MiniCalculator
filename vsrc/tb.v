`timescale 1ns / 1ns
module MiniCalculator_tb;
// Inputs
reg clk;
reg rst;
reg [3:0] btn;
reg [7:0] sw;
reg con;
// Outputs
wire [6:0] seg;
wire [3:0] an;
wire led;


// Instantiate
top top_tb (
    .clk(clk),
    .rst(rst),
    .btn_i(btn),
    .sw_i(sw),
    .seg_o(seg),
    .an_o(an),
    .led_o(led),
    .con_i(con)
);
integer      i;
initial begin
// Initialize Inputs
clk = 0;
rst = 1;
btn = 4'b0000;
sw = 8'b0010_0010;
con = 0;
#10;
rst = 0;
#30;
btn = 4'b1000;

for (i = 0; i < 2000; i=i+1) begin
    #300000000
    con = 0;
end


btn = 4'b0000;
end

always #5 clk = ~clk;
// 产生随机乘数和有符号控制信号
// always @(posedge clk) begin
//     op1 <= $random;
//     op2 <= $random; // $random为系统任务，产生一个随机的32位有符号数
//     // op1 <= -64'd1;
//     // op2 <= 64'd2;
// end

endmodule
