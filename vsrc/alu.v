module alu (
    input rst,
    input clk,

    // 1000 0100 0010 0001 0000
    //  +    -    *    /   STOP
    input  [3:0] op,
    input  [3:0] data1,
    input  [3:0] data2,
    output reg [7:0] o,
    output reg       busy
);
    reg reg_busy;
    reg [8:0] reg_o;
    reg [3:0] reg_op;
    
    reg [2:0] reg_calc_cnt;
    reg [3:0] M, M_comp;
    reg [1:0] reg_step;

    always @(posedge clk) begin 
        if (rst) begin
            reg_busy <= 1'b0;
            reg_o <= 9'd0;
            reg_op <= 4'd0;
            reg_calc_cnt <= 3'd0;
            reg_step <= 2'd0;
            {M, M_comp} <= {4'd0, 4'd0};
        end
        
        case (op)
            4'b1000: begin
                reg_o[4:0] <= data1 + data2;
            end 
            4'b0100: begin
                reg_o[4:0] <= data1 - data2;
            end
            4'b0010: begin
                case (reg_step)
                    2'd0: begin
                        {M, M_comp} <= {data1, ~data1 + 1'b1};
                        reg_o <= {4'b0, data2, 1'b0};
                        reg_step <= reg_step + 1'b1;
                        reg_calc_cnt <=  3'd0;
                        reg_busy <= 1'b1;
                    end 
                    2'd1: begin
                        if (reg_calc_cnt == 3'd4) begin
                            reg_busy <= 1'b0;
                            reg_o <= {1'b0, reg_o[8:1]};
                            reg_step <= reg_step + 2'd2;
                        end
                        else if(reg_o[1:0] == 2'b01) begin
                            reg_o <= {reg_o[8:5] + M_comp, reg_o[4:0]};
                            reg_step <= reg_step + 1'd1;
                        end
                        else if(reg_o[1:0] == 2'b10) begin
                            reg_o <= {reg_o[8:5] + M, reg_o[4:0]};
                            reg_step <= reg_step + 1'd1;
                        end
                        else begin
                            reg_step <= reg_step + 1'd1;
                        end
                        
                    end
                    2'd2: begin
                        reg_o <= {reg_o[8], reg_o[8:1]};
                        reg_calc_cnt <= reg_calc_cnt + 1'd1;
                        reg_step <= reg_step - 1'd1;
                    end
                    2'd3: begin
                        reg_step <= 2'd0;
                    end 
                endcase
            end

            4'b0001: begin
                
            end

            default: begin
            
            end 
        endcase
        
    end

    assign busy = reg_busy;
    assign o = reg_o[7:0];
endmodule