/********************************************************************
	created:	2012/12/06
	created:	6:12:2012   0:55
	filename: 	F:\Git\ClassicHippoAppFrameWork\HippoAppFrameWork\Win32MsgUtil.h
	file path:	F:\Git\ClassicHippoAppFrameWork\HippoAppFrameWork
	file base:	Win32MsgUtil
	file ext:	h
	author:		sssa2000
	
	purpose:	
*********************************************************************/
#pragma once

#include <Windows.h>
#include <functional>

#include <ShellAPI.h>


struct HippoMouseEvent
{
	bool bMouseLeft;
	bool bMouseRight;
	bool bMouseMiddle;
	bool bKeyCtrl;
	bool bKeyShift;
	bool bKeyAlt;
	int PosX;
	int PosY;
};

struct HippoWheelEvent
{
	bool bKeyCtrl;
	bool bKeyShift;
	bool bKeyAlt;
	int wheel_delta;
};

struct HippoSizeEvent
{
	HippoSizeEvent(int _w,int _h)
	{
		w=_w;
		h=_h;
	}
	int w;
	int h;
};

//�Զ������Ϣ������
typedef  std::tr1::function<LRESULT (UINT uMsg, WPARAM wParam, LPARAM lParam)> CustomMsgProc;
//on size�¼�
typedef std::tr1::function<int (HippoSizeEvent& e)> WndSizeCallback;
//keydown�Ļص������������Ǵ��ںͰ��µļ��ļ���
typedef std::tr1::function<int (unsigned int)> KeyCallback;
//mouse key down�Ļص�����
typedef std::tr1::function<int (HippoMouseEvent& e)> MouseKeyCallback;
//mouse wheel
typedef std::tr1::function<int (HippoWheelEvent& w)> MouseWheelCallback;

//file drop
typedef std::tr1::function<void (HDROP hdrop)> DropFileCallback;

class Win32MsgUtil
{
public:
	static HippoMouseEvent GetMouseEventInfoFromMsg(UINT uMsg, WPARAM wParam, LPARAM lParam);
	static HippoWheelEvent GetWheelEventInfoFromMsg(UINT uMsg, WPARAM wParam, LPARAM lParam);
	static HippoSizeEvent GetHippoSizeEventFromMsg(UINT uMsg, WPARAM wParam, LPARAM lParam);
};


