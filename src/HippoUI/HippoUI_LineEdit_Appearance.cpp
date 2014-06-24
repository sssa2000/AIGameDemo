#include "HippoUI_LineEdit_Appearance.h"
#include "HippoUI_LineEdit.h"
#include "HippoUI_Global.h"
#include "HippoUI_Render.h"
#include "HippoUI_Define.h"
#include "HippoUI_Timer.h"

HippoUI_LineEdit_DxutAppearance::HippoUI_LineEdit_DxutAppearance(HippoUI_LineEdit* parent):
m_pParent(parent)
{
	Init();
}

HippoUI_LineEdit_DxutAppearance::~HippoUI_LineEdit_DxutAppearance()
{

}
HippoUI_Rect HippoUI_LineEdit_DxutAppearance::GetCaretRect()
{
	HippoUI_Render* pRender=HippoUI_Global::GetIns()->GetUIRender();
	int caret_x=m_pParent->GetScreenCaretXPos();
	HippoUI_Rect rcCaret=m_rcRender[0];

	// Inflate further by m_nSpacing
	int spacing=m_pParent->GetSpacing();
	InflateRect( &rcCaret, -spacing, -spacing );

	return HippoUI_Rect
		(
		rcCaret.left+caret_x, 
		rcCaret.top,
		rcCaret.left+1+caret_x, 
		rcCaret.bottom 
		);
}

HippoUI_Rect HippoUI_LineEdit_DxutAppearance::GetSelectTextRect()
{
	 std::pair<int,int> range=m_pParent->GetVisibleSelectTextRange();
	 HippoUI_Rect rcSel=m_rcRender[0];
 	 int spacing=m_pParent->GetSpacing();
 	 InflateRect( &rcSel, -spacing, -spacing );

	 rcSel.right=rcSel.left+range.second;
	 	 rcSel.left+=range.first;

	 return rcSel;
}


void HippoUI_LineEdit_DxutAppearance::Render(float escape)
{
	const WCHAR* str=m_pParent->GetText()+m_pParent->GetFirstRenderCharIdx();
	HippoUI_Render* pRender=HippoUI_Global::GetIns()->GetUIRender();
	HippoUI_Timer* pTimer=HippoUI_Global::GetIns()->GetTimer();
	//draw button background
	HippoUI_Rect r=m_pParent->GetScreenRect();

	for (int i=0;i<LINEEDIT_AREA_NUM;++i)
	{
		pRender->DrawSprite(m_rcRender[i],m_pTexture[i],HippoUi_Color(1,1,1,1));
	}

	//draw select text rect
	if(m_pParent->HasSelectText())
	{
		HippoUI_Rect selrect=GetSelectTextRect();
		pRender->DrawRect(selrect,HippoUi_Color(40.0f/255.f,72.0f/255.f,72.0f/255.f,1));
	}


	//text
	HippoUI_Rect rcCaret=m_rcRender[0];
	// Inflate further by m_nSpacing
	int spacing=m_pParent->GetSpacing();
	InflateRect( &rcCaret, -spacing, -spacing );
	pRender->DrawString(rcCaret,str,HippoUi_Color(1,0,1,1),TEXT_AG_LEFT);


	//»æÖÆ¹â±ê
	if(!m_pParent->HasFocus())
		return;

	if(pTimer->GetAbsoluteTime() - m_dfLastBlink >= 0.5 )
	{
		m_bCaretDisplay = !m_bCaretDisplay;
		m_dfLastBlink = pTimer->GetAbsoluteTime();
	}
	if(m_bCaretDisplay)
		pRender->DrawRect(GetCaretRect(),HippoUi_Color(0,0,0,1));
}

void HippoUI_LineEdit_DxutAppearance::Init()
{
	m_dfBlink=0;
	m_dfLastBlink=0;
	m_bCaretDisplay=true;

	m_pTexture[0]=HippoUI_Global::GetIns()->GetResManager()->GetTextureProxy(LINEEDIT_0);
	m_pTexture[1]=HippoUI_Global::GetIns()->GetResManager()->GetTextureProxy(LINEEDIT_1);
	m_pTexture[2]=HippoUI_Global::GetIns()->GetResManager()->GetTextureProxy(LINEEDIT_2);
	m_pTexture[3]=HippoUI_Global::GetIns()->GetResManager()->GetTextureProxy(LINEEDIT_3);
	m_pTexture[4]=HippoUI_Global::GetIns()->GetResManager()->GetTextureProxy(LINEEDIT_4);
	m_pTexture[5]=HippoUI_Global::GetIns()->GetResManager()->GetTextureProxy(LINEEDIT_5);
	m_pTexture[6]=HippoUI_Global::GetIns()->GetResManager()->GetTextureProxy(LINEEDIT_6);
	m_pTexture[7]=HippoUI_Global::GetIns()->GetResManager()->GetTextureProxy(LINEEDIT_7);
	m_pTexture[8]=HippoUI_Global::GetIns()->GetResManager()->GetTextureProxy(LINEEDIT_8);


	UpdateRects();
}

void HippoUI_LineEdit_DxutAppearance::UpdateRects()
{
	// Update the text rectangle
	HippoUI_Rect totalrect=m_pParent->GetScreenRect();
	HippoUI_Rect m_rcText=totalrect;
	// First inflate by m_nBorder to compute render rects
	int border=m_pParent->GetBorderWidth();
	InflateRect( &m_rcText, -border, -border );


	m_rcRender[0] = m_rcText;
	m_rcRender[1].Set(totalrect.left, totalrect.top, m_rcText.left, m_rcText.top );
	m_rcRender[2].Set(m_rcText.left, totalrect.top, m_rcText.right, m_rcText.top );
	m_rcRender[3].Set(m_rcText.right, totalrect.top, totalrect.right, m_rcText.top );
	m_rcRender[4].Set(totalrect.left, m_rcText.top, m_rcText.left, m_rcText.bottom );
	m_rcRender[5].Set(m_rcText.right, m_rcText.top, totalrect.right, m_rcText.bottom );
	m_rcRender[6].Set(totalrect.left, m_rcText.bottom, m_rcText.left, totalrect.bottom );
	m_rcRender[7].Set(m_rcText.left, m_rcText.bottom, m_rcText.right, totalrect.bottom );
	m_rcRender[8].Set(m_rcText.right, m_rcText.bottom, totalrect.right, totalrect.bottom );


}
int HippoUI_LineEdit_DxutAppearance::GetTextAreaWidth()
{
	HippoUI_Rect rcText=m_rcRender[0];
	int spacing=m_pParent->GetSpacing();
	InflateRect( &rcText, -spacing, -spacing );
	return rcText.right-rcText.left;
}