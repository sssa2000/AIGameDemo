/********************************************************************
	created:	2012/11/24
	created:	24:11:2012   22:00
	filename: 	F:\Git\ClassicHippoTestBox\Common\HippoUI\IHippoTextLayOut.h
	file path:	F:\Git\ClassicHippoTestBox\Common\HippoUI
	file base:	IHippoTextLayOut
	file ext:	h
	author:		sssa2000
	
	purpose:	处理文字布局排版
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
	//在字符串的某个位置，插入一个字符
	virtual void InsertChar(int pos,WCHAR c)=0;
	virtual void InsertChar(int pos,const WCHAR* str)=0;
	//删除某一个字符
	virtual void EraseChar(int pos)=0;
	//删除某n个字符
	virtual void EraseChar(int beg,int end)=0;
	//替换一个区域的字符
	virtual void ReplaceStrText(int beg,int end,WCHAR c)=0;
	virtual void ReplaceStrText(int beg,int end,const WCHAR* c)=0;
	//获取光标的屏幕位置（x）
	//logicPos:光标的逻辑位置
	//textStartXPos_Screen:字符串从屏幕的哪个位置开始，例如lineedit的左上角x
	//bFinishSide:表示是否要获取到字符结束的位置，当为false时，得到的是字符起始的位置
	//返回值-1表示获取失败，>=0的值表示成功
	virtual int GetXPos_Screen(int logicPos,bool bFinishSide=false)=0;

	//根据鼠标的屏幕位置 获取光标的逻辑位置
	virtual int GetXPos_Logic(int screenXPos)=0;
};