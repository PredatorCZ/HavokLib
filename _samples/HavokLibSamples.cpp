// HavokLibSamples.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <cstdio>
#include "HavokApi.hpp"
#include "datas/masterprinter.hpp"
#include "datas/fileinfo.hpp"
#include "hkInternalInterfaces.h"

#if _MSC_VER
#include <tchar.h>
#else
#define _tmain main
#define _TCHAR char
#endif

#define MyAssert(thing) if (thing) printerror(#thing, << _T(" :") << __FUNCTION__ <<_T(" at:") <<__LINE__)
#define MyAssertLoop(thing, i) if (thing) printerror(#thing, << _T(" :") << __FUNCTION__ <<_T(" at:") <<__LINE__<< _T(" loop:") << i)

static const TCHAR *compiledFlags[] =
{
	_T("[4001].hkx"),
	_T("[4011].hkx"),
	_T("[4101].hkx"),
	_T("[4111].hkx"),
	_T("[8001].hkx"),
	_T("[8011].hkx"),
	_T("[8101].hkx"),
	_T("[8111].hkx"),
};

static const TCHAR *versions[] =
{
	_T("550/"),
	_T("660/"),
	_T("710/"),
	_T("2010_1/"),
	_T("2011_1/"),
	_T("2011_2/"),
	_T("2012_2/"),
	_T("2013_1/"),
	_T("2014_1/"),
};

static const TCHAR *versions_delta[] =
{
	_T("550/"),
	_T("660/"),
	_T("710/"),
	_T("2010_1/"),
};

#include "SkeletonTesting.h"
#include "InterleavedTesting.h"

#ifdef _MSC_VER
// Use Visual C++'s memory checking functionality
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif // _MSC_VER

int _tmain(const int argc, const TCHAR *argv[])
{
#ifdef _MSC_VER
	//_crtBreakAlloc = 121467;
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif // _MSC_VER


#ifdef UNICODE
	printer.AddPrinterFunction(wprintf);
#else
	printer.AddPrinterFunction(reinterpret_cast<void*>(printf));
#endif


	TFileInfo info(argv[0]);

	TSTRING testingPath = info.GetPath() + _T("rc/");

	TestAllosaurSkeleton(testingPath);
	TestAllosaurInterleaved(testingPath);
	TestAllosaurDelta(testingPath);
	TestAllosaurSpline1(testingPath);
	TestAllosaurSpline2(testingPath);
	TestAllosaurSpline3(testingPath);
	return 0;
}
