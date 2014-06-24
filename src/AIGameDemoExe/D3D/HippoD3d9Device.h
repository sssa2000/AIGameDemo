/********************************************************************
	created:	2012/12/07
	created:	7:12:2012   0:37
	filename: 	F:\Git\ClassicHippoAppFrameWork\HippoDxAppDemo\HippoD3d9Device.h
	file path:	F:\Git\ClassicHippoAppFrameWork\HippoDxAppDemo
	file base:	HippoD3d9Device
	file ext:	h
	author:		sssa2000
	
	purpose:	封装d3d的初始化和销毁
*********************************************************************/
#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <Windows.h>
class HippoD3d9Device
{
public:
	HippoD3d9Device();
	~HippoD3d9Device();

	void Init(HWND hwnd,int w,int h,bool bWindowed=true);
	void CleanUp();
	void BeginRender();
	void EndRender();
	void SetViewport(int w,int h);
	IDirect3DDevice9* GetDeviceD3D9();
	float GetFPS();
	void ProcessDeviceLost();
	D3DPRESENT_PARAMETERS& FillD3dParameter(int w, int h, bool bWindowed);
	void SetMaxFPS(unsigned int max_fps);
private:
	

	bool m_bWindowed;
	int m_width;
	int m_height;
	float  m_fps;
	unsigned int currentTick;
	unsigned int lastTick;
	D3DPRESENT_PARAMETERS m_d3dpp;
	IDirect3D9* m_pD3D;
	IDirect3DDevice9* m_pd3dDevice;
	unsigned int m_maxfps; //0表示无限制
	double inv_maxfps;
};