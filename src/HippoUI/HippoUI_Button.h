/********************************************************************
	created:	2012/11/04
	created:	4:11:2012   21:29
	filename: 	F:\Git\ClassicHippoTestBox\Common\HippoUI\HippoUI_Button.h
	file path:	F:\Git\ClassicHippoTestBox\Common\HippoUI
	file base:	HippoUI_Button
	file ext:	h
	author:		sssa2000
	
	purpose:	
*********************************************************************/
#pragma once

#include "HippoUI_Control.h"
#include "HippoUI_Factory.h"
#include <string>
#include <functional>
#include <memory>
class IHippoUI_Appearance;

enum HippoUI_Button_State
{
	HBS_NORMAL,
	HBS_PRESSED,
	HBS_MOUSEOVER,
};
//保存控件的数据
struct _Button_Data
{
	std::string m_text;
	HippoUI_Button_State m_state;
};

class HippoUI_Button:public HippoUI_Control
{
	friend class HippoUI_Factory;
public:
	~HippoUI_Button();
	void Render(float escape)override;
	void SetText(const char* str){m_data.m_text=str;}
	const char* GetText(){return m_data.m_text.c_str();}

	typedef std::tr1::function<void (HippoUI_Button*)> HippoUI_Button_ClickEvent_Fun;
	HippoUI_Button_ClickEvent_Fun& GetClickEvent(){return m_click_event;}
	void ConnectClickEvent(HippoUI_Button_ClickEvent_Fun& f){m_click_event=f;}

	HippoUI_Button_State GetState(){return m_data.m_state;}

	MSG_HANDLE_RES OnMouseLeftDown(WPARAM wParam, LPARAM lParam);
	MSG_HANDLE_RES OnMouseLeftUp(WPARAM wParam, LPARAM lParam);
	MSG_HANDLE_RES OnMouseOver(WPARAM wParam, LPARAM lParam);
protected:
	
	void Init();
	void CleanUp();
	std::tr1::shared_ptr<IHippoUI_Appearance> m_pAppearance;
	HippoUI_Button_ClickEvent_Fun m_click_event;
	_Button_Data m_data;

private:
	HippoUI_Button(int id,HippoUI_Control* pParent);
};