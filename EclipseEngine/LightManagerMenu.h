#pragma once
#include <vector>
#include <string>
#include "GLTFSceneManager.h"

class LightManagerMenu
{
private:
	std::vector<std::string> lightcharstring;
	std::vector<const char*> lightListchar;
	int lightSelection = -1;
public:
	LightManagerMenu();
	~LightManagerMenu();

	void Update();
	void ImGuiUpdate();
};

