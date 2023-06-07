#include <cstdint>
#include <iostream>
#include <random>
#include <Vtop.h>
#include <verilated.h>

int main(int argc, char **argv) {
    srand(time(0));
    VerilatedContext *contextp = new VerilatedContext;
    contextp->commandArgs(argc, argv);
    Vtop *top = new Vtop{contextp};
    while (!contextp->gotFinish()) {
        int a = (uint8_t)rand(), b = (uint8_t)rand();
        top->a = a;
        top->b = b;
        top->eval();
        if (top->o == (a ^ b)) {
            std::cout << "OK" << std::endl;
        } else {
            std::cout << "Wrong" << std::endl;
        }
    }

    delete top;
    delete contextp;
    return 0;
}