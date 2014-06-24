/********************************************************************
	created:	2012/11/24
	created:	24:11:2012   22:00
	filename: 	F:\Git\ClassicHippoTestBox\Common\HippoUI\IHippoTextLayOut.h
	file path:	F:\Git\ClassicHippoTestBox\Common\HippoUI
	file base:	IHippoTextLayOut
	file ext:	h
	author:		sssa2000
	
	purpose:	�������ֲ����Ű�
*********************************************************************/
#pragma once
#include <Windows.h>

class IHippoTextLayOut
{
public:
	virtual ~IHippoTextLayOut(){};
	virtual void Init(HDC fontdc)=0;
	virtual void SetTextContent(const WCHAR* str)=0;
	virtual const WCHAR* GetTextContent()=0;
	virtual unsigned int GetTextLength()=0;
	//���ַ�����ĳ��λ�ã�����һ���ַ�
	virtual void InsertChar(int pos,WCHAR c)=0;
	virtual void InsertChar(int pos,const WCHAR* str)=0;
	//ɾ��ĳһ���ַ�
	virtual void EraseChar(int pos)=0;
	//ɾ��ĳn���ַ�
	virtual void EraseChar(int beg,int end)=0;
	//�滻һ��������ַ�
	virtual void ReplaceStrText(int beg,int end,WCHAR c)=0;
	virtual void ReplaceStrText(int beg,int end,const WCHAR* c)=0;
	//��ȡ������Ļλ�ã�x��
	//logicPos:�����߼�λ��
	//textStartXPos_Screen:�ַ�������Ļ���ĸ�λ�ÿ�ʼ������lineedit�����Ͻ�x
	//bFinishSide:��ʾ�Ƿ�Ҫ��ȡ���ַ�������λ�ã���Ϊfalseʱ���õ������ַ���ʼ��λ��
	//����ֵ-1��ʾ��ȡʧ�ܣ�>=0��ֵ��ʾ�ɹ�
	virtual int GetXPos_Screen(int logicPos,bool bFinishSide=false)=0;

	//����������Ļλ�� ��ȡ�����߼�λ��
	virtual int GetXPos_Logic(int screenXPos)=0;
};