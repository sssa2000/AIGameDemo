
#include "stdafx.h"
#include "HippoAppBase.h"
#include "HippoDxAppBase.h"
#include "ErrReport.h"
#include "HippoD3d9Device.h"
#include "HippoD3DxModel.h"
#include "IGameWorld.h"
#include "IGameWordContex.h"
#include "TankGameRender.h"
#include "TankGameWorldImpl.h"
#include "EntityFactory.h"
#include <iostream>
#include <memory>
#include <Windows.h>
#include <functional>
#include <vector>
#include "Globals.h"
#include "FPSCamera.h"
#include "FollowCamera.h"
#include "ModelViewCamera.h"
#include "GameKeyBoardControl.h"

struct appInfo
{
	appInfo()
	{
		aimode = false;
		dllfilename = "";
	}
	bool aimode;
	std::string dllfilename;
};

appInfo ginfo;

//鍛戒护琛岋細-ai xxxx.dll
void ParseCommandLine(int argc, char* argv[])
{
	if (argc>2)
	{
		if (strcmp(argv[1], "-ai") == 0)
		{
			ginfo.aimode = true;
			ginfo.dllfilename = argv[2];
			return;
		}
	}
}


//////////////////////////////////////////////////////////////////////////
class TankGameExe :public HippoDxAppBase
{
public:

	TankGameExe(const char* name, unsigned int w, unsigned int h, unsigned int left, unsigned int top) :
		HippoDxAppBase(name, w, h, left, top)
	{

	}

	void SwitchGameMode(bool bAiMode)
	{

	}
	virtual void OnLostDevice()
	{

	}

	virtual void OnResetDevice()
	{

	}

	void InitApp()
	{
		Globals::Init(GetDevice(), ginfo.dllfilename.c_str(), m_pWnd->GetWidth(), m_pWnd->GetHeight());
		Globals::GetWorld()->InitWorld();

		Globals::SwitchCameraAndControl(ginfo.aimode);
	


		m_pWnd->RegisterMouseRightDownEvent(Globals::GetFpsCamera()->GetMouseRightDownCallback());
		m_pWnd->RegisterMouseRightUpEvent(Globals::GetFpsCamera()->GetMouseRightUpCallback());
		m_pWnd->RegisterMouseMoveEvent(Globals::GetFpsCamera()->GetMouseMoveCallback());
		m_pWnd->RegisterKeyDownEvent(Globals::GetFpsCamera()->GetKeyDownCallback());
		
		m_pWnd->RegisterMouseRightDownEvent(Globals::GetRTSCamera()->GetMouseRightDownCallback());
		m_pWnd->RegisterMouseRightUpEvent(Globals::GetRTSCamera()->GetMouseRightUpCallback());
		m_pWnd->RegisterMouseMoveEvent(Globals::GetRTSCamera()->GetMouseMoveCallback());
		m_pWnd->RegisterKeyDownEvent(Globals::GetRTSCamera()->GetKeyDownCallback());

		GameKeyBoardControl* contorl=Globals::GetKeyBoardControl();
		m_pWnd->RegisterKeyDownEvent(contorl->GetKeyDownCallback());


		m_pDevice->GetDeviceD3D9()->SetTransform(D3DTS_PROJECTION, Globals::GetRender()->GetProjMatrix());
		


	}

	void ShutdownApp()
	{
		Globals::CleanUp();
	}


	void Update(unsigned int escapeMs)
	{
		IGameWordContex* pworld=Globals::GetWorld();
		Globals::GetCurrentControl()->OnTick((IGameWorld*)pworld,escapeMs);
		Globals::GetCurrentCamera()->FrameUpdate(escapeMs);
		Globals::GetCurrentCamera()->LookAt(m_pDevice.get()->GetDeviceD3D9());
		//更新逻辑世界
		unsigned int logic_interval=1000 / 60;
		pworld->Update(logic_interval);

	}

	void Render(unsigned int escapeMs)
	{
		m_pDevice->GetDeviceD3D9()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
			D3DCOLOR_XRGB(0, 255, 255), 1.0f, 0);

		m_pDevice->BeginRender();
		Globals::GetRender()->Render((IGameWordContex*)Globals::GetWorld(),1000/60);
		m_pDevice->EndRender();
	}

};


//命令行：-ai xxxx.dll
int _tmain(int argc, _TCHAR* argv[])
{
	ParseCommandLine(argc, argv);

	std::tr1::shared_ptr<TankGameExe> pApp = std::tr1::make_shared<TankGameExe>("TankGameExe", 1024, 768, 0, 0);
	pApp->Init();
	pApp->StartRun();
	pApp->Shutdown();
	return 1;
}
