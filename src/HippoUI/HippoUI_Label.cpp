#include "HippoUI_Label.h"
#include "HippoUI_Label_Appearance.h"


HippoUI_Label::HippoUI_Label(int id,HippoUI_Control* pParent):
HippoUI_Control(id,pParent)
{
	Init();
}
HippoUI_Label::~HippoUI_Label()
{
	CleanUp();
}


void HippoUI_Label::Render(float escape)
{
	m_pAppearance->Render(escape);
}

void HippoUI_Label::Init()
{
	m_pAppearance=make_shared<HippoUI_Label_DxutAppearance>(this);
}
void HippoUI_Label::CleanUp()
{
}

