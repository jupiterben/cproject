#include "./jsonexp.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include "kernel/export.h"

JSONExpClassDesc * JSONExpClassDesc::getInstance()
{
	static JSONExpClassDesc desc;
	return &desc;
}

int JsonExport::DoExport(const TCHAR *name, ExpInterface *ei, Interface *i, BOOL suppressPrompts /*= FALSE*/, DWORD options /*= 0*/)
{
	// Export	file
	auto json = ExportKernel::export2JSON(i);
	std::ofstream o(name);
	o << std::setw(4) << json << std::endl;
	return 1;
}

BOOL JsonExport::SupportsOptions(int ext, DWORD options)
{
	return TRUE;
}
