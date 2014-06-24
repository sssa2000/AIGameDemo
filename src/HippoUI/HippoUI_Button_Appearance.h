/********************************************************************
	created:	2012/11/04
	created:	4:11:2012   21:54
	filename: 	F:\Git\ClassicHippoTestBox\Common\HippoUI\HippoUI_Button_Appearance.h
	file path:	F:\Git\ClassicHippoTestBox\Common\HippoUI
	file base:	HippoUI_Button_Appearance
	file ext:	h
	author:		sssa2000
	
	purpose:	
*********************************************************************/
#pragma once
#include "IHippoUI_Appearance.h"
#include "HippoUI_ResManager.h"
#include <memory>

class HippoUI_Button;
class HippoUI_TextureProxy;

//appearance
class HippoUI_Button_DxutAppearance:public IHippoUI_Appearance
{
public:
	HippoUI_Button_DxutAppearance(HippoUI_Button* parent);
	~HippoUI_Button_DxutAppearance();
	void Render(float escape);
private:
	void Init();

	HippoUI_Button* m_pParent;

	HippoUI_TextureProxy* m_pButtonBackGroudTexture;
	HippoUI_TextureProxy* m_pButtonForeGroudTexture;
};
