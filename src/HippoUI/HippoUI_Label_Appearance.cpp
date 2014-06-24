#include "HippoUI_Label_Appearance.h"
#include "HippoUI_Render.h"
#include "HippoUI_Label.h"
#include "HippoUI_Global.h"

HippoUI_Label_DxutAppearance::HippoUI_Label_DxutAppearance(HippoUI_Label* p):
m_pParent(p)
{
	Init();
}

HippoUI_Label_DxutAppearance::~HippoUI_Label_DxutAppearance()
{

}

void HippoUI_Label_DxutAppearance::Render(float escape)
{
	//ÏÈ»­±³¾°¡¢ÔÙ»­×ÖÌå
	HippoUI_Rect r=m_pParent->GetScreenRect();
	const char* str=m_pParent->GetText();
	HippoUI_Render* pRender=HippoUI_Global::GetIns()->GetUIRender();
	pRender->DrawRect(r,HippoUi_Color(1,1,1,0.2f));
	pRender->DrawString(r,str,HippoUi_Color(1,1,1,1),TEXT_AG_AUTO);
	
}

void HippoUI_Label_DxutAppearance::Init()
{
	m_pLabelBackGroudTexture=HippoUI_Global::GetIns()->GetResManager()->GetTextureProxy(LABEL_NORMAL);
}