#include <cstddef>
#include <cstdint>
#include <iostream>
#include "Vtop.h"
#include "verilated.h"
#include "VcdWriter.h"
#include "testFactory.h"
#include <memory>
#include <random>
#include <bitset>

using std::bitset;


#define MAX_TEST 1000000

// clang-format off
#define print_err(fmt, args...) printf("\033[31;1m" fmt "\033[0m\n", ##args)
#define print_info(fmt, args...) printf("\033[33;1m" fmt "\033[0m\n", ##args)
#define debug(fmt, args...) printf("\033[32;1m" "[DEBUG] %s:%d:%s" "\033[0m  "  fmt "\n", __FILE__, __LINE__, __func__, ##args)
// clang-format on


#define U8H(x) (int8_t)((int8_t)((x) >> 4) & 0x0F)
#define U8L(x) (int8_t)((int8_t)(x)&0x0F)
#define BIT(x, n) bitset<(n)>((x)).to_string().c_str()
#define SET_4BIT(x) ((x)&0x0F)


template <typename T>
bool emulateMul(T *top, std::unique_ptr<testFactory>, VcdWriter<T> &vcdWriter);

template <typename T>
bool emulateDiv(T *top, std::unique_ptr<testFactory>, VcdWriter<T> &vcdWriter);

template <typename T>
bool emulateRandom(T *top, std::unique_ptr<testFactory> tf, VcdWriter<T> &vcdWriter);

int main(int argc, char **argv) {
    srand(time(0));

    VerilatedContext *contextp = new VerilatedContext;
    contextp->commandArgs(argc, argv);
    Vtop *top = new Vtop{contextp};

    VcdWriter vcdWriter("top.vcd", contextp, top, 99);

    int64_t count = 0;

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

        auto check = emulateRandom(top, std::make_unique<testFactory>(), vcdWriter);

        if (check) {
            count++;
            if (count % 1000 == 0) {
                std::cout << "PASS TEST: " << count << std::endl;
            }
        } else {
            break;
        }
    }

    delete top;
    delete contextp;
    return 0;
}

template <typename T>
bool emulateRandom(T *top, std::unique_ptr<testFactory> tf, VcdWriter<T> &vcdWriter) {
    auto [op, opCode, op1, op2, res1, res2] = tf->genRandomTest(false);

    bool clk = 0;

    // debug("RESET: top->a = %d top->b = %d top->op = %d top->o %d(0x%s) top->clk %d", top->a, top->b, top->op, (int8_t)top->o, BIT(top->o, 8), top->clk);
    // prepare data
    top->a   = op1;
    top->b   = op2;
    top->op  = opCode;
    top->rst = 0;

    top->clk = clk;
    vcdWriter.tick();

    clk      = !clk;
    top->clk = clk;
    vcdWriter.tick();

    // debug("INIT: top->a = %d(0x%s) top->b = %d(0x%s) top->busy = %d top->o %d(0x%s) top->op %d", (int8_t)top->a, BIT(top->a, 8), (int8_t)top->b, BIT(top->b, 8), top->busy, (int8_t)top->o, BIT(top->o, 8), top->op);
    while (top->busy) {
        clk      = !clk;
        top->clk = clk;
        vcdWriter.tick();
        // debug("INSIDE: top->o = %d(0x%s)  top->clk %d", (int8_t)top->o, BIT(top->o, 8), top->clk);
    }
#define ANS (int8_t)((res1 << 4) | res2)
    if (opCode >= 4 && SET_4BIT(top->o) == res2) {
        return true;
    } else if (opCode > 0 && opCode <= 2 && (int8_t)top->o == ANS) {
        return true;
    } else {
        print_err("======== Wrong ========");
        print_info("Operator: %s", op.c_str());
        printf("Input a:\t\t %3d(0x%s) \n", op1, BIT(op1, 4));
        printf("Input b:\t\t %3d(0x%s) \n", op2, BIT(op2, 4));
        printf("Actually Answer:\t  TOT = %3d(0x%s)  High = %3d(0x%s)  LOW = %3d(0x%s)\n", (int8_t)top->o, BIT(top->o, 8), U8H(top->o), BIT(U8H(top->o), 4), U8L(top->o), BIT(U8L(top->o), 4));
        printf("Expected Answer:\t  TOT = %3d(0x%s)  High = %3d(0x%s)  LOW = %3d(0x%s)\n", ANS, BIT(ANS, 8), res1, BIT(SET_4BIT(res1), 4), res2, BIT(SET_4BIT(res2), 4));
        return false;
    }
#undef ANS
}


template <typename T>
bool emulateDiv(T *top, std::unique_ptr<testFactory> tf, VcdWriter<T> &vcdWriter) {
    auto [op, opCode, op1, op2, res1, res2] = tf->genDivTest();

    bool clk = 0;

    // debug("RESET: top->a = %d top->b = %d top->busy = %d top->o %d(0x%s) top->clk %d", top->a, top->b, top->busy, (int8_t)top->o, BIT(top->o, 8), top->clk);
    // prepare data
    top->a   = op1;
    top->b   = op2;
    top->op  = opCode;
    top->rst = 0;

    top->clk = clk;
    vcdWriter.tick();

    clk      = !clk;
    top->clk = clk;
    vcdWriter.tick();

    // debug("INIT: top->a = %d(0x%s) top->b = %d(0x%s) top->busy = %d top->o %d(0x%s) top->clk %d", (int8_t)top->a, BIT(top->a, 8), (int8_t)top->b, BIT(top->b, 8), top->busy, (int8_t)top->o, BIT(top->o, 8), top->clk);
    while (top->busy) {
        clk      = !clk;
        top->clk = clk;
        vcdWriter.tick();
        // debug("INSIDE: top->o = %d(0x%s)  top->clk %d", (int8_t)top->o, BIT(top->o, 8), top->clk);
    }
#define ANS (int8_t)((res1 << 4) | res2)
    if ((int8_t)top->o == ANS) {
        return true;
    } else {
        print_err("======== Wrong ========");
        print_info("Operator: MUL");
        printf("Input a:\t\t %3d(0x%s) \n", op1, BIT(op1, 4));
        printf("Input b:\t\t %3d(0x%s) \n", op2, BIT(op2, 4));
        printf("Actually Answer:\t quotient = %3d(0x%s)  remainder = %3d(0x%s)\n", U8H(top->o), BIT(U8H(top->o), 4), U8L(top->o), BIT(U8L(top->o), 4));
        printf("Expected Answer:\t quotient = %3d(0x%s)  remainder = %3d(0x%s)\n", res1, BIT(SET_4BIT(res1), 4), res2, BIT(SET_4BIT(res2), 4));
        return false;
    }
#undef ANS
}


template <typename T>
bool emulateMul(T *top, std::unique_ptr<testFactory> tf, VcdWriter<T> &vcdWriter) {
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

    // debug("INIT: top->a = %d(0x%s) top->b = %d(0x%s) top->busy = %d top->o %d(0x%s) top->clk %d", (int8_t)top->a, BIT(top->a, 8), (int8_t)top->b, BIT(top->b, 8), top->busy, (int8_t)top->o, BIT(top->o, 8), top->clk);
    while (top->busy) {
        clk      = !clk;
        top->clk = clk;
        vcdWriter.tick();
        // debug("INSIDE: top->o = %d(0x%s)  top->clk %d", (int8_t)top->o, BIT(top->o, 8), top->clk);
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