module alu (
    input rst,
    input clk,

    // 1000 0100 0010 0001 0000
    //  +    -    *    /   STOP
    input [3:0] op,
    input [3:0] a,
    input [3:0] b,
    output busy,
    output [7:0] o
);
    reg reg_busy;
    reg [8:0] reg_o;

    always @(posedge clk) begin 
        if (rst) begin
            reg_busy <= 0;
            assign busy = 0;
            assign output = 0;
        end
        

        casez (op)
            2'b1000: begin
                tmp_o = a + b;
            end 
            2'b0100: begin
                tmp_o = a - b;
            end
            2'b0010: begin
                
            end

            2'b0001: begin

            end

            default: begin
            
            
            end 
        endcase
        
        assign reg = reg_busy;
        assign o = reg_o[7:0];
    end

endmodule