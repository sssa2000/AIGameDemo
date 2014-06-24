/********************************************************************
	created:	2012/10/21
	created:	21:10:2012   11:17
	filename: 	F:\Git\ClassicHippoTestBox\Common\HippoUI\HippoUI_Wnd_Appearance.h
	file path:	F:\Git\ClassicHippoTestBox\Common\HippoUI
	file base:	HippoUI_Wnd_Appearance
	file ext:	h
	author:		sssa2000
	
	purpose:	
*********************************************************************/
#pragma once
#include "IHippoUI_Appearance.h"

//appearance
class HippoUI_Wnd_Appearance:public IHippoUI_Appearance
{
public:
	void Render(float escape);
private:
};
