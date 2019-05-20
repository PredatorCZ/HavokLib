// HavokLibSamples.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <cstdio>
#include <tchar.h>
#include "HavokApi.hpp"
#include "datas/masterprinter.hpp"
#include "datas/fileinfo.hpp"
#include "hkInternalInterfaces.h"

#define MyAssert(thing) if (thing) printline(#thing, << _T(" :") << __FUNCTION__ <<_T(" at:") <<__LINE__)
#define MyAssertLoop(thing, i) if (thing) printline(#thing, << _T(" :") << __FUNCTION__ <<_T(" at:") <<__LINE__<< _T(" loop:") << i)

static const wchar_t *compiledFlags[] =
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

static const wchar_t *versions[] =
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

static const wchar_t *versions_delta[] =
{
	L"550/",
	L"660/",
	L"710/",
	L"2010_1/",
};

#include "SkeletonTesting.h"
#include "InterleavedTesting.h"

int _tmain(const int argc, const TCHAR *argv[])
{
	printer.AddPrinterFunction(wprintf);

	TFileInfo info(argv[0]);

	std::wstring testingPath = info.GetPath() + L"rc/";

	TestAllosaurSkeleton(testingPath);
	TestAllosaurInterleaved(testingPath);
	TestAllosaurDelta(testingPath);
	return 0;
}
