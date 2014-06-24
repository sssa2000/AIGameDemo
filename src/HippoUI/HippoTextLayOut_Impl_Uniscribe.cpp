#include "HippoTextLayOut_Impl_Uniscribe.h"
#include <usp10.h>
#include <tchar.h>
#pragma comment(lib, "usp10.lib") //use usp lib


HippoTextLayOut_Impl_Uniscribe::HippoTextLayOut_Impl_Uniscribe()
{

}

HippoTextLayOut_Impl_Uniscribe::~HippoTextLayOut_Impl_Uniscribe()
{
	if( m_ups_analysis )
		ScriptStringFree( &m_ups_analysis );
}

void HippoTextLayOut_Impl_Uniscribe::Init(HDC fontdc)
{
	m_font_dc=fontdc;
	m_ups_analysis=0;
}

void HippoTextLayOut_Impl_Uniscribe::InsertChar(int pos,WCHAR c)
{
	m_data.insert(pos,1,c);
	AnalyseString();
}

void HippoTextLayOut_Impl_Uniscribe::InsertChar(int pos,const WCHAR* str)
{
	m_data.insert(pos,str);
	AnalyseString();
}

void HippoTextLayOut_Impl_Uniscribe::EraseChar(int beg,int end)
{
	m_data.erase(beg,end-beg+1);
	AnalyseString();
}

void HippoTextLayOut_Impl_Uniscribe::EraseChar(int pos)
{
	m_data.erase(pos,1);
	AnalyseString();
}

//��ȡ������Ļλ�ã�x��
//����ֵ-1��ʾ��ȡʧ�ܣ�>=0��ֵ��ʾ�ɹ�
int HippoTextLayOut_Impl_Uniscribe::GetXPos_Screen(int logicPos,bool bFinishSide)
{
	//AnalyseString();
	int trailing=bFinishSide?1:0;
	int res_ScreenXPos=0;
	HRESULT res=ScriptStringCPtoX(m_ups_analysis, logicPos, trailing, &res_ScreenXPos) ;
	if(FAILED(res))
	{
		return -1;
	}
	return res_ScreenXPos;
}

//����������Ļλ�� ��ȡ�����߼�λ��
int HippoTextLayOut_Impl_Uniscribe::GetXPos_Logic(int screenXPos)
{
	int res_LogicXPos=0;
	int trailing=0;
	HRESULT res=ScriptStringXtoCP(m_ups_analysis,screenXPos,&res_LogicXPos,&trailing);
	if(FAILED(res))
	{
		return 0;
	}
	int len=m_data.length();
	
	if( res_LogicXPos == -1 && trailing == TRUE )//���λ��С���κ��ַ���λ��
	{
		/*
		from msdn:
			If the x coordinate is before the beginning of the line, 
			the function retrieves -1 for piCh and 1 for piTrailing, 
			indicating the trailing edge of the nonexistent character before the line
		*/
		return -1;
	}
	else if( res_LogicXPos > len && trailing == FALSE ) //����ַ������ַ������һ��
	{
		/*
		from msdn:
			If the x coordinate is after the end of the line, 
			the function retrieves for piCh the first index 
			beyond the length of the line and 0 for piTrailing. 
			The 0 value indicates the leading edge of the nonexistent character after the line.
		*/
		res_LogicXPos = len;
	}

	return res_LogicXPos;
}

void HippoTextLayOut_Impl_Uniscribe::SetTextContent(const WCHAR* str)
{
	m_data=str;
	AnalyseString();//���¼����Ű�
}

const WCHAR* HippoTextLayOut_Impl_Uniscribe::GetTextContent()
{
	return m_data.c_str();
}
unsigned int HippoTextLayOut_Impl_Uniscribe::GetTextLength()
{
	return m_data.length();
}

void HippoTextLayOut_Impl_Uniscribe::AnalyseString()
{
	if( m_ups_analysis )
		ScriptStringFree( &m_ups_analysis );

	SCRIPT_CONTROL			ScriptControl = { 0 };
	SCRIPT_STATE			ScriptState   = { 0 };
	
	HRESULT hr = ScriptStringAnalyse( 
		m_font_dc,
		m_data.c_str(),
		m_data.length(),
		2*m_data.length(),
		-1,
		SSA_BREAK | SSA_GLYPHS | SSA_FALLBACK | SSA_LINK,
		0,
		&ScriptControl,
		&ScriptState,
		0,
		0,
		0,
		&m_ups_analysis );

}

void HippoTextLayOut_Impl_Uniscribe::ReplaceStrText(int beg,int end,WCHAR c)
{
	m_data.replace(beg,end-beg-1+1,1,c); //-1������end�����ʾ����һ���ַ������λ�ã�+1������length��idx�Ļ���
	AnalyseString();//���¼����Ű�
}

void HippoTextLayOut_Impl_Uniscribe::ReplaceStrText(int beg,int end,const WCHAR* c)
{
	m_data.replace(beg,end-beg-1+1,c);
	AnalyseString();//���¼����Ű�
}