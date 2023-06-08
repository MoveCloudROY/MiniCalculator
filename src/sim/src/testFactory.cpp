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

std::vector<int8_t> testFactory::genAddTest(int a, int b)
{
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
std::vector<int8_t> testFactory::genDivTest() {
    std::vector<int8_t> test;
    // 随机生成两个操作数a和b, 范围-8到7
    int8_t a    = rand() % 16 - 8;
    int8_t b    = rand() % 16 - 8;
    // 计算正确的结果
    int8_t quot = a / b; // 忽略溢出
    int8_t rem  = a % b; // 忽略溢出
    test.push_back(a);
    test.push_back(b);
    test.push_back(quot);
    test.push_back(rem);
    return test;
}

// 生成随机测试, nop表示是否包含nop测试
testCase testFactory::genRandomTest(bool nop) {
    // 随机生成两个操作数a和b, 范围-8到7
    int8_t a    = rand() % 16 - 8;
    int8_t b    = rand() % 16 - 8;
    // 计算正确的结果
    int8_t sum  = (a + b) & 0x0f;
    int8_t diff = a - b;
    int8_t prod_h = (a * b)>>4; // 乘法高位
    int8_t prod_l = a * b; // 乘法地位
    int8_t quot = a / b;
    int8_t rem  = a % b;
    // 随机选择8, 4, 2, 1, 0中的一个作为操作码
    uint8_t opCode = (1 << ( rand()% (nop ? 5 : 4) ) ) & 0x0f;
    std::vector<int8_t> params = {a, b, sum, diff, prod_l, prod_h, quot, rem};
    
    return testCase(opCode, params);
    
}