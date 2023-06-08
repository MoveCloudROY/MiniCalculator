#include "testFactory.h"

testFactory::testFactory(/* args */) {
}

testFactory::~testFactory() {
}

// 生成加法测试
std::vector<int8_t> testFactory::genAddTest() {
    std::vector<int8_t> test;
    // 随机生成两个操作数a和b, 范围-8到7
    int8_t a   = rand() % 16 - 8;
    int8_t b   = rand() % 16 - 8;
    // 计算正确的结果, 高4位恒为0
    int8_t sum = (a + b) & 0x0f; // 忽略溢出
    // int8_t sum = (a + b); // 忽略溢出
    test.push_back(a);
    test.push_back(b);
    test.push_back(sum);
    return test;
}

std::vector<int8_t> testFactory::genAddTest(int a, int b) {
    std::vector<int8_t> test;
    // 计算正确的结果, 高4位恒为0
    int8_t sum = (a + b) & 0x0f; // 忽略溢出
    // int8_t sum = (a + b); // 忽略溢出
    test.push_back(a);
    test.push_back(b);
    test.push_back(sum);
    return test;
}

// 生成减法测试
std::vector<int8_t> testFactory::genSubTest() {
    std::vector<int8_t> test;
    // 随机生成两个操作数a和b, 范围-8到7
    int8_t a    = rand() % 16 - 8;
    int8_t b    = rand() % 16 - 8;
    // 计算正确的结果
    int8_t diff = a - b; // 忽略溢出
    test.push_back(a);
    test.push_back(b);
    test.push_back(diff);
    return test;
}

// 生成乘法测试
std::vector<int8_t> testFactory::genMulTest() {
    std::vector<int8_t> test;
    // 随机生成两个操作数a和b, 范围-8到7
    int8_t a    = rand() % 16 - 8;
    int8_t b    = rand() % 16 - 8;
    // 计算正确的结果
    int8_t prod = a * b; // 忽略溢出
    test.push_back(a);
    test.push_back(b);
    test.push_back(prod);
    return test;
}

// 生成除法测试
testCase testFactory::genDivTest() {
    int8_t a, b, sum, diff, prod_h, prod_l, quot, rem;

    // 随机选择8, 4, 2, 1, 0中的一个作为操作码
    uint8_t opCode = 1 & 0x0f;
    do {
        // 随机生成两个操作数a和b, 范围-8到7
        a      = rand() % 16 - 8;
        b      = rand() % 16 - 8;
        sum    = (a + b) & 0x0f;
        diff   = a - b;
        prod_h = (a * b) >> 4; // 乘法高位
        prod_l = a * b;        // 乘法地位
        quot   = a / b;
        rem    = a % b;
    } while (opCode == 0x01 && b == 0); // 除法时除数不能为0

    std::vector<int8_t> params = {a, b, sum, diff, prod_l, prod_h, quot, rem};

    return testCase(opCode, params);
}

// 生成随机测试, nop表示是否包含nop测试
testCase testFactory::genRandomTest(bool nop) {
    int8_t a, b, sum, diff, prod_h, prod_l, quot, rem;

    // 随机选择8, 4, 2, 1, 0中的一个作为操作码
    uint8_t opCode = (1 << (rand() % (nop ? 5 : 4))) & 0x0f;
    do {
        // 随机生成两个操作数a和b, 范围-8到7
        a      = rand() % 16 - 8;
        b      = rand() % 16 - 8;
        sum    = (a + b) & 0x0f;
        diff   = a - b;
        prod_h = (a * b) >> 4; // 乘法高位
        prod_l = a * b;        // 乘法地位
        quot   = a / b;
        rem    = a % b;
    } while (opCode == 0x01 && b == 0); // 除法时除数不能为0

    std::vector<int8_t> params = {a, b, sum, diff, prod_l, prod_h, quot, rem};

    return testCase(opCode, params);
}