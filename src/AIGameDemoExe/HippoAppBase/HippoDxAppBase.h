/********************************************************************
	created:	2014/03/07
	created:	7:3:2014   15:49
	filename: 	C:\Users\sssa\Desktop\HippoDxAppDemo\HippoAppBase\HippoDxAppBase.h
	file path:	C:\Users\sssa\Desktop\HippoDxAppDemo\HippoAppBase
	file base:	HippoDxAppBase
	file ext:	h
	author:		sssa2000
	
	purpose:	��װ��dx���õĹ��ܣ�
	
	1���豸�������ͷ�
	2���豸��ʧ�Ĵ���
	3��OnSize��ȫ�������ڵ��л�
*********************************************************************/
#pragma once
#include "HippoAppBase.h"
#include "HippoD3d9Device.h"
#include <memory>
class HippoDxAppBase:public HippoAppBase
{
public:
	HippoDxAppBase(const char* name,unsigned int w,unsigned int h,unsigned int left,unsigned int top);
	~HippoDxAppBase();

	virtual void Init();
	HippoD3d9Device* GetDevice(){return m_pDevice.get();}
	
protected:
	void ProcessLostDevice();
	void ProcessResetDevice();
	virtual void OnLostDevice(){};
	virtual void OnResetDevice(){};
	void SwitchWindowMode(bool bWindowed,unsigned int newWidth,unsigned int newHeight);
	void RegisterCallBack();
	int OnWndSize(HippoSizeEvent& e);
	void ChangeSize(int new_w, int new_h, int m_bwinMode);
	int OnKeyDown(unsigned int key);
	virtual void InitDevice();
	std::tr1::shared_ptr<HippoD3d9Device> m_pDevice;
private:
	
	bool m_bwinMode;
	WINDOWINFO m_wndInfo;
};
