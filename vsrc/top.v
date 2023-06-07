module top (
    input clk,
    input rst,
    input [3:0] a,
    input [3:0] b,
    input [3:0] op,
    output [7:0] o,
    output wire busy
);


    alu alu_t(
        .clk(clk),
        .rst(rst),
        .data1 (a),
        .data2 (b),
        .op(op),
        .busy(busy),
        .o(o)
    );

endmodule