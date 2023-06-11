module alu (
    input rst,
    input clk,

    // 1000 0100 0010 0001 0000
    //  +    -    *    /   STOP
    input        sign,
    input  [3:0] op,
    input  [3:0] data1,
    input  [3:0] data2,
    output reg [7:0] o, // 前4位存余数, 后4位存商
    output reg       busy
);
    reg reg_busy;
    reg [9:0] reg_o;
    
    reg [2:0] reg_calc_cnt;
    reg [4:0] M, M_comp;
    reg [1:0] reg_state;

    reg [7:0] reg_data1_ext;
    reg [7:0] reg_data2_ext;


    reg cin_r;
    reg [3 : 0] G;
    reg [3 : 0] P;

    reg [4: 0] sum_tmp;
    reg [3 : 0] cout_tmp;

    always @(posedge clk) begin 
        if (rst) begin
            reg_busy <= 1'b0;
            reg_o <= 10'd0;
            reg_calc_cnt <= 3'd0;
            reg_state <= 2'd0;
            {M, M_comp} <= {5'd0, 5'd0};

            cin_r <= 1'b0;
        end
        
        case (op)
            4'b1000: begin
                cin_r <= 1'b0;

                G[0] <=  data1[0] & data2[0];        //第0bit加法
                P[0] <=  data1[0] | data2[0];
                sum_tmp[0] <= data1[0] ^ data2[0] ^ cin_r;
                cout_tmp[0] <=  G[0] | P[0] & cin_r;

                G[1] <=  data1[1] & data2[1];        //第1bit加法
                P[1] <=  data1[1] | data2[1];
                sum_tmp[1] <= data1[1] ^ data2[1] ^ cout_tmp[0];
                cout_tmp[1] <=  G[1] | P[1] & cout_tmp[0];

                G[2] <=  data1[2] & data2[2];        //第2bit加法
                P[2] <=  data1[2] | data2[2];
                sum_tmp[2] <= data1[2] ^ data2[2] ^ cout_tmp[1];
                cout_tmp[2] <=  G[2] | P[2] & cout_tmp[1];

                G[3] <=  data1[3] & data2[3];        //第3bit加法
                P[3] <=  data1[3] | data2[3];
                sum_tmp[3] <= data1[3] ^ data2[3] ^ cout_tmp[2];
                cout_tmp[3] <=  G[3] | P[3] & cout_tmp[2];

                sum_tmp[4] <= data1[3] ^ data2[3] ^ cout_tmp[3];

                reg_o[4:0] <= sum_tmp;
            end 
            4'b0100: begin
                reg_o[4:0] <= data1 - data2;
            end
            /*
             *   The above-mentioned technique is inadequate when the multiplicand
             *   is the most negative number that can be represented 
             *   (e.g. if the multiplicand has 4 bits then this value is −8). 
             *   This is because then an overflow occurs when computing -m, 
             *   the negation of the multiplicand, which is needed in order to set S.
             *
             *   ref: https://en.wikipedia.org/wiki/Booth%27s_multiplication_algorithm
            */
            4'b0010: begin
                case (reg_state)
                    2'd0: begin
                        {M, M_comp} <= {{data1[3], data1}, ~{data1[3], data1} + 1'b1};
                        reg_o <= {5'd0, data2, 1'b0};
                        reg_state <= reg_state + 1'b1;
                        reg_calc_cnt <=  3'd0;
                        reg_busy <= 1'b1;
                    end 
                    2'd1: begin
                        if (reg_calc_cnt == 3'd4) begin
                            reg_busy <= 1'b0;
                            reg_o <= {reg_o[9], reg_o[9:1]};
                            reg_state <= reg_state - 1'd1;
                        end
                        else if(reg_o[1:0] == 2'b01) begin
                            reg_o <= {reg_o[9:5] + M, reg_o[4:0]};
                            reg_state <= reg_state + 1'd1;
                        end
                        else if(reg_o[1:0] == 2'b10) begin
                            reg_o <= {reg_o[9:5] + M_comp, reg_o[4:0]};
                            reg_state <= reg_state + 1'd1;
                        end
                        else begin
                            reg_state <= reg_state + 1'd1;
                        end
                        
                    end
                    2'd2: begin
                        reg_o <= {reg_o[9], reg_o[9:1]};
                        reg_calc_cnt <= reg_calc_cnt + 1'd1;
                        reg_state <= reg_state - 1'd1;
                    end

                    // ?
                    2'd3: begin
                        reg_state <= 2'd0;
                    end 
                endcase
            end

            4'b0001: begin
                // 0000 xxxx
                // xxxx 0000
                case(reg_state)
                    2'd0: begin
                        reg_o <= 10'd0;
                        if (sign && data1[3]) begin
                            reg_data1_ext <= {4'd0, ~data1 + 1'b1};
                        end
                        else begin
                            reg_data1_ext <= {4'd0, data1};
                        end
                        if (sign && data2[3]) begin
                            reg_data2_ext <= {~data2 + 1'b1, 4'd0};
                        end
                        else begin
                            reg_data2_ext <= {data2, 4'd0};
                        end
                        
                        reg_busy <= 1'b1;
                        reg_calc_cnt <= 3'd0;
                        
                        if (data2 == 0) begin
                            reg_state <= reg_state + 2'd3;
                        end else begin
                            reg_state <= reg_state + 1'b1;
                        end
                    end
                    2'd1: begin
                        if (reg_calc_cnt == 3'd4) begin
                            if (sign && (data1[3] ^ data2[3])) begin
                                reg_o[7:4] <=  ~reg_o[3:0] + 1'b1;
                            end else begin
                                reg_o[7:4] <=  reg_o[3:0];
                            end
                            if (sign && (data1[3] ^ reg_data1_ext[7])) begin
                                reg_o[3:0] <= ~reg_data1_ext[7:4]+1'b1;
                            end else begin
                                reg_o[3:0] <= reg_data1_ext[7:4];
                            end
                            reg_o[9:8] <= 2'd0;
                            reg_busy <= 1'b0;
                            reg_calc_cnt <= 3'd0;

                            reg_state <= 2'd0;
                        end
                        else begin
                            reg_data1_ext <= {reg_data1_ext[6:0], 1'b0};
                            reg_o <= {reg_o[8:0], 1'b0};

                            reg_state <= reg_state + 1'b1;
                        end
                    end
                    2'd2: begin
                        
                        if (reg_data1_ext >= reg_data2_ext) begin
                            reg_data1_ext <= reg_data1_ext - reg_data2_ext;
                            reg_o[0] <= 1'b1;
                            reg_calc_cnt <= reg_calc_cnt + 1'b1;

                            reg_state <= reg_state - 1'b1;
                        end
                        else begin
                            reg_data1_ext <= reg_data1_ext;
                            reg_o[0] <= 1'b0;
                            reg_calc_cnt <= reg_calc_cnt + 1'b1;

                            reg_state <= reg_state - 1'b1;
                        end
                    end
                    
                    2'd3: begin
                        reg_o <= 10'd0;
                        reg_busy <= 1'b0;
                        reg_calc_cnt <= 3'd0;
                        reg_state <= 2'd0;
                    end

                endcase


            end
            default: begin
            
            end 
        endcase
        
    end

    assign busy = reg_busy;
    assign o = reg_o[7:0];
endmodule