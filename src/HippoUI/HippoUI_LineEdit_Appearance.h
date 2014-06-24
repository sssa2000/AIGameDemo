/********************************************************************
	created:	2012/11/11
	created:	11:11:2012   21:18
	filename: 	F:\Git\ClassicHippoTestBox\Common\HippoUI\HippoUI_LineEdit_Appearance.h
	file path:	F:\Git\ClassicHippoTestBox\Common\HippoUI
	file base:	HippoUI_LineEdit_Appearance
	file ext:	h
	author:		sssa2000
	
	purpose:	
*********************************************************************/
#pragma once
#include "IHippoUI_Appearance.h"
#include "HippoUI_ResManager.h"
#include "HippoUI_Define.h"
#include <memory>

class HippoUI_LineEdit;
class HippoUI_TextureProxy;

#define LINEEDIT_AREA_NUM 9
//appearance
class HippoUI_LineEdit_DxutAppearance:public IHippoUI_Appearance
{
public:
	HippoUI_LineEdit_DxutAppearance(HippoUI_LineEdit* parent);
	~HippoUI_LineEdit_DxutAppearance();
	void Render(float escape);

	void UpdateRects();
	int GetTextAreaWidth();
private:
	HippoUI_Rect GetCaretRect();
	HippoUI_Rect GetSelectTextRect();
	void Init();

	bool m_bCaretDisplay;
	double m_dfBlink;  
    double m_dfLastBlink;
	HippoUI_LineEdit* m_pParent;
	/*
	//   0 - text area
	//   1 - top left border
	//   2 - top border
	//   3 - top right border
	//   4 - left border
	//   5 - right border
	//   6 - lower left border
	//   7 - lower border
	//   8 - lower right border
	*/
	HippoUI_TextureProxy* m_pTexture[LINEEDIT_AREA_NUM];
	HippoUI_Rect m_rcRender[LINEEDIT_AREA_NUM];
};
