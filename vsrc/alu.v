module alu (
    input rst,
    input clk,
    input [3:0] op,
    input [3:0] a,
    input [3:0] b,
    output busy,
    output [7:0] o

);
    always @(posedge clk) begin 
        if (rst) begin
            assign busy = 0;
            assign output = 0;
        end

    end

endmodule