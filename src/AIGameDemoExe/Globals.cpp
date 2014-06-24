#include "Globals.h"
#include "EntityFactory.h"
#include "Timer.h"
#include "IGameControl.h"
#include "GameKeyBoardControl.h"
#include "TankGameWorldImpl.h"
#include "FPSCamera.h"
#include "FollowCamera.h"
#include "ModelViewCamera.h"
#include <windows.h>
#include "ErrReport.h"
#include "HippoD3d9Device.h"
#include "FxEffectManager.h"
#include "TankGameRender.h"
#include "FreeImage.h"
HippoD3d9Device* gDevice = 0;
EntityFactory* gEntityFactory=0;
Timer* timer=0;

IGameControl* gControl=0;
IGameControl* gAIControl = 0;
GameKeyBoardControl* gKeyControl=0;
IGameWordContex* gWorld=0;

ModelViewCamera* gRTSCamera=0;
FPSCamera* gFpsCamera=0;
FollowCamera* gFollowCamera=0;
CameraBase* gCurrentCamera = 0;
FxEffectManager* gFxEffectManager = 0;
TankGameRender* gRender = 0;

HMODULE m_dll;
CreateAIControl_FunPtr gCreateAIPtr = 0;
DeleteAIControl_FunPtr gDeleteAIPtr = 0;

#define SAFE_RELEASE(p) {if(p)p->Release();p=0;}
#define SAFE_DELETE(p) {if(p)delete p;p=0;}

HippoD3d9Device* Globals::GetDevice()
{
	return gDevice;
}

void InitDLL(const char* dllname)
{
	if (!dllname || strlen(dllname) < 3)
			return;
	m_dll = LoadLibraryA(dllname);
	if (!m_dll)
	{
		DWORD err = GetLastError();
		ReportErrWithLastErr(err, "Load Library Failed!:%s", dllname);
		return ;
	}
	gCreateAIPtr = (CreateAIControl_FunPtr)GetProcAddress(m_dll, "CreateAIControl");
	if (!gCreateAIPtr)
	{
		DWORD err = GetLastError();
		ReportErrWithLastErr(err, "GetProcAddress!:CreateAIControl");
		return;
	}

	gDeleteAIPtr = (DeleteAIControl_FunPtr)GetProcAddress(m_dll, "DeleteAIControl");
	if (!gDeleteAIPtr)
	{
		DWORD err = GetLastError();
		ReportErrWithLastErr(err, "GetProcAddress!:DeleteAIControl");
		return;
	}

	gAIControl = gCreateAIPtr();

	FreeImage_Initialise();

}
void Globals::Init(HippoD3d9Device* device, const char* dllname, int w, int h)
{
	gDevice = device;
	gRender = new TankGameRender(gDevice);
	gRender->Init(w,h);
	gFxEffectManager = new FxEffectManager;

	gEntityFactory=new EntityFactory(device);
	timer=new Timer;
	gWorld=new TankGameWorldImpl;
	gKeyControl=new GameKeyBoardControl(gWorld);
	gControl=gKeyControl;

	gFpsCamera = new FPSCamera;
	gFollowCamera = new FollowCamera;
	gRTSCamera=new ModelViewCamera;
	gCurrentCamera = gRTSCamera;

	InitDLL(dllname);
}
void Globals::CleanUp()
{
	SAFE_RELEASE(gEntityFactory);
	SAFE_RELEASE(timer);
	SAFE_RELEASE(gWorld);
	SAFE_RELEASE(gKeyControl);
	SAFE_DELETE(gFpsCamera);
	SAFE_DELETE(gFollowCamera);
	SAFE_DELETE(gRTSCamera);
	gCurrentCamera = 0;
	gAIControl = 0;
	if (gDeleteAIPtr)
		gDeleteAIPtr();
	if (m_dll)
		FreeModule(m_dll);
	SAFE_DELETE(gFxEffectManager);
	SAFE_DELETE(gRender);

	FreeImage_DeInitialise();
}

EntityFactory* Globals::GetEntityFactory()
{
	return gEntityFactory;
}

Timer* Globals::GetTimer()
{
	return timer;
}

IGameControl* Globals::GetCurrentControl()
{
	return gControl;
}

IGameWordContex* Globals::GetWorld()
{
	return gWorld;
}

GameKeyBoardControl* Globals::GetKeyBoardControl()
{
	return gKeyControl;
}
void Globals::SwitchCameraAndControl(bool aimode)
{
	if (aimode)
	{
		gControl = gAIControl;
		gCurrentCamera = (CameraBase*)gFollowCamera;
	}
	else
	{
		gControl = gKeyControl;
		gCurrentCamera =(CameraBase*) gRTSCamera;
	}
}

FPSCamera* Globals::GetFpsCamera()
{
	return gFpsCamera;
}
FollowCamera* Globals::GetFollowCamera()
{
	return gFollowCamera;
}
ModelViewCamera* Globals::GetRTSCamera()
{
	return gRTSCamera;
}
CameraBase* Globals::GetCurrentCamera()
{
	return gCurrentCamera;
}

FxEffectManager* Globals::GetFxManager()
{
	return gFxEffectManager;
}

TankGameRender* Globals::GetRender()
{
	return gRender;
}