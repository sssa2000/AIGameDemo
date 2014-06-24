/********************************************************************
	created:	2012/11/11
	created:	11:11:2012   21:10
	filename: 	F:\Git\ClassicHippoTestBox\Common\HippoUI\HippoUI_LineEdit.h
	file path:	F:\Git\ClassicHippoTestBox\Common\HippoUI
	file base:	HippoUI_LineEdit
	file ext:	h
	author:		sssa2000
	
	purpose:	
*********************************************************************/
#pragma once

#include "HippoUI_Control.h"
#include "HippoUI_Factory.h"
#include <string>
#include <functional>
#include <memory>

class IHippoUI_Appearance;
class IHippoTextLayOut;

//�����߼�λ�õĵ�һ��λ��=-1
#define FIRST_LOGIC_POS -1

//����ؼ�������
struct _LineEdit_Data
{
	int m_nBorder;      // Border of the window
	int m_nSpacing;     // Spacing between the text and the edge of border
	int m_LogicCaretPos; //�����߼�λ��(���ڵڼ����ַ���)
	int m_nSelStartIdx; //ѡ������ʼ���ַ���û��ѡ��ʱ��Ӧ�õ��ڹ��λ��
	bool m_bLeftDrag;
};

class HippoUI_LineEdit:public HippoUI_Control
{
	friend class HippoUI_Factory;
public:
	~HippoUI_LineEdit();
	virtual void UpdateRects();

	void Render(float escape)override;
	void SetText(const char* str);
	const WCHAR* GetText();
	std::string GetTextMultiByte();
	int GetBorderWidth(){return m_data.m_nBorder;}
	int GetSpacing(){return m_data.m_nSpacing;}
	int GetTextAreaWidth();

	//int GetLogicCaretPos(){return m_data.m_LogicCaretPos;}
	//void SetLogicCaretPos(int i){m_data.m_LogicCaretPos=i;}
	int GetScreenCaretXPos();

	bool HasFocus();
	
	//�Ƿ���ѡ�������
	bool HasSelectText();

	//��ȡ��һ���ɼ����ַ�����
	int GetFirstRenderCharIdx(){return m_1stCharIdx_Logic;}
	//��ȡѡ������ķ�Χ (��Ļ�ռ�)
	std::pair<int,int> GetSelectTextRange();
	std::pair<int,int> GetVisibleSelectTextRange();

	//��Ӧ�س����¼�
	typedef std::tr1::function<void (HippoUI_LineEdit*)> HippoUI_LineEdit_EnterDown;
	HippoUI_LineEdit_EnterDown& GetEnterKeyDownEvent(){return m_enterdown_event;}
	void ConnectClickEvent(HippoUI_LineEdit_EnterDown& f){m_enterdown_event=f;}

	MSG_HANDLE_RES OnMouseLeftDown(WPARAM wParam, LPARAM lParam);
	MSG_HANDLE_RES OnMouseLeftUp(WPARAM wParam, LPARAM lParam);
	MSG_HANDLE_RES OnMouseMove(WPARAM wParam, LPARAM lParam);
	MSG_HANDLE_RES OnKeyChar(WPARAM wParam, LPARAM lParam);
	MSG_HANDLE_RES OnKeyDown(WPARAM wParam, LPARAM lParam);
	MSG_HANDLE_RES OnSetCursor(WPARAM wParam, LPARAM lParam);

	MSG_HANDLE_RES ProcessEnterKeyDown(WPARAM wParam, LPARAM lParam);
	MSG_HANDLE_RES ProcessBackSpaceKey(WPARAM wParam, LPARAM lParam);
	MSG_HANDLE_RES ProcessSelectAll(WPARAM wParam, LPARAM lParam);
	MSG_HANDLE_RES ProcessPast(WPARAM wParam, LPARAM lParam);
	MSG_HANDLE_RES ProcessCut(WPARAM wParam, LPARAM lParam);
	MSG_HANDLE_RES ProcessCopy(WPARAM wParam, LPARAM lParam);
	MSG_HANDLE_RES ProcessLeftRightArrow(WCHAR c,WPARAM wParam, LPARAM lParam);
	MSG_HANDLE_RES ProcessDeleteKey(WPARAM wParam, LPARAM lParam);
	MSG_HANDLE_RES ProcessHomeKey(WPARAM wParam, LPARAM lParam);
	MSG_HANDLE_RES ProcessEndKey(WPARAM wParam, LPARAM lParam);

protected:
	int _GetScreenCaretXPos_impl(int logicPos);
	bool IsCaretAtFirstPos();
	//�õ�������Ļλ�ã��������λ���ַ�����βʱ�����������
	int GetCaretLeadingPos_Screen(int logicPos);
	int GetCaretFinishPos_Screen(int logicPos);
	bool DeleteSelectText(); //�����Ƿ���ѡ�������
	//��ȡѡ������ķ�Χ (�߼���Χ)
	std::pair<int,int> GetSelectTextRange_Logic();
	void SetLogicCaretPos(int pos);
	void MoveCaretPos(int offset);
	void SyncSelectPosWithCaretPos();
	void Init();
	void CleanUp();
	
	//��ȡ���ֿ�ʼ��Xλ��(��Ļ�ռ�)
	int GetTextStartXPos();

	int m_1stCharIdx_Logic;//��һ����Ⱦ���ַ�����
	//int m_1stCharLeadingPos_Scene;//��һ����Ⱦ���ַ���Ļλ��(Leading Pos)

	_LineEdit_Data m_data;
	HippoUI_LineEdit_EnterDown m_enterdown_event;
	std::tr1::shared_ptr<IHippoUI_Appearance> m_pAppearance;
	std::tr1::shared_ptr<IHippoTextLayOut> m_pTextLayOut;
private:
	HippoUI_LineEdit(int id,HippoUI_Control* pParent);
};