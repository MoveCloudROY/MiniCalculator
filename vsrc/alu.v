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
    
    reg [2:0] reg_calc_cnt;
    reg [3:0] M, m;

    always @(posedge clk) begin 
        if (rst) begin
            reg_busy <= 1'b0;
            reg_o <= 9'd0;
            reg_calc_cnt <= 3'd0;
            {M, Q} <= {4'd0, 4'd0};
        end
        
        case (op)
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
        
        assign busy = reg_busy;
        assign o = reg_o[7:0];
    end

endmodule