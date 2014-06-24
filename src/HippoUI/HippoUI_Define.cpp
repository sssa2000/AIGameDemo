#include "HippoUI_Factory.h"
#include <string>
#include <vector>

const char* strUISTATE[UISTATE_END]={
	"LABEL_NORMAL",
	"BUTTON_BG",
	"BUTTON_FG",
	"LINEEDIT_0",
	"LINEEDIT_1",
	"LINEEDIT_2",
	"LINEEDIT_3",
	"LINEEDIT_4",
	"LINEEDIT_5",
	"LINEEDIT_6",
	"LINEEDIT_7",
	"LINEEDIT_8",
	"LISTBOX_FRAME"
};


//help fun
void split_string(std::vector<std::string>&resStr,const std::string& str, const std::string& token)
{
	resStr.clear();

	unsigned int sl = token.length();
	unsigned int pos=0;
	for(;;)
	{
		unsigned int newpos = str.find(token, pos);
		if(newpos==std::string::npos)
		{
			if(pos!=str.length())
			{
				resStr.push_back(str.substr(pos));
			}
			break;
		}
		else
		{
			if(newpos!=pos)
			{
				resStr.push_back(str.substr(pos, newpos-pos));
			}
		}
		pos = newpos+sl;
	}
}

//”√∂∫∫≈∑÷∏Ó
void HippoUI_Rect::SetRectFromString(const char* str_rect)
{
	if(!str_rect)
	{
		left=right=0;
		top=bottom=0;
		return;
	}
	std::vector<std::string> points;

	split_string(points,std::string(str_rect),std::string(","));
	std::string& s_l=points.at(0);
	left=atoi(s_l.c_str());
	
	std::string& s_t=points.at(1);
	top=atoi(s_t.c_str());

	std::string& s_r=points.at(2);
	right=atoi(s_r.c_str());

	std::string& s_b=points.at(3);
	bottom=atoi(s_b.c_str());

}

bool HippoUI_Rect::ContainPoint(int x,int y)
{
	bool b1=x>left && x<right;
	bool b2=y>top && y<bottom;

	return b1 && b2;
}