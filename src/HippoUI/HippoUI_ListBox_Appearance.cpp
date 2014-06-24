#include "HippoUI_ListBox_Appearance.h"
#include "HippoUI_ListBox.h"
#include "HippoUI_Global.h"
#include "HippoUI_Render.h"
#include "HippoUI_Define.h"

HippoUI_ListBox_DxutAppearance::HippoUI_ListBox_DxutAppearance(HippoUI_ListBox* parent):
m_pParent(parent)
{
	Init();
}

HippoUI_ListBox_DxutAppearance::~HippoUI_ListBox_DxutAppearance()
{

}

void HippoUI_ListBox_DxutAppearance::Render(float escape)
{
	HippoUI_Rect listframe=m_pParent->GetScreenRect();
	HippoUI_Global::GetIns()->GetUIRender()->DrawRect(listframe,HippoUi_Color::White());
	//HippoUI_Global::GetIns()->GetUIRender()->DrawSprite(listframe,m_pListBoxFrameTexture,HippoUi_Color(1,1,1,1));

	//draw all item
	for(int idx=0;idx<m_pParent->GetItemNum();++idx)
	{
		HippoUI_ListBoxItem* pItem=m_pParent->GetItem(idx);
		if(!pItem)
			break;

		const std::string& str=pItem->GetText();
		HippoUI_Global::GetIns()->GetUIRender()->DrawString(GetLineRect(idx),str.c_str(),HippoUi_Color::Black(),TEXT_AG_LEFT);
	}
}

void HippoUI_ListBox_DxutAppearance::Init()
{
	m_pListBoxFrameTexture=HippoUI_Global::GetIns()->GetResManager()->GetTextureProxy(LISTBOX_FRAME);
}

HippoUI_Rect HippoUI_ListBox_DxutAppearance::GetLineRect(int lineIdx)
{
	HippoUI_Rect r=m_pParent->GetScreenRect();
	r.top=r.top + lineIdx * m_pParent->GetItemHeight();
	r.bottom=r.top + m_pParent->GetItemHeight();
	return r;
}