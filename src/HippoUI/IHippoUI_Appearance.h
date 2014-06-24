/********************************************************************
	created:	2012/10/20
	created:	20:10:2012   23:53
	filename: 	F:\Git\ClassicHippoTestBox\Common\HippoUI\IHippoUI_Appearance.h
	file path:	F:\Git\ClassicHippoTestBox\Common\HippoUI
	file base:	IHippoUI_Appearance
	file ext:	h
	author:		sssa2000
	
	purpose:	
*********************************************************************/
#pragma once

class IHippoUI_Appearance
{
public:
	virtual ~IHippoUI_Appearance(){}
	virtual void Render(float )=0;
	virtual void UpdateRects(){}
};

//最小的绘制元素，即一个贴图和一个字体
class HippoUI_DrawElement
{
public:
	HippoUI_DrawElement();
	~HippoUI_DrawElement();

	int m_BackGround_Color[4];
	int m_TextureColorFactor[4];
	int m_FontColorFactor[4];


};