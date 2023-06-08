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