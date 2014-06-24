#include "HippoUI_Button.h"
#include "HippoUI_Button_Appearance.h"
#include "HippoUI_Include.h"
#include "HippoUI_HelpFun.h"
#include "HippoUI_Wnd.h"
#include <functional>

HippoUI_Button::HippoUI_Button(int id,HippoUI_Control* pParent):
HippoUI_Control(id,pParent)
{
	Init();
}



HippoUI_Button::~HippoUI_Button()
{
	CleanUp();
}

void HippoUI_Button::Init()
{
	//注册消息回调
	HippoUI_Wnd* pwnd=(HippoUI_Wnd*)m_pParent;
	m_MsgCon[WM_LBUTTONDOWN]=std::tr1::bind(&HippoUI_Button::OnMouseLeftDown,this,std::placeholders::_1,std::placeholders::_2);
	m_MsgCon[WM_LBUTTONUP]=std::tr1::bind(&HippoUI_Button::OnMouseLeftUp,this,std::placeholders::_1,std::placeholders::_2);
	m_MsgCon[WM_MOUSEMOVE]=std::tr1::bind(&HippoUI_Button::OnMouseOver,this,std::placeholders::_1,std::placeholders::_2);
	m_MsgCon[WM_LBUTTONDBLCLK]=std::tr1::bind(&HippoUI_Button::OnMouseLeftUp,this,std::placeholders::_1,std::placeholders::_2);

	m_pAppearance=make_shared<HippoUI_Button_DxutAppearance>(this);
	m_data.m_state=HBS_NORMAL;
}


void HippoUI_Button::CleanUp()
{
	
}

void HippoUI_Button::Render(float escape)
{
	m_pAppearance->Render(escape);
}



MSG_HANDLE_RES HippoUI_Button::OnMouseLeftDown(WPARAM wParam, LPARAM lParam)
{
	//按下的位置（窗口坐标系下）
	int x=( int )LOWORD( lParam );
	int y=( int )HIWORD( lParam );
	if(ScreenPointInControl(x,y))
	{
		m_data.m_state=HBS_PRESSED;
		return HAD_HANDLE;
	}
	return NOT_HANDLE;
}

MSG_HANDLE_RES HippoUI_Button::OnMouseLeftUp(WPARAM wParam, LPARAM lParam)
{
	//按下的位置（窗口坐标系下）
	int x=( int )LOWORD( lParam );
	int y=( int )HIWORD( lParam );
	if(ScreenPointInControl(x,y))
	{
		m_data.m_state=HBS_NORMAL;
		m_click_event(this); //回调执行
		return HAD_HANDLE;
	}
	return NOT_HANDLE;
}

MSG_HANDLE_RES HippoUI_Button::OnMouseOver(WPARAM wParam, LPARAM lParam)
{
	//按下的位置（窗口坐标系下）
	int x=( int )LOWORD( lParam );
	int y=( int )HIWORD( lParam );
	m_data.m_state=HBS_NORMAL;
	if(ScreenPointInControl(x,y))
	{
		m_data.m_state=HBS_MOUSEOVER;
		return HAD_HANDLE;
	}
	return NOT_HANDLE;
}