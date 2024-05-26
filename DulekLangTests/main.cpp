#include "../Du/Program/Settings.h"
#include<gtest/gtest.h>
#include <iostream>
#include <algorithm>
#ifdef _WIN32
#include <Windows.h>
#else

#endif
void attach()
{
#ifdef _WIN32
	while (!IsDebuggerPresent())
		Sleep(1);
#else

#endif
}

#define ATTACH1
int main(int argc, char** argv)
{
#ifdef ATTACH
	attach();
#endif
	::testing::InitGoogleTest(&argc, argv);
	int ret = RUN_ALL_TESTS();
	std::cin.get();
	return ret;
}

class DulekLangTestClass
{
public:

};
