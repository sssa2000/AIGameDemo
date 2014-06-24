#include "HippoUI_Wnd.h"
#include "HippoUI_Wnd_Appearance.h"
#include <algorithm>
#include "HippoUI_Button.h"


HippoUI_Wnd::HippoUI_Wnd(int id,HippoUI_Control* p):
HippoUI_Control(id,0)
{
	Init();
}

HippoUI_Wnd::~HippoUI_Wnd()
{
	CleanUp();
}

void HippoUI_Wnd::Init()
{
	m_pFocusControl=0;
	m_pAppearance=new HippoUI_Wnd_Appearance;

	m_PublicMsgs.push_back(WM_MOUSEMOVE);
}

void HippoUI_Wnd::CleanUp()
{
	delete m_pAppearance;
}

void HippoUI_Wnd::AddControl(HippoUI_Control* p)
{
	m_all_controls.push_back(p);
	p->UpdateRects();
}

HippoUI_Control* HippoUI_Wnd::GetControlByID(int id)
{
	auto itr=m_all_controls.begin();
	auto itrend=m_all_controls.end();
	while (itr!=itrend)
	{
		HippoUI_Control* p=*itr;
		if(p->GetID()==id)
			return p;
	}
	return 0;
}

void HippoUI_Wnd::Render(float escape)
{
	//first draw itself
	m_pAppearance->Render(escape);

	//then draw all control 
	auto itr=m_all_controls.begin();
	auto itrend=m_all_controls.end();
	while (itr!=itrend)
	{
		HippoUI_Control* p=*itr;
		p->Render(escape);
		++itr;
	}
}
void HippoUI_Wnd::UpdateFocusControl(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(uMsg==WM_LBUTTONDOWN)
	{
		//按下的位置（窗口坐标系下）
		int x=( int )LOWORD( lParam );
		int y=( int )HIWORD( lParam );

		auto itr=m_all_controls.begin();
		auto itrend=m_all_controls.end();
		while (itr!=itrend)
		{
			HippoUI_Control* pControl=*itr;
			bool b=pControl->ScreenPointInControl(x,y);
			if(b)
			{
				SetFocus(pControl);
				return; //目前只支持一个有焦点的控件
			}
			++itr;
		}
	}

}

MSG_HANDLE_RES HippoUI_Wnd::HandleWindowsMsg(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//将已有的获得焦点的控件清空
	if(uMsg==WM_LBUTTONDOWN && m_pFocusControl)
		SetFocus(0);

	UpdateFocusControl(uMsg,wParam,lParam);

	MSG_HANDLE_RES r1=ProcessPublicMsg(uMsg,wParam,lParam);
	if(r1==NOT_HANDLE && m_pFocusControl)
	{
		HippoUI_MsgFun* funptr=m_pFocusControl->GetMsgFun(uMsg);
		if(!funptr)
			return NOT_HANDLE;
		return (*funptr)(wParam,lParam);
	}
	return NOT_HANDLE;
}


//设置一个控件获得焦点
void HippoUI_Wnd::SetFocus(HippoUI_Control* p)
{
	m_pFocusControl=p;
}

//有的消息不只是有焦点的能获得，例如MouseMove
MSG_HANDLE_RES HippoUI_Wnd::ProcessPublicMsg(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	auto itr=m_PublicMsgs.begin();
	auto itrend=m_PublicMsgs.end();
	auto itrfind=std::find(itr,itrend,uMsg);
	if(itrfind==itrend)
		return NOT_HANDLE;

	auto vec_itr=m_all_controls.begin();
	auto vec_itrend=m_all_controls.end();
	while (vec_itr!=vec_itrend)
	{
		HippoUI_Control* p=*vec_itr;
		HippoUI_MsgFun* funptr=p->GetMsgFun(uMsg);
		if(funptr)
		{
			MSG_HANDLE_RES r=(*funptr)(wParam,lParam);
			if(r==HAD_HANDLE)
				return HAD_HANDLE;
		}
		++vec_itr;
	}
	return NOT_HANDLE;
}