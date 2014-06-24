#include "HippoUI_LineEdit.h"
#include "HippoUI_LineEdit_Appearance.h"
#include "HippoUI_Include.h"
#include "HippoUI_HelpFun.h"
#include "HippoUI_Wnd.h"
#include "HippoTextLayOut_Impl_Uniscribe.h"
#include "HippoUI_Global.h"
#include "HippoUI_Render.h"
#include <assert.h>
#include <functional>

HippoUI_LineEdit::HippoUI_LineEdit(int id,HippoUI_Control* pParent):
HippoUI_Control(id,pParent)
{
	Init();
}

HippoUI_LineEdit::~HippoUI_LineEdit()
{

}

int HippoUI_LineEdit::GetTextAreaWidth()
{
	//整个控件的宽度减去边框，减去边框和文本之间的空白即可
	return ((HippoUI_LineEdit_DxutAppearance*)((m_pAppearance.get())))->GetTextAreaWidth();
}

MSG_HANDLE_RES HippoUI_LineEdit::OnMouseLeftDown(WPARAM wParam, LPARAM lParam)
{
	/*
	1、根据鼠标位置 判断处理焦点
	2、如果是单击在本控件内，计算光标应该放置的位置
	3、修改m_nSelStartIdx
	4、记录状态，准备drag的操作
	*/
	//按下的位置（窗口坐标系下）
	int x=( int )LOWORD( lParam );
	int y=( int )HIWORD( lParam );
	if(ScreenPointInControl(x,y))
	{
		HippoUI_Wnd* pwnd=(HippoUI_Wnd*)m_pParent;
		pwnd->SetFocus(this);
		int screen_pos=x-GetTextStartXPos();
		int _1st_screen_pos=m_pTextLayOut->GetXPos_Screen(m_1stCharIdx_Logic);
		SetLogicCaretPos(m_pTextLayOut->GetXPos_Logic(screen_pos+_1st_screen_pos));
		SyncSelectPosWithCaretPos();
		m_data.m_bLeftDrag=true;

		return HAD_HANDLE;
	}
	return NOT_HANDLE;

}

MSG_HANDLE_RES HippoUI_LineEdit::ProcessDeleteKey(WPARAM wParam, LPARAM lParam)
{
	//当有文字选中时，不管是backspace还是delete，行为都是：将选中文字删掉即可
	if(!DeleteSelectText())
	{
		if(m_data.m_LogicCaretPos<(int)(m_pTextLayOut->GetTextLength()))
		{
			int delete_pos=m_data.m_LogicCaretPos;
			m_pTextLayOut->EraseChar(delete_pos);
		}
	}

	return HAD_HANDLE;
}

MSG_HANDLE_RES HippoUI_LineEdit::ProcessHomeKey(WPARAM wParam, LPARAM lParam)
{
	SetLogicCaretPos(FIRST_LOGIC_POS);
	bool bShiftDown=GetKeyState(VK_SHIFT)<0;
	if(!bShiftDown)
		SyncSelectPosWithCaretPos();
	return HAD_HANDLE;
}

MSG_HANDLE_RES HippoUI_LineEdit::ProcessEndKey(WPARAM wParam, LPARAM lParam)
{
	SetLogicCaretPos(m_pTextLayOut->GetTextLength());
	bool bShiftDown=GetKeyState(VK_SHIFT)<0;
	if(!bShiftDown)
		SyncSelectPosWithCaretPos();
	return HAD_HANDLE;
}


MSG_HANDLE_RES HippoUI_LineEdit::OnKeyDown(WPARAM wParam, LPARAM lParam)
{
	WCHAR k=( WCHAR )wParam;
	switch(k)
	{

	case VK_BACK:
		return ProcessBackSpaceKey(wParam,lParam);
	case VK_DELETE:
		return ProcessDeleteKey(wParam,lParam);
	case VK_HOME:
		return ProcessHomeKey(wParam,lParam);
	case VK_END:
		return ProcessEndKey(wParam,lParam);

	case VK_LEFT:
	case VK_RIGHT:
		return ProcessLeftRightArrow(k,wParam,lParam);
	default:
		return HAD_HANDLE;

	}

	return HAD_HANDLE;
}

bool is_accept_char(WCHAR c)
{
	if (IsCharAlphaNumericW(c) || c==VK_SPACE)
	{
		return true;
	}
	return false;
	
}

MSG_HANDLE_RES HippoUI_LineEdit::OnKeyChar(WPARAM wParam, LPARAM lParam)
{
	WCHAR k=( WCHAR )wParam;
	switch(k)
	{
	case VK_RETURN:
		return ProcessEnterKeyDown(wParam,lParam);
	case 1: //crtl+a
		return ProcessSelectAll(wParam,lParam);
	case 22://crtl+v
		return ProcessPast(wParam,lParam);
	case 24://crtl+x
		return ProcessCut(wParam,lParam);
	case VK_CANCEL://crtl+c
		return ProcessCopy(wParam,lParam);

	default://正常输入字符
		{
			if(!is_accept_char(k))
				return HAD_HANDLE;
			DeleteSelectText();
			m_pTextLayOut->InsertChar(m_data.m_LogicCaretPos+1,k);

			MoveCaretPos(1);
			SyncSelectPosWithCaretPos();
			return HAD_HANDLE;
		}
	}
}
MSG_HANDLE_RES HippoUI_LineEdit::ProcessEnterKeyDown(WPARAM wParam, LPARAM lParam)
{
	if(!m_enterdown_event._Empty())
	{
		m_enterdown_event(this);
	}
	return HAD_HANDLE;
}

void HippoUI_LineEdit::SetLogicCaretPos(int pos)
{
	unsigned int len=m_pTextLayOut->GetTextLength();

	int CaretCurLogicPos=m_data.m_LogicCaretPos;
	int CaretDstLogicPos=pos;
	m_data.m_LogicCaretPos=pos;
	if(pos < 0)
	{
		CaretDstLogicPos=0;
		m_data.m_LogicCaretPos=FIRST_LOGIC_POS;
	}
		
	/*
		CaretDstLogicPos=光标将要放置的逻辑位置
		CaretDstScreenPos_L=光标将要放置的屏幕位置的左边缘
		CaretDstScreenPos_R=光标将要放置的屏幕位置的右边缘
		
		CaretCurLogicPos=光标移动前的逻辑位置
		CaretCurScreenPos_L=光标移动前的屏幕位置的左边缘

		当CaretDstScreenPos_R<m_1stCharIdx_Logic，m_1stCharIdx_Logic要向左移动
		当CaretDstScreenPos_L>m_1stCharIdx_Logic+文本区域的宽度时，m_1stCharIdx_Logic要向右移动
	*/

	

	int CaretDstScreenPos_L=GetCaretLeadingPos_Screen(CaretDstLogicPos);
	int CaretDstScreenPos_F=GetCaretFinishPos_Screen(CaretDstLogicPos);
	if(CaretDstLogicPos > (int)(len-1)) //光标位于字符串最末尾
	{
		CaretDstScreenPos_F=GetCaretFinishPos_Screen(len);
		m_data.m_LogicCaretPos=len-1;
	}
	int m_1stCharLeadingPos_Scene=GetCaretLeadingPos_Screen(m_1stCharIdx_Logic);
	int CaretCurScreenPos_L=GetCaretLeadingPos_Screen(CaretCurLogicPos);
	int cur_total=m_1stCharLeadingPos_Scene + GetTextAreaWidth();
	if(CaretDstScreenPos_F > cur_total)
	{
		//m_1stCharIdx_Logic要向右移动
		int offset=CaretDstScreenPos_F-GetTextAreaWidth();
		m_1stCharIdx_Logic=m_pTextLayOut->GetXPos_Logic(offset);
		
		//处理圆整情况，即offset转化为屏幕空间后的值可能落在半个字符的位置
 		int testPos=GetCaretLeadingPos_Screen(m_1stCharIdx_Logic);
 		if(testPos<offset)
 			m_1stCharIdx_Logic+=1;
	}
	else if(CaretDstScreenPos_L < m_1stCharLeadingPos_Scene)
	{
		m_1stCharIdx_Logic=CaretDstLogicPos;
	}

}

void HippoUI_LineEdit::MoveCaretPos(int offset)
{
	SetLogicCaretPos(m_data.m_LogicCaretPos+offset);
}



void HippoUI_LineEdit::SyncSelectPosWithCaretPos()
{
	m_data.m_nSelStartIdx=m_data.m_LogicCaretPos;
}

MSG_HANDLE_RES HippoUI_LineEdit::ProcessLeftRightArrow(WCHAR c,WPARAM wParam, LPARAM lParam)
{
	MoveCaretPos(c==VK_LEFT?-1:1);

	bool bShiftDown=GetKeyState(VK_SHIFT)<0;

	if(!bShiftDown)
		SyncSelectPosWithCaretPos();
	return HAD_HANDLE;
}


MSG_HANDLE_RES HippoUI_LineEdit::ProcessBackSpaceKey(WPARAM wParam, LPARAM lParam)
{
	//当有文字选中时，不管是backspace还是delete，行为都是：将选中文字删掉即可
	if(!DeleteSelectText())
	{
		if(m_data.m_LogicCaretPos>FIRST_LOGIC_POS)
		{
			int delete_pos=m_data.m_LogicCaretPos;
			m_pTextLayOut->EraseChar(delete_pos);
			SetLogicCaretPos(delete_pos);
		}
	}
	SyncSelectPosWithCaretPos();

	return HAD_HANDLE;
}

MSG_HANDLE_RES HippoUI_LineEdit::ProcessSelectAll(WPARAM wParam, LPARAM lParam)
{
	SetLogicCaretPos(FIRST_LOGIC_POS);
	m_data.m_nSelStartIdx=m_pTextLayOut->GetTextLength();
	return HAD_HANDLE;
}

MSG_HANDLE_RES HippoUI_LineEdit::ProcessPast(WPARAM wParam, LPARAM lParam)
{
	//需要处理两种情况：有选择的文字、无选择的文字
	//所以为了统一处理，先删除选中的内容
	DeleteSelectText();
	if( OpenClipboard( NULL ) )
	{
		HANDLE handle = GetClipboardData( CF_UNICODETEXT );
		if( handle )
		{
			WCHAR* pwszText = ( WCHAR* )GlobalLock( handle );
			if( pwszText )
			{
				m_pTextLayOut->InsertChar(m_data.m_LogicCaretPos+1,pwszText);
				SetLogicCaretPos(m_pTextLayOut->GetTextLength());
				SyncSelectPosWithCaretPos();
				GlobalUnlock( handle );
			}
		}
		CloseClipboard();
	}

	return HAD_HANDLE;
}

MSG_HANDLE_RES HippoUI_LineEdit::ProcessCut(WPARAM wParam, LPARAM lParam)
{
	ProcessCopy(wParam,lParam);
	DeleteSelectText();
	return HAD_HANDLE;
}

HGLOBAL BeginWriteClipboard(int len)
{
	BOOL res=::EmptyClipboard();
	//分配内存，分配长一点
	HGLOBAL hBlock = GlobalAlloc(GMEM_MOVEABLE, sizeof(WCHAR)*(len+1));
	return hBlock;
}

void WriteClipboard(HGLOBAL& hBlock,int len,const WCHAR* content)
{
	WCHAR* pwszText = ( WCHAR* )GlobalLock( hBlock );
	if( pwszText )
	{
		CopyMemory( pwszText, content, len * sizeof( WCHAR ) );
		pwszText[len] = L'\0';
		GlobalUnlock( hBlock );
	}
	SetClipboardData( CF_UNICODETEXT, hBlock );
}

void EndWriteClipboard(HGLOBAL hBlock)
{
	CloseClipboard();
	if( hBlock )
		GlobalFree( hBlock );
}

MSG_HANDLE_RES HippoUI_LineEdit::ProcessCopy(WPARAM wParam, LPARAM lParam)
{
	if(FAILED(::OpenClipboard( NULL )))
	{
		::MessageBoxA(0,"打开剪切板失败，无法复制","HippoUIError",0);
		return HAD_HANDLE;
	}

	if( HasSelectText())
	{
		std::pair<int,int> _range=GetSelectTextRange_Logic();
		int len=_range.second - _range.first;
		HGLOBAL hBlock=BeginWriteClipboard(len);
		if( hBlock )
		{
			WriteClipboard(hBlock,len,m_pTextLayOut->GetTextContent()+_range.first);
		}
		EndWriteClipboard(hBlock);
	}
	return HAD_HANDLE;
}

MSG_HANDLE_RES HippoUI_LineEdit::OnMouseLeftUp(WPARAM wParam, LPARAM lParam)
{
	//不管鼠标在什么位置放开鼠标左键，drag状态都结束
	m_data.m_bLeftDrag=false;
	return NOT_HANDLE;
}
MSG_HANDLE_RES HippoUI_LineEdit::OnMouseMove(WPARAM wParam, LPARAM lParam)
{
	if(!m_data.m_bLeftDrag)
		return NOT_HANDLE;

	int x=( int )LOWORD( lParam );
	int y=( int )HIWORD( lParam );

	SetLogicCaretPos(m_pTextLayOut->GetXPos_Logic(x-GetTextStartXPos()));
	return HAD_HANDLE;
}

void HippoUI_LineEdit::Init()
{
	m_1stCharIdx_Logic=0;
	m_data.m_nBorder=5;
	m_data.m_nSpacing=4;
	m_data.m_LogicCaretPos=FIRST_LOGIC_POS;
	SyncSelectPosWithCaretPos();
	m_data.m_bLeftDrag=false;

	m_pAppearance=make_shared<HippoUI_LineEdit_DxutAppearance>(this);
	m_pTextLayOut=make_shared<HippoTextLayOut_Impl_Uniscribe>();
	HippoUI_Render* pRender=HippoUI_Global::GetIns()->GetUIRender();
	m_pTextLayOut->Init(pRender->GetFontDC());
	HippoUI_Wnd* pwnd=(HippoUI_Wnd*)m_pParent;

	m_MsgCon[WM_LBUTTONDOWN]=std::tr1::bind(&HippoUI_LineEdit::OnMouseLeftDown,this,std::placeholders::_1,std::placeholders::_2);
	m_MsgCon[WM_CHAR]=std::tr1::bind(&HippoUI_LineEdit::OnKeyChar,this,std::placeholders::_1,std::placeholders::_2);
	m_MsgCon[WM_MOUSEMOVE]=std::tr1::bind(&HippoUI_LineEdit::OnMouseMove,this,std::placeholders::_1,std::placeholders::_2);
	m_MsgCon[WM_LBUTTONUP]=std::tr1::bind(&HippoUI_LineEdit::OnMouseLeftUp,this,std::placeholders::_1,std::placeholders::_2);
	m_MsgCon[WM_KEYDOWN]=std::tr1::bind(&HippoUI_LineEdit::OnKeyDown,this,std::placeholders::_1,std::placeholders::_2);
	m_MsgCon[WM_SETCURSOR]=std::tr1::bind(&HippoUI_LineEdit::OnSetCursor,this,std::placeholders::_1,std::placeholders::_2);

}

MSG_HANDLE_RES HippoUI_LineEdit::OnSetCursor(WPARAM wParam, LPARAM lParam)
{
	::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_IBEAM)));
	return HAD_HANDLE;
}

//获取选择区域的范围 (逻辑范围)
std::pair<int,int> HippoUI_LineEdit::GetSelectTextRange_Logic()
{
	int sel_logic_min=min(m_data.m_nSelStartIdx,m_data.m_LogicCaretPos);
	int sel_logic_max=max(m_data.m_nSelStartIdx,m_data.m_LogicCaretPos);
	return std::make_pair(sel_logic_min,sel_logic_max);
}

std::pair<int,int> HippoUI_LineEdit::GetSelectTextRange()
{
	std::pair<int,int> range_logic=GetSelectTextRange_Logic();

	int sel_min=_GetScreenCaretXPos_impl(range_logic.first);
	int sel_max=_GetScreenCaretXPos_impl(range_logic.second);
	return std::make_pair(sel_min,sel_max);
	
}

std::pair<int,int> HippoUI_LineEdit::GetVisibleSelectTextRange()
{
	//处理可视化区域 不可见的不要选择
	std::pair<int,int> range_logic=GetSelectTextRange_Logic();
	if(range_logic.first!=FIRST_LOGIC_POS && range_logic.first< m_1stCharIdx_Logic)
		range_logic.first=m_1stCharIdx_Logic;

	int sel_min=_GetScreenCaretXPos_impl(range_logic.first);
	int sel_max=_GetScreenCaretXPos_impl(range_logic.second);

	int maxVisiblePos=GetTextAreaWidth();
	sel_max=min(sel_max,maxVisiblePos);
	return std::make_pair(sel_min,sel_max);
}

void HippoUI_LineEdit::CleanUp()
{

}

void HippoUI_LineEdit::Render(float escape)
{
	m_pAppearance->Render(escape);
}

void HippoUI_LineEdit::UpdateRects()
{
	m_pAppearance->UpdateRects();
}

bool HippoUI_LineEdit::HasFocus()
{
	HippoUI_Wnd* pwnd=(HippoUI_Wnd*)m_pParent;
	return pwnd->GetFocus()==this;
}
int HippoUI_LineEdit::_GetScreenCaretXPos_impl(int logicPos)
{
	//当光标位于整个字符串最开始的位置时，光标位于0号字符的左边
	//当光标位于整个字符串最末尾的位置时，光标总是位于字符的右边
	int pos=logicPos;
	bool bright_side=true;
	int len=m_pTextLayOut->GetTextLength();
	if(pos < 0)
	{
		pos=0;
		bright_side=false;
	}
	else if(pos >= len)
	{
		pos=len-1;
	}
	//pos-=m_1stCharIdx_Logic;
	//return m_pTextLayOut->GetXPos_Screen(pos,bright_side); 
	int _1stCharScreenPos=m_pTextLayOut->GetXPos_Screen(m_1stCharIdx_Logic); 
	int respos=m_pTextLayOut->GetXPos_Screen(pos,bright_side);
	return respos-_1stCharScreenPos; 
}

int HippoUI_LineEdit::GetScreenCaretXPos()
{
	return _GetScreenCaretXPos_impl(m_data.m_LogicCaretPos);
}

void HippoUI_LineEdit::SetText(const char* str)
{
	wstring tmpstr;
	int len=::MultiByteToWideChar(CP_ACP,0,str,-1,0,0);  
	tmpstr.resize(len);
	int res=::MultiByteToWideChar(CP_ACP,0,(LPCSTR)str,strlen(str),(LPWSTR)(tmpstr.c_str()),len);
	if(!res)
	{
		//convert failed
		assert(0);
	}
	m_pTextLayOut->SetTextContent(tmpstr.c_str());
}

const WCHAR* HippoUI_LineEdit::GetText()
{
	return m_pTextLayOut->GetTextContent();
}

std::string HippoUI_LineEdit::GetTextMultiByte()
{
	string tmpstr;
	const WCHAR* pUnicode=GetText();
	int needsz=::WideCharToMultiByte(CP_ACP,0,pUnicode,-1,0,0,0,0);
	tmpstr.resize(needsz);
	::WideCharToMultiByte(CP_ACP,0,pUnicode,-1,(LPSTR)(tmpstr.c_str()),needsz,0,0);
	return tmpstr;
}
//获取文字开始的X位置(屏幕空间)
int HippoUI_LineEdit::GetTextStartXPos()
{
	return m_posX+GetBorderWidth()+GetSpacing();
}

//是否有选择的文字
bool HippoUI_LineEdit::HasSelectText()
{
	return m_data.m_LogicCaretPos!=m_data.m_nSelStartIdx;
}

bool HippoUI_LineEdit::DeleteSelectText()
{
	if(HasSelectText())
	{
		std::pair<int,int> range=GetSelectTextRange_Logic();
		m_pTextLayOut->EraseChar(range.first+1,range.second); //注意 显示上光标位于每个字符的右侧，所以这里要-1
		SetLogicCaretPos(range.first); //删除后，光标应该放到选中文字的最小的位置
		SyncSelectPosWithCaretPos();
		return true;
	}
	return false;
}

//得到光标的屏幕位置（处理当光标位于字符串首尾时的特殊情况）
int HippoUI_LineEdit::GetCaretLeadingPos_Screen(int logicPos)
{
	int res=0;
	int len=m_pTextLayOut->GetTextLength();
	if(logicPos==0) //光标位于字符串首
		return 0;

	int _tpos=logicPos;
	if(_tpos>=len) //尾部
		_tpos=len-1;
	return m_pTextLayOut->GetXPos_Screen(_tpos,false);
}

int HippoUI_LineEdit::GetCaretFinishPos_Screen(int logicPos)
{
	int res=0;
	int len=m_pTextLayOut->GetTextLength();
	if(logicPos==0) //光标位于字符串首
		return m_pTextLayOut->GetXPos_Screen(0,true);

	int _tpos=logicPos;
	if(_tpos>=len) //尾部
		_tpos=len-1;
	return m_pTextLayOut->GetXPos_Screen(_tpos,true);
}