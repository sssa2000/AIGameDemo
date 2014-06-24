/********************************************************************
	created:	2012/12/04
	created:	4:12:2012   23:58
	filename: 	F:\Git\ClassicHippoAppFrameWork\HippoAppFrameWork\HippoAppBase.h
	file path:	F:\Git\ClassicHippoAppFrameWork\HippoAppFrameWork
	file base:	HippoAppBase
	file ext:	h
	author:		sssa2000
	
	purpose:	
*********************************************************************/
#pragma once
#include "HippoWindow.h"
#include <memory>
class HippoAppBase
{
public:
	HippoAppBase(const char* name,unsigned int w,unsigned int h,unsigned int left,unsigned int top);
	virtual ~HippoAppBase();
	virtual void Init();
	virtual void Shutdown();
	void StartRun();
	HippoWindow* GetWnd(){return m_pWnd.get();}
protected:
	
	//以下函数为用户覆盖函数
	virtual void InitApp()=0;
	virtual void ShutdownApp(){}
	virtual void Render(unsigned int escapeMs){}
	virtual void Update(unsigned int escapeMs){}


	std::tr1::shared_ptr<HippoWindow> m_pWnd;

};
