// HavokLibSamples.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <cstdio>
#include <tchar.h>
#include <array>
#include "HavokApi.hpp"
#include "datas/masterprinter.hpp"
#include "datas/fileinfo.hpp"

#define MyAssert(thing) if (thing) printline(#thing, << _T(" :") << __FUNCTION__ <<_T(" at:") <<__LINE__)
#define MyAssertLoop(thing, i) if (thing) printline(#thing, << _T(" :") << __FUNCTION__ <<_T(" at:") <<__LINE__<< _T(" loop:") << i)

static const std::array<const wchar_t *, 8>compiledFlags = 
{
	L"[4001].hkx",
	L"[4011].hkx",
	L"[4101].hkx",
	L"[4111].hkx",
	L"[8001].hkx",
	L"[8011].hkx",
	L"[8101].hkx",
	L"[8111].hkx",
};

static const std::array<const wchar_t *, 9>versions =
{
	L"550/",
	L"660/",
	L"710/",
	L"2010_1/",
	L"2011_1/",
	L"2011_2/",
	L"2012_2/",
	L"2013_1/",
	L"2014_1/",
};

#include "SkeletonTesting.h"

int _tmain(const int argc, const TCHAR *argv[])
{
	printer.AddPrinterFunction(wprintf);

	TFileInfo info(argv[0]);

	std::wstring testingPath = info.GetPath() + L"rc/";

	TestAllosaurSkeleton(testingPath);
	
	return 0;
}
