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
	//�����ؼ��Ŀ�ȼ�ȥ�߿򣬼�ȥ�߿���ı�֮��Ŀհ׼���
	return ((HippoUI_LineEdit_DxutAppearance*)((m_pAppearance.get())))->GetTextAreaWidth();
}

MSG_HANDLE_RES HippoUI_LineEdit::OnMouseLeftDown(WPARAM wParam, LPARAM lParam)
{
	/*
	1���������λ�� �жϴ�����
	2������ǵ����ڱ��ؼ��ڣ�������Ӧ�÷��õ�λ��
	3���޸�m_nSelStartIdx
	4����¼״̬��׼��drag�Ĳ���
	*/
	//���µ�λ�ã���������ϵ�£�
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
	//��������ѡ��ʱ��������backspace����delete����Ϊ���ǣ���ѡ������ɾ������
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

	default://���������ַ�
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
		CaretDstLogicPos=��꽫Ҫ���õ��߼�λ��
		CaretDstScreenPos_L=��꽫Ҫ���õ���Ļλ�õ����Ե
		CaretDstScreenPos_R=��꽫Ҫ���õ���Ļλ�õ��ұ�Ե
		
		CaretCurLogicPos=����ƶ�ǰ���߼�λ��
		CaretCurScreenPos_L=����ƶ�ǰ����Ļλ�õ����Ե

		��CaretDstScreenPos_R<m_1stCharIdx_Logic��m_1stCharIdx_LogicҪ�����ƶ�
		��CaretDstScreenPos_L>m_1stCharIdx_Logic+�ı�����Ŀ��ʱ��m_1stCharIdx_LogicҪ�����ƶ�
	*/

	

	int CaretDstScreenPos_L=GetCaretLeadingPos_Screen(CaretDstLogicPos);
	int CaretDstScreenPos_F=GetCaretFinishPos_Screen(CaretDstLogicPos);
	if(CaretDstLogicPos > (int)(len-1)) //���λ���ַ�����ĩβ
	{
		CaretDstScreenPos_F=GetCaretFinishPos_Screen(len);
		m_data.m_LogicCaretPos=len-1;
	}
	int m_1stCharLeadingPos_Scene=GetCaretLeadingPos_Screen(m_1stCharIdx_Logic);
	int CaretCurScreenPos_L=GetCaretLeadingPos_Screen(CaretCurLogicPos);
	int cur_total=m_1stCharLeadingPos_Scene + GetTextAreaWidth();
	if(CaretDstScreenPos_F > cur_total)
	{
		//m_1stCharIdx_LogicҪ�����ƶ�
		int offset=CaretDstScreenPos_F-GetTextAreaWidth();
		m_1stCharIdx_Logic=m_pTextLayOut->GetXPos_Logic(offset);
		
		//����Բ���������offsetת��Ϊ��Ļ�ռ���ֵ�������ڰ���ַ���λ��
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
	//��������ѡ��ʱ��������backspace����delete����Ϊ���ǣ���ѡ������ɾ������
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
	//��Ҫ���������������ѡ������֡���ѡ�������
	//����Ϊ��ͳһ������ɾ��ѡ�е�����
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
	//�����ڴ棬���䳤һ��
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
		::MessageBoxA(0,"�򿪼��а�ʧ�ܣ��޷�����","HippoUIError",0);
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
	//���������ʲôλ�÷ſ���������drag״̬������
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

//��ȡѡ������ķ�Χ (�߼���Χ)
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
	//������ӻ����� ���ɼ��Ĳ�Ҫѡ��
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
	//�����λ�������ַ����ʼ��λ��ʱ�����λ��0���ַ������
	//�����λ�������ַ�����ĩβ��λ��ʱ���������λ���ַ����ұ�
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
//��ȡ���ֿ�ʼ��Xλ��(��Ļ�ռ�)
int HippoUI_LineEdit::GetTextStartXPos()
{
	return m_posX+GetBorderWidth()+GetSpacing();
}

//�Ƿ���ѡ�������
bool HippoUI_LineEdit::HasSelectText()
{
	return m_data.m_LogicCaretPos!=m_data.m_nSelStartIdx;
}

bool HippoUI_LineEdit::DeleteSelectText()
{
	if(HasSelectText())
	{
		std::pair<int,int> range=GetSelectTextRange_Logic();
		m_pTextLayOut->EraseChar(range.first+1,range.second); //ע�� ��ʾ�Ϲ��λ��ÿ���ַ����Ҳ࣬��������Ҫ-1
		SetLogicCaretPos(range.first); //ɾ���󣬹��Ӧ�÷ŵ�ѡ�����ֵ���С��λ��
		SyncSelectPosWithCaretPos();
		return true;
	}
	return false;
}

//�õ�������Ļλ�ã��������λ���ַ�����βʱ�����������
int HippoUI_LineEdit::GetCaretLeadingPos_Screen(int logicPos)
{
	int res=0;
	int len=m_pTextLayOut->GetTextLength();
	if(logicPos==0) //���λ���ַ�����
		return 0;

	int _tpos=logicPos;
	if(_tpos>=len) //β��
		_tpos=len-1;
	return m_pTextLayOut->GetXPos_Screen(_tpos,false);
}

int HippoUI_LineEdit::GetCaretFinishPos_Screen(int logicPos)
{
	int res=0;
	int len=m_pTextLayOut->GetTextLength();
	if(logicPos==0) //���λ���ַ�����
		return m_pTextLayOut->GetXPos_Screen(0,true);

	int _tpos=logicPos;
	if(_tpos>=len) //β��
		_tpos=len-1;
	return m_pTextLayOut->GetXPos_Screen(_tpos,true);
}