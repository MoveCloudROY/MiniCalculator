module Buttons(
    input clk,
    input [3:0] btn_i,
    output [3:0] btn_o
);
reg [3:0] btn;
assign btn_o = btn;

// 每个时钟上升沿, 读取一次按键的值;
// 输出中只有一位为1, 其余为0, 无论多少个按键按下
always @(posedge clk) begin
    if (btn_i[3] == 1'b1) begin // 加法 L
        btn <= 4'b1000;
    end else if (btn_i[2] == 1'b1) begin // 减法 R
        btn <= 4'b0100;
    end else if (btn_i[1] == 1'b1) begin // 乘法 U
        btn <= 4'b0010;
    end else if (btn_i[0] == 1'b1) begin // 除法 D
        btn <= 4'b0001;
    end else begin // 没有按键按下
        btn <= 4'b0000;
    end
end

endmodule