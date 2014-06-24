#include "HippoUI_Include.h"
#include "HippoUI_Factory.h"
#include "HippoUI_ResManager.h"
#include "HippoUI_Global.h"
#include "HippoUI_Render.h"
#include "HippoUI_Render.h"
#include "HippoUI_Wnd.h"
#include "HippoUI_Label.h"
#include "HippoUI_Button.h"
#include "HippoUI_LineEdit.h"
#include "HippoUI_ListBox.h"
#include <algorithm>

HippoUI::HippoUI(const char* stylefilename,void* device,HIPPOUI_DEVICE_TYPE t)
{
	sinitInfo s;
	s.stylefilename=stylefilename;
	s.pDevice=device;
	s.t=t;
	HippoUI_Global::Init(s);
	HippoUI_Global::GetIns()->GetResManager()->LoadUIStyleFromFile(stylefilename);
}

void HippoUI::CleanUp()
{
	HippoUI_Global::Destroy();
}


HippoUI_Wnd* HippoUI::CreateWnd(int controlId)
{
	HippoUI_Wnd* p=HippoUI_Factory::CreateWnd(controlId);
	m_all_wnd.push_back(p);
	return p;
}

HippoUI_Wnd* HippoUI::GetWndByIdx(int idx)
{
	auto itr=std::find_if(
		m_all_wnd.begin(),
		m_all_wnd.end(),
		[&](HippoUI_Wnd* pWnd)->bool{return pWnd->GetID()==idx;}
	);

	if(itr!=m_all_wnd.end())
	{
		return (*itr);
	}
	return 0;
}

HippoUI_Label* HippoUI::CreateLabel(HippoUI_Wnd* parent,int controlId,int posX,int posY,int w,int h)
{
	HippoUI_Label* p=HippoUI_Factory::CreateLabel(controlId,parent);
	p->SetPos(posX,posY);
	p->SetSize(w,h);
	parent->AddControl(p);
	return p; 
}

HippoUI_Button* HippoUI::CreateButton(HippoUI_Wnd* parent,int controlId,int posX,int posY,int w,int h)
{
	HippoUI_Button* p=HippoUI_Factory::CreateButton(controlId,parent);
	p->SetPos(posX,posY);
	p->SetSize(w,h);
	parent->AddControl(p);
	return p; 
}

HippoUI_LineEdit* HippoUI::CreateLineEdit(HippoUI_Wnd* parent,int controlId,int posX,int posY,int w,int h)
{
	HippoUI_LineEdit* p=HippoUI_Factory::CreateLineEdit(controlId,parent);
	p->SetPos(posX,posY);
	p->SetSize(w,h);
	parent->AddControl(p);
	return p; 
}

HippoUI_ListBox* HippoUI::CreateListBox(HippoUI_Wnd* parent,int controlId,int posX,int posY,int w,int h)
{
	HippoUI_ListBox* p=HippoUI_Factory::CreateListBox(controlId,parent);
	p->SetPos(posX,posY);
	p->SetSize(w,h);
	parent->AddControl(p);
	return p; 
}


void HippoUI::Update(float escape)
{
	//std::for_each(
	//	m_all_wnd.begin(),
	//	m_all_wnd.end(),
	//	[&](HippoUI_Wnd* pWnd){pWnd->Update(escape);}
	//);
}

void HippoUI::Render(float escape)
{
	HippoUI_Render* pUIRender=HippoUI_Global::GetIns()->GetUIRender();
	pUIRender->BeginRender();
	std::for_each(
		m_all_wnd.begin(),
		m_all_wnd.end(),
		[&](HippoUI_Wnd* pWnd){pWnd->Render(escape);}
		);
	pUIRender->EndRender();
}

//这里统一分析鼠标事件，分发给各个wnd
int HippoUI::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	auto itr=m_all_wnd.begin();
	auto itrend=m_all_wnd.end();
	while (itr!=itrend)
	{
		HippoUI_Wnd* pwnd=*itr;
		if(pwnd->IsVisible())
		{
			int handed=pwnd->HandleWindowsMsg(uMsg,wParam,lParam);
			if(handed)
				return 1;
		}
		++itr;
	}
	return 0;

}

int HippoUI::HandleMouseMessage( UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//HippoUI_MouseMessage _event;
	//if(uMsg==WM_LBUTTONDOWN)
	//	_event.leftkey_state=1;
	//else if (uMsg==WM_LBUTTONUP)
	//	_event.leftkey_state=2;
	//else if (uMsg==WM_RBUTTONDOWN)
	//	_event.rightkey_state=1;
	//else if (uMsg==WM_RBUTTONUP)
	//	_event.rightkey_state=2;
	//
	////按下的位置（窗口坐标系下）
	//_event.x=( int )LOWORD( lParam );
	//_event.y=( int )HIWORD( lParam );

	//auto itr=m_all_wnd.begin();
	//auto itrend=m_all_wnd.end();
	//while (itr!=itrend)
	//{
	//	HippoUI_Wnd* pwnd=*itr;
	//	if(pwnd->IsVisible())
	//	{
	//		int handed=pwnd->HandleWindowsMsg(uMsg,wParam,lParam);
	//		if(handed)
	//			return 1;
	//	}
	//	++itr;
	//}
	return 0;
}

void HippoUI::OnLostDevice()
{
	HippoUI_Global::GetIns()->GetUIRender()->OnLostDevice();
}
void HippoUI::OnResetDevice()
{
	HippoUI_Global::GetIns()->GetUIRender()->OnResetDevice();
}
//处理窗体缩放
void HippoUI::OnSize(int w,int h)
{
}