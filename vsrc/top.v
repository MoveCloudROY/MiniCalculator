module top (
    input clk,
    input rst,
    input [3:0] btn_i,
    input [7:0] sw_i,
    input wire con_i,
    output [6:0] seg_o,
    output [3:0] an_o,
    output wire led_o
);
    wire [7:0] sw_t;
    wire [3:0] btn_t;
    wire [15:0] num_t;
    wire [3:0] sel_t;
    wire [7:0] alu_t;
    wire [3:0] alu_op;
    wire alu_busy;
    wire [3:0]op1_t;

    assign op1_t = (con_i == 1'b1) ? num_t[3:0] : sw_t[7:4];

    Controller ctrl(
        .clk(clk),
        .rst(rst),
        .sw_i(sw_t),
        .btn_i(btn_t),
        .alu_busy_i(alu_busy),
        .alu_i(alu_t),
        .seg_num_o(num_t),
        .seg_sel_o(sel_t),
        .alu_op_o(alu_op),
        .led_o(led_o)
    );

    Switches sw(
        .clk(clk),
        .sw_i(sw_i),
        .sw_o(sw_t)
    );

    Buttons btn(
        .clk(clk),
        .rst(rst),
        .btn_i(btn_i),
        .btn_o(btn_t)
    );

    Seg7 seg7(
        .num(num_t),
        .clk(clk),
        .rst(rst),
        .sel(sel_t),
        .seg(seg_o),
        .an(an_o)
    );

    ALU alu(
        .clk(clk),
        .rst(rst),
        .sign(1),
        .data1 (op1_t),
        .data2 (sw_t[3:0]),
        .op(alu_op),
        .busy(alu_busy),
        .o(alu_t)
    );

endmodule