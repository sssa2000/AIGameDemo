#include "HippoUI_Button_Appearance.h"
#include "HippoUI_Button.h"
#include "HippoUI_Global.h"
#include "HippoUI_Render.h"
#include "HippoUI_Define.h"

HippoUI_Button_DxutAppearance::HippoUI_Button_DxutAppearance(HippoUI_Button* parent):
m_pParent(parent)
{
	Init();
}
HippoUI_Button_DxutAppearance::~HippoUI_Button_DxutAppearance()
{

}

void HippoUI_Button_DxutAppearance::Render(float escape)
{
	const char* str=m_pParent->GetText();

	//draw button background
	HippoUI_Rect r=m_pParent->GetScreenRect();
	HippoUI_Global::GetIns()->GetUIRender()->DrawSprite(r,m_pButtonBackGroudTexture,HippoUi_Color(1,1,1,1));
	//draw button foreground
	if(m_pParent->GetState()==HBS_NORMAL)
	{
		HippoUI_Global::GetIns()->GetUIRender()->DrawSprite(r,m_pButtonForeGroudTexture,HippoUi_Color(1,1,1,1));
		//draw text
		HippoUI_Global::GetIns()->GetUIRender()->DrawString(r,str,HippoUi_Color(1,1,1,1));
	}
	else if(m_pParent->GetState()==HBS_PRESSED)
	{
		HippoUI_Rect r1=r;
		r1.left+=1;
		r1.bottom+=2;
		HippoUI_Global::GetIns()->GetUIRender()->DrawSprite(r1,m_pButtonForeGroudTexture,HippoUi_Color(1,1,1,1));
		HippoUI_Global::GetIns()->GetUIRender()->DrawString(r1,str,HippoUi_Color(1,0,0,1));
	}
	else
	{
		HippoUI_Rect r1=r;
		r1.left-=1;
		r1.bottom-=2;
		//draw text
		HippoUI_Global::GetIns()->GetUIRender()->DrawSprite(r1,m_pButtonForeGroudTexture,HippoUi_Color(1,1,1,1));
		HippoUI_Global::GetIns()->GetUIRender()->DrawString(r1,str,HippoUi_Color(1,0,1,1));
	}
	
	
}

void HippoUI_Button_DxutAppearance::Init()
{
	m_pButtonBackGroudTexture=HippoUI_Global::GetIns()->GetResManager()->GetTextureProxy(BUTTON_BG);
	m_pButtonForeGroudTexture=HippoUI_Global::GetIns()->GetResManager()->GetTextureProxy(BUTTON_FG);
}