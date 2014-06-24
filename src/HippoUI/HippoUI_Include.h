/********************************************************************
	created:	2012/10/16
	created:	16:10:2012   0:23
	filename: 	F:\Git\ClassicHippoTestBox\Common\HippoUI\HippoUI_Include.h
	file path:	F:\Git\ClassicHippoTestBox\Common\HippoUI
	file base:	HippoUI_Include
	file ext:	h
	author:		sssa2000
	
	purpose:	
*********************************************************************/
#pragma once

#include "HippoUI_Control.h"

#include <vector>

class HippoUI_Wnd;
class HippoUI_Label;
class HippoUI_Button;
class HippoUI_LineEdit;
class HippoUI_ListBox;

enum HIPPOUI_DEVICE_TYPE
{
	HDT_D3D9,
	HDT_D3D11,
	HDT_END
};

enum MOUSE_KEY_STATE
{

};
class HippoUI_MouseMessage
{
public:
	HippoUI_MouseMessage()
	{
		x=y=-1;
		leftkey_state=rightkey_state=0;
	}

	int x;
	int y;
	int leftkey_state;//1���£�2̧��0�ޱ仯
	int rightkey_state;//1���£�2̧��0�ޱ仯
};

//HippoUI������������UI��Interface
//������Get��ͨ�������
class HippoUI
{
public:
	HippoUI(const char* stylefilename,void* device,HIPPOUI_DEVICE_TYPE t=HDT_D3D9);
	void Init(const char* stylefilename);
	void CleanUp();
	HippoUI_Wnd* CreateWnd(int controlId);
	HippoUI_Wnd* GetWndByIdx(int idx);

	HippoUI_Label* CreateLabel(HippoUI_Wnd* parent,int controlId,int posX,int posY,int w,int h);
	HippoUI_Button* CreateButton(HippoUI_Wnd* parent,int controlId,int posX,int posY,int w,int h);
	HippoUI_LineEdit* CreateLineEdit(HippoUI_Wnd* parent,int controlId,int posX,int posY,int w,int h);
	HippoUI_ListBox* CreateListBox(HippoUI_Wnd* parent,int controlId,int posX,int posY,int w,int h);

	//����������
	void OnLostDevice();
	void OnResetDevice();
	void OnSize(int w,int h);

	//����������룬����0��ʾ�Ը���Ϣ������Ȥ������1��ʾ�Ѿ�����
	int HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam);
	
	void Update(float escape);
	void Render(float escape);

	//bool ConnectSignal(HippoUI_Control* pControl,);

	
private:
	int HandleMouseMessage( UINT uMsg, WPARAM wParam, LPARAM lParam);
	typedef std::vector<HippoUI_Wnd*> WndCon;
	typedef WndCon::iterator WndConItr;
	WndCon m_all_wnd;
};