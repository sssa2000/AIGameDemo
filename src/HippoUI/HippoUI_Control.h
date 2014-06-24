/********************************************************************
	created:	2012/10/16
	created:	16:10:2012   0:25
	filename: 	F:\Git\ClassicHippoTestBox\Common\HippoUI\HippoUI_Control.h
	file path:	F:\Git\ClassicHippoTestBox\Common\HippoUI
	file base:	HippoUI_Control
	file ext:	h
	author:		sssa2000
	
	purpose:	
*********************************************************************/
#pragma once

#include <Windows.h>
#include "HippoUI_Define.h"
#include <map>
#include <functional>

class HippoUI_MouseMessage;
class HippoUI_Control
{
public:
	//每个控件消息处理的函数
	typedef std::tr1::function<MSG_HANDLE_RES (WPARAM,LPARAM)> HippoUI_MsgFun;
	//按照消息id保存处理该消息的函数（暂时只实现一个消息对应一个处理函数）
	typedef std::map<unsigned int,HippoUI_MsgFun > MsgFilterCon;

	HippoUI_Control(int id,HippoUI_Control* pParent)
	{
		m_id=id;
		m_posX=0;
		m_posY=0;
		m_bIsVisible=true;
		//m_bFocus=false;
		m_width=m_height=0;
		m_pParent=pParent;
	}
	virtual ~HippoUI_Control(){}
	virtual void UpdateRects(){}

	int GetID(){return m_id;}
	void SetPos(int x,int y){m_posX=x;m_posY=y;}
	void SetSize(int w,int h){m_width=w;m_height=h;}
	
	bool IsVisible(){return m_bIsVisible;}
	void SetVisible(bool b){m_bIsVisible=b;}

	//bool HasFocus(){return m_bFocus;}
	//void SetFocus(bool b){m_bFocus=b;}

	HippoUI_Rect GetLocalRect(){return HippoUI_Rect(0,0,m_width,m_height);}
	HippoUI_Rect GetScreenRect(){return HippoUI_Rect(m_posX,m_posY,m_posX+m_width,m_posY+m_height);}
	bool ScreenPointInControl(int x,int y)
	{
		bool b0=x>m_posX && x<m_posX+m_width;
		bool b1=y>m_posY && y<m_posY+m_height;
		return b1 && b0;
	}

	virtual void Render(float escape)=0;

	virtual HippoUI_MsgFun* GetMsgFun(unsigned int msgID)
	{
		auto itr=m_MsgCon.find(msgID);
		if(itr==m_MsgCon.end())
			return 0;
		return &(itr->second);
	}

protected:

	//virtual void InitMsgProcessFun()=0;

	bool m_bIsVisible;
	int m_posX;
	int m_posY;
	int m_width;
	int m_height;

	int m_id;
	MsgFilterCon m_MsgCon;
	HippoUI_Control* m_pParent;
private:
};