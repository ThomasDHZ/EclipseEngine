#pragma once
#include "WindowsIncludeStuff.h"
#include <string>
class File
{
public:
	static std::string OpenFile(const std::string& reletivePath);
	static std::string RemoveFileExtenstion(const std::string& filename);
	static std::string GetFileNameFromPath(const std::string& filename);
};

