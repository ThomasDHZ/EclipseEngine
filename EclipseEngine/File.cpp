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

std::string File::RemoveFileExtenstion(const std::string& filename)
{
	std::string FileName = filename.substr(0, filename.find_last_of("."));
	return FileName;
}

std::string File::GetFileNameFromPath(const std::string& filename)
{
	std::string FileName = filename.substr(0, filename.find_last_of("."));
	FileName = FileName.substr(FileName.find_first_of("/") + 1, FileName.size());
	return FileName;
}
