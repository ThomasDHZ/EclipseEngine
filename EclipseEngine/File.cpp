#include "File.h"
#include <algorithm>

std::string File::OpenFile(const std::string& reletivePath)
{
	char resul2t[MAX_PATH];
	GetModuleFileName(NULL, resul2t, MAX_PATH);
	std::string baseFilePath = resul2t;
	baseFilePath += "/../../../";
	std::replace(baseFilePath.begin(), baseFilePath.end(), '\\', '/');

	return baseFilePath + reletivePath;
}