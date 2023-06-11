module seg7(
    input [15:0] num, // 四位七段数码管显示的数字
    input clk, // 时钟
    input [3:0] sel, // 七段数码管选择
    output [6:0] seg, // 七段数码管阴极
    output [3:0] an // 七段数码管阳极
);

    reg[3:0] seg_sel_t = 4'b0001; // 七段数码管选择, 每个时钟周期左移一位
    reg[6:0] seg_show_t;  // 实际选择的阴极输出

    wire [6:0] show_data[3:0]; // 每一位七段数码管显示的数字

    assign show_data[0] = convert(num[3:0]);
    assign show_data[1] = convert(num[7:4]);
    assign show_data[2] = convert(num[11:8]);
    assign show_data[3] = convert(num[15:12]);

    always @(posedge clk)
    begin
        seg_sel_t <= seg_sel_t << 1'b1;
        if (seg_sel_t == 4'b1000)
            seg_sel_t <= 4'b0001;
    end

    always @(seg_sel_t)
    begin
        case(seg_sel_t)
            4'b0001: seg_show_t = (sel[1]) ? show_data[0] : 7'h111_1111;
            4'b0010: seg_show_t = (sel[2]) ? show_data[1] : 7'h111_1111;
            4'b0100: seg_show_t = (sel[3]) ? show_data[2] : 7'h111_1111;
            default: seg_show_t = (sel[4]) ? show_data[3] : 7'h111_1111;
        endcase
    end

    assign seg = seg_show_t;
    assign an = ~seg_sel_t; // 取反输出(电路相关)


    function[6:0] convert;
		input[3:0] x;
		reg[6:0] conv_seg;
		begin
            case (x)
                            // G F E D C B A
                4'h00: conv_seg= 7'b100_0000;   //40
                4'h01: conv_seg= 7'b111_1001;   //79
                4'h02: conv_seg= 7'b010_0100;   //24
                4'h03: conv_seg= 7'b011_0000;   //30
                4'h04: conv_seg= 7'b001_1001;   //19
                4'h05: conv_seg= 7'b001_0010;   //12
                4'h06: conv_seg= 7'b000_0010;   //02
                4'h07: conv_seg= 7'b111_1000;   //78
                4'h08: conv_seg= 7'b000_0000;   //00
                4'h09: conv_seg= 7'b001_0000;   //10
                4'h0a: conv_seg= 7'b000_1000;   //08
                4'h0b: conv_seg= 7'b000_0011;   //03
                4'h0c: conv_seg= 7'b010_0111;   //27
                4'h0d: conv_seg= 7'b010_0001;   //21
                4'h0e: conv_seg= 7'b000_0110;   //06
                4'h0f: conv_seg= 7'b000_1110;   //0d
                default: conv_seg= 7'b1111111; //ff
            endcase
			convert = conv_seg;
		end

	endfunction
endmodule
