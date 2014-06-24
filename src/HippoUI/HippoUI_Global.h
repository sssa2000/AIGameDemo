/********************************************************************
	created:	2012/11/02
	created:	2:11:2012   1:04
	filename: 	F:\Git\ClassicHippoTestBox\Common\HippoUI\HippoUI_Global.h
	file path:	F:\Git\ClassicHippoTestBox\Common\HippoUI
	file base:	HippoUI_Global
	file ext:	h
	author:		sssa2000
	
	purpose:	
*********************************************************************/
#pragma once

#include "HippoUI_Include.h"
struct sinitInfo
{
	//device type
	HIPPOUI_DEVICE_TYPE t;
	//device ptr
	void* pDevice;
	//style filename
	std::string stylefilename;
};

class HippoUI_ResManager;
class HippoUI_Render;
class HippoUI_ResFactory;
class HippoUI_Timer;

struct IDirect3DDevice9;

class HippoUI_Global
{
public:
	static void Init(sinitInfo& info);
	static void Destroy();
	static HippoUI_Global* GetIns(){return m_pIns;}
	HippoUI_Global(sinitInfo& info);
	~HippoUI_Global();
	HippoUI_ResManager* GetResManager();
	HippoUI_Render* GetUIRender();
	HippoUI_ResFactory* GetResFactory();
	HippoUI_Timer* GetTimer();

	IDirect3DDevice9* GetDevice9();
	//IDirect3DDevice9* GetDevice9();
private:

	void InitResourceManager(sinitInfo& info);
	void InitResourceFactory(sinitInfo& info);
	void InitUIRender(sinitInfo& info);

	HippoUI_ResManager* m_pResManager;
	HippoUI_Render* m_pUIRender;
	HippoUI_ResFactory* m_pResFactory;
	HippoUI_Timer* m_pTimer;
	static HippoUI_Global* m_pIns;

	void* m_pDevice;
	HIPPOUI_DEVICE_TYPE m_DeviceType;
};

