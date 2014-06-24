#include "HippoUI_HelpFun.h"
#include "HippoUI_Include.h"

bool IsLeftKeyDown(HippoUI_MouseMessage& _event)
{
	return _event.leftkey_state==1;
}
bool IsLeftKeyUp(HippoUI_MouseMessage& _event)
{
	return _event.leftkey_state==2;
}