// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

#include "IGameControl.h"
#include "AIControlImplExample.h"

AIControlImplExample* gControl=0;
IGameControl* CreateAIControl()
{
	if (!gControl)
		gControl = new AIControlImplExample;
	return (IGameControl*)(gControl);
}

void DeleteAIControl()
{
	if (gControl)
		delete gControl;
	gControl = 0;
}