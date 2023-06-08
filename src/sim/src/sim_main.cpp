#include <cstddef>
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
#define SET_4BIT(x) ((x)&0x0F)


template <typename T>
bool emulateMul(T *top, testFactory *tf, VcdWriter<T> &vcdWriter);


int main(int argc, char **argv) {
    srand(time(0));

    VerilatedContext *contextp = new VerilatedContext;
    contextp->commandArgs(argc, argv);
    Vtop *top = new Vtop{contextp};

    VcdWriter vcdWriter("bin/top.vcd", contextp, top, 99);

    testFactory *tf    = new testFactory();
    int64_t      count = 0;
<<<<<<< HEAD
=======
    int8_t       expected = 0;
    bool         clk   = 0;
>>>>>>> 8816057d3e3a5a191fca5111eabc68a5beb0d423

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

    while (!contextp->gotFinish() && count < MAX_TEST) {

<<<<<<< HEAD
        auto check = emulateMul(top, tf, vcdWriter);

        if (check) {
=======
        testCase           tc   = tf->genDivTest();
        clk                      = 0;

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
        // debug("RESET: top->a = %d top->b = %d top->busy = %d top->o %d(0x%s) top->clk %d", top->a, top->b, top->busy, (int8_t)top->o, BIT(top->o, 8), top->clk);
        // prepare data
        top->a   = SET_4BIT(tc.op1);
        top->b   = SET_4BIT(tc.op2);
        top->op  = tc.opCode;
        top->rst = 0;

        top->clk = clk;
        vcdWriter.tick();

        clk      = !clk;
        top->clk = clk;
        vcdWriter.tick();

        // debug("INIT: top->a = %d top->b = %d top->busy = %d top->o %d(0x%s) top->clk %d", top->a, top->b, top->busy, (int8_t)top->o, BIT(top->o, 8), top->clk);
        while (top->busy) {
            clk      = !clk;
            top->clk = clk;
            vcdWriter.tick();
            // debug("INSIDE: top->o = %d(0x%s)  top->clk %d", (int8_t)top->o, BIT(top->o, 8), top->clk);
        }
        expected = (SET_4BIT(tc.res2) << 4) | SET_4BIT(tc.res1);
        if ((int8_t)top->o == expected) {
>>>>>>> 8816057d3e3a5a191fca5111eabc68a5beb0d423
            count++;
            if (count % 1000 == 0) {
                std::cout << "PASS TEST: " << count << std::endl;
            }
        } else {
<<<<<<< HEAD
            break;
=======
            print_err("======== WRONG ========");
            print_info("Operator: %s", tc.op.c_str());
            printf("Input a:\t\t %3d(0x%s) \n", tc.op1, BIT(tc.op1, 4));
            printf("Input b:\t\t %3d(0x%s) \n", tc.op2, BIT(tc.op2, 4));
            printf("Actually Answer:\t %3d(0x%s) \n", (int8_t)top->o, BIT(top->o, 8));
            printf("Expected Answer:\t %3d(0x%s) \n", expected, BIT(expected, 8));
            exit(0);
>>>>>>> 8816057d3e3a5a191fca5111eabc68a5beb0d423
        }
    }

    delete top;
    delete contextp;
    return 0;
}

template <typename T>
bool emulateMul(T *top, testFactory *tf, VcdWriter<T> &vcdWriter) {
    std::vector<int8_t> test = tf->genMulTest();
    bool                clk  = 0;

    // debug("RESET: top->a = %d top->b = %d top->busy = %d top->o %d(0x%s) top->clk %d", top->a, top->b, top->busy, (int8_t)top->o, BIT(top->o, 8), top->clk);
    // prepare data
    top->a   = SET_4BIT(test[0]);
    top->b   = SET_4BIT(test[1]);
    top->op  = 2;
    top->rst = 0;

    top->clk = clk;
    vcdWriter.tick();

    clk      = !clk;
    top->clk = clk;
    vcdWriter.tick();

    debug("INIT: top->a = %d top->b = %d top->busy = %d top->o %d(0x%s) top->clk %d", top->a, top->b, top->busy, (int8_t)top->o, BIT(top->o, 8), top->clk);
    while (top->busy) {
        clk      = !clk;
        top->clk = clk;
        vcdWriter.tick();
        debug("INSIDE: top->o = %d(0x%s)  top->clk %d", (int8_t)top->o, BIT(top->o, 8), top->clk);
    }

    if ((int8_t)top->o == (int8_t)test[2]) {
        return true;
    } else {
        print_err("======== Wrong ========");
        print_info("Operator: MUL");
        printf("Input a:\t\t %3d(0x%s) \n", test[0], BIT(test[0], 4));
        printf("Input b:\t\t %3d(0x%s) \n", test[1], BIT(test[1], 4));
        printf("Actually Answer:\t %3d(0x%s) \n", (int8_t)top->o, BIT(top->o, 8));
        printf("Expected Answer:\t %3d(0x%s) \n", (int8_t)test[2], BIT(test[2], 8));
        return false;
    }
}