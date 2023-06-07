#include <cstdint>
#include <iostream>
#include "Vtop.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "VcdWriter.h"
#include "testFactory.h"
#include <random>
#include <bitset>

using std::bitset;


#define MAX_TEST 1000000

// clang-format off
#define print_err(fmt, args...) printf("\033[31;1m" fmt "\033[0m\n", ##args)
#define print_info(fmt, args...) printf("\033[33;1m" fmt "\033[0m\n", ##args)
#define debug(fmt, args...) printf("\033[32;1m" "[DEBUG] %s:%d:%s" "\033[0m  "  fmt "\n", __FILE__, __LINE__, __func__, ##args)
// clang-format on


#define BIT(x, n) bitset<(n)>(x).to_string().c_str()
int main(int argc, char **argv) {
    srand(time(0));

    VerilatedContext *contextp = new VerilatedContext;
    contextp->commandArgs(argc, argv);
    Vtop *top = new Vtop{contextp};

    VcdWriter vcdWriter("bin/top.vcd", contextp, top, 99);

    testFactory *tf    = new testFactory();
    int64_t      count = 0;
    bool         clk   = 0;


    while (!contextp->gotFinish() && count < MAX_TEST) {

        std::vector<int8_t> test = tf->genMulTest();

        // reset
        top->a   = 0;
        top->b   = 0;
        top->op  = 0;
        top->clk = 0;
        top->rst = 1;
        vcdWriter.tick();
        top->a   = 0;
        top->b   = 0;
        top->op  = 0;
        top->clk = 1;
        top->rst = 1;
        vcdWriter.tick();
        debug("RESET: top->o = %d top->busy = %d", top->o, top->busy);
        // prepare data
        top->a   = test[0];
        top->b   = test[1];
        top->op  = 2;
        top->rst = 0;

        top->clk = clk;
        vcdWriter.tick();

        clk      = !clk;
        top->clk = clk;
        vcdWriter.tick();

        debug("INIT: top->a = %d top->b = %d top->busy = %d top->rst %d top->clk %d", top->a, top->b, top->busy, top->rst, top->clk);
        while (top->busy) {
            clk      = !clk;
            top->clk = clk;
            vcdWriter.tick();
            debug("INSIDE: top->o = %d(0x%s)  top->clk %d", top->o, BIT(top->o, 8), top->clk);
        }

        if (top->o == test[2]) {
            count++;
            if (count % 1000 == 0) {
                std::cout << "PASS TEST: " << count << std::endl;
            }
        } else {
            print_err("======== Wrong ========");
            print_info("Operator: MUL");
            printf("Input a:\t\t %3d(0x%s) \n", test[0], BIT(test[0], 4));
            // printInt8(test[0]);
            printf("Input b:\t\t %3d(0x%s) \n", test[1], BIT(test[1], 4));
            // printInt8(test[1]);
            printf("Actually Answer:\t %3d(0x%s) \n", top->o, BIT(top->o, 8));
            // printInt8(test[1]);
            printf("Expected Answer:\t %3d(0x%s) \n", test[2], BIT(test[2], 8));
            exit(-1);
        }
    }

    delete top;
    delete contextp;
    return 0;
}