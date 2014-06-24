#include "HippoUI_Factory.h"
#include "HippoUI_Wnd.h"
#include "HippoUI_Label.h"
#include "HippoUI_Button.h"
#include "HippoUI_LineEdit.h"
#include "HippoUI_ListBox.h"

HippoUI_Wnd* HippoUI_Factory::CreateWnd(int id)
{
	return new HippoUI_Wnd(id,0);
}

HippoUI_Label* HippoUI_Factory::CreateLabel(int id,HippoUI_Wnd* pParent)
{
	return new HippoUI_Label(id,pParent);
}

HippoUI_Button* HippoUI_Factory::CreateButton(int id,HippoUI_Wnd* pParent)
{
	return new HippoUI_Button(id,pParent);
}

HippoUI_LineEdit* HippoUI_Factory::CreateLineEdit(int id,HippoUI_Wnd* pParent)
{
	return new HippoUI_LineEdit(id,pParent);
}

HippoUI_ListBox* HippoUI_Factory::CreateListBox(int id,HippoUI_Wnd* pParent)
{
	return new HippoUI_ListBox(id,pParent);
}