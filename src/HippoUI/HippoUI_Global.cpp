#include "HippoUI_Global.h"
#include "HippoUI_ResManager.h"
#include "HippoUI_ResFactoryD3D9.h"
#include "HippoUI_RenderD3D9.h"
#include "HippoUI_Timer.h"
#include "d3d9.h"

#ifdef _DEBUG
#pragma comment(lib,"d3dx9d.lib")
#pragma comment(lib,"d3d9.lib")
#else
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"d3d9.lib")
#endif


HippoUI_Global* HippoUI_Global::m_pIns=0;
 void HippoUI_Global::Init(sinitInfo& info)
 {
	 if(!m_pIns)
		 m_pIns=new HippoUI_Global(info);
	
 }
 void HippoUI_Global::Destroy()
 {
	 delete m_pIns;
	 m_pIns=0;
 }

 //////////////////////////////////////////////////////////////////////////
 HippoUI_Global::HippoUI_Global(sinitInfo& info)
 {
	 m_pUIRender=0;
	 m_pResManager=0;
	 m_pResFactory=0;
	 m_pTimer=0;
	 m_pDevice=info.pDevice;
	 m_DeviceType=info.t;

	 InitResourceManager(info);
	 InitResourceFactory(info);
	 InitUIRender(info);
	 GetTimer()->Reset();
 }

 HippoUI_Global::~HippoUI_Global()
 {

 }
 void HippoUI_Global::InitResourceManager(sinitInfo& info)
 {
	 m_pResManager=new HippoUI_ResManager;

 }

 void HippoUI_Global::InitResourceFactory(sinitInfo& info)
 {
	 if(m_DeviceType==HDT_D3D9)
	 {
		 m_pResFactory=new HippoUI_ResFactoryD3D9;
	 }
 }
 void HippoUI_Global::InitUIRender(sinitInfo& info)
 {
	 m_pUIRender=0;
	 if(m_DeviceType==HDT_D3D9)
	 {
		 m_pUIRender=new HippoUI_RenderD3D9(m_pDevice);
	 }
 }


 HippoUI_ResManager* HippoUI_Global::GetResManager()
 {
	 if(!m_pResManager)
		 m_pResManager=new HippoUI_ResManager;
	 return m_pResManager;
 }


 HippoUI_Render* HippoUI_Global::GetUIRender()
 {
	 return m_pUIRender;
 }

 IDirect3DDevice9* HippoUI_Global::GetDevice9()
 {
	 if (m_DeviceType==HDT_D3D9)
	 {
		 return (IDirect3DDevice9*)m_pDevice;
	 }
	 return 0;
 }

 HippoUI_ResFactory* HippoUI_Global::GetResFactory()
 {
	 return m_pResFactory;
 }

 HippoUI_Timer* HippoUI_Global::GetTimer()
 {
	 if(!m_pTimer)
		 m_pTimer=new HippoUI_Timer;
	 return m_pTimer;
 }