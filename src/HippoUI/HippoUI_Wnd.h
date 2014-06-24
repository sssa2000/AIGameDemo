/********************************************************************
	created:	2012/10/16
	created:	16:10:2012   0:23
	filename: 	F:\Git\ClassicHippoTestBox\Common\HippoUI\HippoUI_Wnd.h
	file path:	F:\Git\ClassicHippoTestBox\Common\HippoUI
	file base:	HippoUI_Wnd
	file ext:	h
	author:		sssa2000
	
	purpose:	
*********************************************************************/
#pragma once
#include "HippoUI_Control.h"
#include <vector>
#include <map>
#include <set>
#include <functional>

class IHippoUI_Appearance;



class HippoUI_Wnd:public HippoUI_Control
{
	friend class HippoUI_Factory;
public:
	~HippoUI_Wnd();
	void Render(float escape)override;
	void AddControl(HippoUI_Control* p);
	virtual MSG_HANDLE_RES HandleWindowsMsg(UINT uMsg, WPARAM wParam, LPARAM lParam);
	
	HippoUI_Control* GetControlByID(int id);

	//设置一个控件获得焦点
	void SetFocus(HippoUI_Control* p);
	HippoUI_Control* GetFocus(){return m_pFocusControl;}
protected:

	void Init();
	void CleanUp();

	//保存窗体中的所有控件
	typedef std::vector<HippoUI_Control*> ControlCon;
	ControlCon m_all_controls;

	//保存所有的公共msg
	std::vector<unsigned int> m_PublicMsgs;

	IHippoUI_Appearance* m_pAppearance;
private:
	//有的消息不只是有焦点的能获得，例如MouseMove
	MSG_HANDLE_RES ProcessPublicMsg(UINT uMsg, WPARAM wParam, LPARAM lParam);
	//更新控件的焦点
	void UpdateFocusControl(UINT uMsg, WPARAM wParam, LPARAM lParam);
	//查找一个控件是否已经注册了消息处理函数
	//bool FindMsgFilter(unsigned int msg,HippoUI_Control* pControl);
	HippoUI_Control* m_pFocusControl;
	HippoUI_Wnd(int id,HippoUI_Control* p);
};