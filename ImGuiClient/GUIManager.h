#pragma once

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

class GUIManager
{
public:
	static GUIManager* GetInstance()
	{
		static GUIManager Instance;
		return &Instance;
	}

	void Start();
	void Loop();
	void End();
private:
	GUIManager(){}
	~GUIManager(){}
	
private:
};