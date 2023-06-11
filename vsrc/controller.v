module controller(
    input clk,
    input rst,
    input [7:0] sw_i,           // 开关 
    input [3:0] btn_i,          // 按键
    input wire alu_busy_i,      // 运算器忙
    input [7:0] alu_i,          // 运算器结果

    output [15:0] seg_num_o,    // 数码管显示的数字
    output [3:0] seg_sel_o,     // 数码管选择
    output [3:0] alu_op_o,      // 运算器的运算
    output wire led_o           // 除零LED
);

wire event_ProDiv; // 乘法/除法事件(非除零)
assign event_ProDiv = (btn_i[1] | btn_i[0]) & ~event_Zero;
wire event_AddSub; // 加法/减法事件
assign event_AddSub = btn_i[3] | btn_i[2];
wire event_Zero;   // 除零事件
assign event_Zero = btn_i[0] & (sw_i[3:0] == 4'b0000);
wire event_ALUEND; // 运算结束事件
assign event_ALUEND = ~alu_busy_i;

wire[2:0] next_state; // 下一个状态

// 状态寄存器: 
// 000: 初始状态
// 001: 开始加法/减法
// 010: 结束加法/减法
// 011: 开始乘法/除法
// 100: 结束乘法/除法
// 101: 停机(除零)
reg [2:0] current_state; // 当前状态
reg [7:0] current_alu_sum; // 当前运算器的结果

always @(posedge clk) begin
    if (
        ((current_state == 3'b001) || (current_state == 3'b011)) && 
        ((next_state == 3'b010) || (next_state == 3'b100))
       ) begin  // 当运算结束时, 将运算结果保存到current_alu_sum中
        current_alu_sum <= alu_i;
    end else begin
        current_alu_sum <= current_alu_sum;
    end
end

// 1. 状态转移
always @(posedge clk) begin
    if (rst) begin
        current_state <= 3'b000;
    end else begin      
        current_state <= next_state;
    end
end

// 2. 状态转移逻辑
always @(*) begin
    next_state = current_state; // 赋初值, 防止Latch的出现
    case (current_state)
        3'b000: // 初始状态
            if (event_AddSub) begin
                next_state = 3'b001;
            end else if (event_ProDiv) begin
                next_state = 3'b011;
            end else if (event_Zero) begin
                next_state = 3'b101;
            end else begin
                next_state = 3'b000;
            end
        3'b001: // 开始加法/减法
            if (event_ALUEND) begin
                next_state = 3'b010;
            end else begin
                next_state = 3'b001;
            end
        3'b010: // 结束加法/减法
            if (event_AddSub) begin
                next_state = 3'b001;
            end else if (event_ProDiv) begin
                next_state = 3'b011;
            end else if (event_Zero) begin
                next_state = 3'b101;
            end else begin
                next_state = 3'b010;
            end
        3'b011: // 开始乘法/除法
            if (event_ALUEND) begin
                next_state = 3'b100;
            end else begin
                next_state = 3'b011;
            end
        3'b100: // 结束乘法/除法
            if (event_AddSub) begin
                next_state = 3'b001;
            end else if (event_ProDiv) begin
                next_state = 3'b011;
            end else if (event_Zero) begin
                next_state = 3'b101;
            end else begin
                next_state = 3'b100;
            end
        3'b101: // 停机(除零)
            next_state = 3'b101; // 保持
        default: 
            next_state = 3'b101;
    endcase
end

// 3. 输出逻辑
always @(*) begin
    seg_sel_o = 4'b0000;
    alu_op_o = 4'b0000;
    led_o = 1'b1;
    case (current_state)
        3'b000: // 初始状态
            seg_sel_o = 4'b0000;
            alu_op_o = 4'b0000;
            led_o = 1'b0;
        3'b001: // 开始加法/减法
            seg_sel_o = 4'b1100;
            alu_op_o = (btn_i[3]) ? 4'b1000 : 4'b0100;
            led_o = 1'b0;
        3'b010: // 结束加法/减法
            seg_sel_o = 4'b1101;
            alu_op_o = 4'b0000;
            led_o = 1'b0;
        3'b011: // 开始乘法/除法
            seg_sel_o = 4'b1100;
            alu_op_o = (btn_i[1]) ? 4'b0010 : 4'b0001;
            led_o = 1'b0;
        3'b100: // 结束乘法/除法
            seg_sel_o = 4'b1111;
            alu_op_o = 4'b0000;
            led_o = 1'b0;
        3'b101: // 停机(除零)
            seg_sel_o = 4'b0000;
            alu_op_o = 4'b0000;
            led_o = 1'b1;
        default: 
            seg_sel_o = 4'b0000;
            alu_op_o = 4'b0000;
            led_o = 1'b1;
    endcase
end

// seg_num_o
assign seg_num_o[15:12] = sw_i[7:4];
assign seg_num_o[11:8] = sw_i[3:0];
assign seg_num_o[7:4] = current_alu_sum[7:4];
assign seg_num_o[3:0] = current_alu_sum[3:0];

endmodule