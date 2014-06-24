/********************************************************************
	created:	2012/12/10
	created:	10:12:2012   21:37
	filename: 	F:\Git\ClassicHippoAppFrameWork\HippoUI\HippoUI_ListBox.h
	file path:	F:\Git\ClassicHippoAppFrameWork\HippoUI
	file base:	HippoUI_ListBox
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

class HippoUI_ListBoxItem
{
public:
	HippoUI_ListBoxItem(const char* str,bool bVis)
	{
		m_text=str;
		m_bVisible=bVis;
	}

	const std::string& GetText(){return m_text;}

private:
	std::string m_text;
	bool m_bVisible;	

};
class HippoUI_ListBox:public HippoUI_Control
{
	friend class HippoUI_Factory;
public:
	~HippoUI_ListBox();
	void Render(float escape)override;
	void AddItem(const char* str);
	HippoUI_ListBoxItem* GetItem(int i);
	int GetItemNum(){return m_all_item.size();}
	int GetItemHeight();
protected:
	void Init();
	void CleanUp();

	//边框的宽度
	int m_nBorder;
	//每一个item文字前的空白宽度
	int m_nMargin;
	//每一个item的高度
	int m_nItemHeight;
	
	std::vector<HippoUI_ListBoxItem*> m_all_item;
	std::tr1::shared_ptr<IHippoUI_Appearance> m_pAppearance;
private:
	HippoUI_ListBox(int id,HippoUI_Control* pParent);
};