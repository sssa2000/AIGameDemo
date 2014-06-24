/********************************************************************
	created:	2012/10/20
	created:	20:10:2012   21:29
	filename: 	F:\Git\ClassicHippoTestBox\Common\HippoUI\HippoUI_Factory.h
	file path:	F:\Git\ClassicHippoTestBox\Common\HippoUI
	file base:	HippoUI_Factory
	file ext:	h
	author:		sssa2000
	
	purpose:	
*********************************************************************/
#pragma once
#include "HippoUI_Include.h"

class HippoUI_Wnd;
class HippoUI_Label;
class HippoUI_Button;
class HippoUI_LineEdit;
//以后有需要再换成抽象工厂
class HippoUI_Factory
{
public:
	static HippoUI_Wnd* CreateWnd(int id);
	static HippoUI_Label* CreateLabel(int id,HippoUI_Wnd* pParent);
	static HippoUI_Button* CreateButton(int id,HippoUI_Wnd* pParent);
	static HippoUI_LineEdit* CreateLineEdit(int id,HippoUI_Wnd* pParent);
	static HippoUI_ListBox* CreateListBox(int id,HippoUI_Wnd* pParent);
};