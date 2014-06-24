/********************************************************************
	created:	2012/10/17
	created:	17:10:2012   0:23
	filename: 	F:\Git\ClassicHippoTestBox\Common\HippoUI\HippoUI_Label.h
	file path:	F:\Git\ClassicHippoTestBox\Common\HippoUI
	file base:	HippoUI_Label
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
//保存控件的数据
struct _Label_State
{
	std::string m_text;
};


class IHippoUI_Appearance;

class HippoUI_Label:public HippoUI_Control
{
	friend class HippoUI_Factory;
public:
	~HippoUI_Label();
	void Render(float escape)override;
	void SetText(const char* str){m_state.m_text=str;}
	const char* GetText(){return m_state.m_text.c_str();}


protected:

	void Init();
	void CleanUp();
	_Label_State m_state;
	std::tr1::shared_ptr<IHippoUI_Appearance> m_pAppearance;


private:
	HippoUI_Label(int id,HippoUI_Control* pParent);


};