/********************************************************************
	created:	2012/11/24
	created:	24:11:2012   22:06
	filename: 	F:\Git\ClassicHippoTestBox\Common\HippoUI\HippoTextLayOut_Impl_Uniscribe.h
	file path:	F:\Git\ClassicHippoTestBox\Common\HippoUI
	file base:	HippoTextLayOut_Impl_Uniscribe
	file ext:	h
	author:		sssa2000
	
	purpose:	
*********************************************************************/
#pragma once
#include "IHippoTextLayOut.h"
#include <string>
#include <usp10.h>

class HippoTextLayOut_Impl_Uniscribe:public IHippoTextLayOut
{
public:
	HippoTextLayOut_Impl_Uniscribe();
	~HippoTextLayOut_Impl_Uniscribe();
	virtual void Init(HDC fontdc);
	virtual void SetTextContent(const WCHAR* str);
	virtual const WCHAR* GetTextContent();
	virtual unsigned int GetTextLength();
	virtual void InsertChar(int pos,WCHAR c);
	virtual void InsertChar(int pos,const WCHAR* str);
	//ɾ��ĳn���ַ�
	virtual void EraseChar(int beg,int end);
	//ɾ��ĳһ���ַ�
	virtual void EraseChar(int pos);
	//�滻
	virtual void ReplaceStrText(int beg,int end,WCHAR c);
	virtual void ReplaceStrText(int beg,int end,const WCHAR* c);
	//��ȡ������Ļλ�ã�x��
	virtual int GetXPos_Screen(int logicPos,bool bFinishSide);
	//����������Ļλ�� ��ȡ�����߼�λ��
	virtual int GetXPos_Logic(int screenXPos);
private:
	std::wstring m_data;
	HDC m_font_dc;
	
	SCRIPT_STRING_ANALYSIS m_ups_analysis;

	void AnalyseString();
};