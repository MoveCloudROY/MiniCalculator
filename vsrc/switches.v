module Switches(
    input clk,
    input [7:0] sw_i,
    output [7:0] sw_o
);
reg [7:0] sw;
assign sw_o = sw;

// 每个时钟上升沿, 读取一次开关的值, 同步复位
always @(posedge clk) begin
    sw <= sw_i;
end

endmodule