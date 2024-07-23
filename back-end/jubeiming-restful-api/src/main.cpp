// 主函数
// #define TEST_PROGRAM

#ifndef TEST_PROGRAM
#include "main.hpp"

boost::cobalt::main co_main(int argc, char** argv)
{
	co_return co_await run();
}
#else

#include "test/main.hpp"
int main(void) {
	return starta_test();
}

#endif