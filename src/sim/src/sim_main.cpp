#include <cstdint>
#include <iostream>
#include "Vtop.h"
#include "verilated.h"
#include "testFactory.h"
#include <random>

#define MAX_TEST 1000000

// 遍历int8_t类型变量每一位并输出
void printInt8(int8_t a) {
    for (int i = 7; i >= 0; i--) {
        std::cout << ((a >> i) & 0x01);
    }
    std::cout << std::endl;
}

int main(int argc, char **argv) {
    srand(time(0));
    VerilatedContext *contextp = new VerilatedContext;
    contextp->commandArgs(argc, argv);
    Vtop *top = new Vtop{contextp};

    testFactory *tf = new testFactory();
    int64_t count = 0;
    while (!contextp->gotFinish() && count < MAX_TEST) {
        std::vector <int8_t> test = tf->genAddTest();
        top->a = test[0];
        top->b = test[1];
        top->eval();
        if (top->o == test[2]) {
            count++;
            if (count % 1000 == 0) {
                std::cout << "PASS TEST: " << count << std::endl;
            }
        } else {
            std::cout << "Wrong: " << std::endl;
            std::cout << "op: ADD" << std::endl;
            std::cout << "a: \t\t" << (int)test[0] << std::endl;
            printInt8(test[0]);
            std::cout << "b: \t\t" << (int)test[1] << std::endl;
            printInt8(test[1]);
            std::cout << "top->o\t\t: " << (int)top->o << std::endl;
            std::cout << "answer: " << (int)test[2] << std::endl;
            printInt8(test[2]);
            exit(-1);
        }
    }

    delete top;
    delete contextp;
    return 0;
}