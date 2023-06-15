module Buttons(
    input clk,
    input rst,
    input [3:0] btn_i,
    output [3:0] btn_o
);
reg [3:0] btn;
reg [3:0] btn_prev; // 上一次的按键值
reg [23:0] counter;
assign btn_o = btn;

always @(posedge clk) begin
    btn_prev <= btn_i;
end

always @(posedge clk) begin
    if (rst) begin
        counter <= 24'd0;
    end else if (btn_i != btn_prev) begin
        counter <= 24'd0;
    end else if (counter == 24'hffff_ff) begin
        counter <= 24'd0;
    end else begin
        counter <= counter + 24'd1;
    end
end

// 每个时钟上升沿, 读取一次按键的值;
// 输出中只有一位为1, 其余为0, 无论多少个按键按下
always @(posedge clk) begin
    if (btn_prev[3] == 1'b1 && counter == 24'hffff_ff) begin // 加法
        btn <= 4'b1000;
    end else if (btn_prev[2] == 1'b1 && counter == 24'hffff_ff) begin // 减法
        btn <= 4'b0100;
    end else if (btn_prev[1] == 1'b1 && counter == 24'hffff_ff) begin // 乘法
        btn <= 4'b0010;
    end else if (btn_prev[0] == 1'b1 && counter == 24'hffff_ff) begin // 除法
        btn <= 4'b0001;
    end else begin // 没有按键按下
        btn <= 4'b0000;
    end
end

endmodule