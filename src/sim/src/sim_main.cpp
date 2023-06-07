#include <cstdint>
#include <iostream>
#include "Vtop.h"
#include "verilated.h"
#include "testFactory.h"
#include <random>

#define MAX_TEST 1000000

int main(int argc, char **argv) {
    srand(time(0));
    VerilatedContext *contextp = new VerilatedContext;
    contextp->commandArgs(argc, argv);
    Vtop *top = new Vtop{contextp};

    testFactory tf = new testFactory();
    int64_t count = 0;
    while (!contextp->gotFinish() && count < MAX_TEST) {
        std::vector <uint8_t> test = tf.genAddTest();
        top->a = test[0];
        top->b = test[1];
        top->eval();
        if (top->o == test[2]) {
            count++;
            if count % 1000 == 0 {
                std::cout << "PASS TEST: " << count << std::endl;
            }
        } else {
            std::cout << "Wrong: " << std::endl;
            std::cout << "op: ADD" << std::endl;
            std::cout << "a: " << test[0] << std::endl;
            std::cout << "b: " << test[1] << std::endl;
            std::cout << "op: " << top->o << std::endl;
            std::cout << "answer: " << test[1] << std::endl;
            exit(-1);
        }
    }

    delete top;
    delete contextp;
    return 0;
}