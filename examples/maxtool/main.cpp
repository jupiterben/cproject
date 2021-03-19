#include <iostream>
#include "common.h"

extern void BaseImpl(const std::string &path, const std::string &outPath);
extern void Opt0Impl(const std::string &path, const std::string &outPath);
extern void Opt1Impl(const std::string &path, const std::string &outPath);
extern void Opt2Impl(const std::string &path, const std::string &outPath);
extern void Opt3Impl(const std::string &path, const std::string &outPath);

int main()
{
	std::string testFile = __DIR__ + "test.xml";

	BaseImpl(testFile, __DIR__ + "base\\");
	Opt0Impl(testFile, __DIR__ + "opt0\\");
	//Opt1Impl(testFile, __DIR__ + "opt1\\");
	//Opt2Impl(testFile, __DIR__ + "opt2\\");
	Opt3Impl(testFile, __DIR__ + "opt3\\");
	return 1;
}
