#include "HippoUI_ListBox.h"
#include "HippoUI_ListBox_Appearance.h"

HippoUI_ListBox::HippoUI_ListBox(int id,HippoUI_Control* pParent):
HippoUI_Control(id,pParent)
{
	Init();
}

HippoUI_ListBox::~HippoUI_ListBox()
{
	CleanUp();
}

void HippoUI_ListBox::Render(float escape)
{
	m_pAppearance->Render(escape);
}

void HippoUI_ListBox::Init()
{
	m_nBorder=0;
	m_nMargin=0;
	m_nItemHeight=0;
	m_pAppearance=make_shared<HippoUI_ListBox_DxutAppearance>(this);
}

void HippoUI_ListBox::CleanUp()
{
	auto itr=m_all_item.begin();
	auto itrend=m_all_item.end();
	while (itr!=itrend)
	{
		HippoUI_ListBoxItem* p=*itr;
		delete p;
		++itr;
	}
	m_all_item.clear();



}


void HippoUI_ListBox::AddItem(const char* str)
{
	m_all_item.push_back(new HippoUI_ListBoxItem(str,true));
}

HippoUI_ListBoxItem* HippoUI_ListBox::GetItem(int i)
{
	if((int)(m_all_item.size())>i && i>=0)
		return m_all_item.at(i);
	return 0;
}

int HippoUI_ListBox::GetItemHeight()
{
	return 18; //暂时写死，因为render中创建的d3dxfont 高度是16
}