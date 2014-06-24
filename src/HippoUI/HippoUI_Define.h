/********************************************************************
	created:	2012/11/02
	created:	2:11:2012   21:47
	filename: 	F:\Git\ClassicHippoTestBox\Common\HippoUI\HippoUI_Define.h
	file path:	F:\Git\ClassicHippoTestBox\Common\HippoUI
	file base:	HippoUI_Define
	file ext:	h
	author:		sssa2000
	
	purpose:	
*********************************************************************/
#pragma once
#include <Windows.h>
//
enum MSG_HANDLE_RES
{
	NOT_HANDLE=0,
	HAD_HANDLE
};
enum UISTATE
{
	LABEL_NORMAL,
	BUTTON_BG,
	BUTTON_FG,
	LINEEDIT_0,
	LINEEDIT_1,
	LINEEDIT_2,
	LINEEDIT_3,
	LINEEDIT_4,
	LINEEDIT_5,
	LINEEDIT_6,
	LINEEDIT_7,
	LINEEDIT_8,

	LISTBOX_FRAME,
	UISTATE_END
};

enum TEXT_AG
{
	TEXT_AG_LEFT,
	TEXT_AG_CENTER,
	TEXT_AG_RIGHT,
	TEXT_AG_AUTO,
};
extern const char* strUISTATE[UISTATE_END];

class HippoUI_Rect:public RECT
{
public:
	HippoUI_Rect()
	{
		left=0;
		right=0;
		top=0;
		bottom=0;
	}
	void Set(int leftupX,int leftupY,int rightDownX,int rightDownY)
	{
		left=leftupX;
		right=rightDownX;
		top=leftupY;
		bottom=rightDownY;
	}
	HippoUI_Rect(int leftupX,int leftupY,int rightDownX,int rightDownY)
	{
		Set(leftupX,leftupY,rightDownX,rightDownY);
	}
	~HippoUI_Rect(){}

	HippoUI_Rect(const HippoUI_Rect& r)
	{
		left=r.left;
		right=r.right;
		top=r.top;
		bottom=r.bottom;
	}

	bool bIsNullRect(){return left==right;}
	const RECT* GetRECT()const{return this;}

	void SetRectFromString(const char* str_rect);//”√∂∫∫≈∑÷∏Ó
	int GetWidth()const{return right-left;}
	int GetHeight()const{return bottom-top;}

	bool ContainPoint(int x,int y);
};

class HippoUi_Color
{
public:
	static HippoUi_Color White(){return HippoUi_Color(1,1,1,1);}
	static HippoUi_Color Black(){return HippoUi_Color(0,0,0,1);}

	
	float r,g,b,a;

	HippoUi_Color(float _r,float _g,float _b,float _a)
	{
		r=_r;
		g=_g;
		b=_b;
		a=_a;
	}
	
	
};

