#include "HippoDxAppBase.h"
#include <functional>
#include "HippoWindow.h"
#include "ErrReport.h"
HippoDxAppBase::HippoDxAppBase(const char* name,unsigned int w,unsigned int h,unsigned int left,unsigned int top):
HippoAppBase(name,w,h,left,top)
{
	m_bwinMode=true;
}

HippoDxAppBase::~HippoDxAppBase()
{

}

void HippoDxAppBase::Init()
{
	InitDevice();
	RegisterCallBack();



	HippoAppBase::Init();

}




void HippoDxAppBase::InitDevice()
{
	m_pDevice=std::tr1::make_shared<HippoD3d9Device>();
	m_pDevice->Init(m_pWnd->GetHwnd(), m_pWnd->GetWidth(), m_pWnd->GetHeight());

	//将窗口的信息保存下来
	m_wndInfo.cbSize = sizeof(WINDOWINFO);
	GetWindowInfo(m_pWnd->GetHwnd(), &m_wndInfo);

}

void HippoDxAppBase::RegisterCallBack()
{
	//register on size call back
	WndSizeCallback scb=bind(&HippoDxAppBase::OnWndSize,this,std::placeholders::_1);
	m_pWnd->RegisterWndSizeEvent(scb);

	//register keydown
	KeyCallback kcb=bind(&HippoDxAppBase::OnKeyDown,this,std::placeholders::_1);
	m_pWnd->RegisterKeyDownEvent(kcb);
}

void HippoDxAppBase::ProcessLostDevice()
{
	//这里需要device的使用者先把该释放的释放掉
	OnLostDevice();
	m_pDevice->ProcessDeviceLost();
	
}

void HippoDxAppBase::ProcessResetDevice()
{
	OnResetDevice();
}
void HippoDxAppBase::ChangeSize(int new_w, int new_h, int m_bwinMode)
{
	m_pDevice->FillD3dParameter(new_w, new_h, m_bwinMode==1);
	ProcessLostDevice();
	ProcessResetDevice();
	m_pDevice->SetViewport(new_w, new_h);
}
int HippoDxAppBase::OnWndSize(HippoSizeEvent& e)
{
	//get new size
	int new_w=e.w;
	int new_h=e.h;

	ChangeSize(new_w, new_h, m_bwinMode);

	return 1;
}

int HippoDxAppBase::OnKeyDown(unsigned int key)
{
	SHORT bCtrlDown=GetKeyState(VK_CONTROL);
	if(key==VK_RETURN)
	{
		SwitchWindowMode(!m_bwinMode,1920,1080);
	}
	return 1;
}

void HippoDxAppBase::SwitchWindowMode(bool bWindowed,unsigned int newWidth,unsigned int newHeight)
{
	//if(m_bwinMode==bWindowed)
	//	return;

	bool oldState=m_bwinMode;
	m_bwinMode=bWindowed;

	if(!m_bwinMode)
	{
		//从窗口切换到全屏
		DEVMODE dm;
		dm.dmSize = sizeof(DEVMODE);
		dm.dmDriverExtra = 0;
		if (!EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm))
		{
			ReportErr("EnumDisplaySettings Failed");
		}
		newWidth=dm.dmPelsWidth;
		newHeight=dm.dmPelsHeight;
		//SetWindowLong(m_pWnd->GetHwnd(), GWL_STYLE, WS_POPUP);
		SetWindowPos(m_pWnd->GetHwnd(), HWND_NOTOPMOST, 0, 0, newWidth,newHeight, SWP_SHOWWINDOW);
	}
	else
	{
		//全屏到窗口
		int x=m_wndInfo.rcWindow.left;
		int y=m_wndInfo.rcWindow.top;
		int w=m_wndInfo.rcWindow.right - m_wndInfo.rcWindow.left;
		int h=m_wndInfo.rcWindow.bottom - m_wndInfo.rcWindow.top;
		SetWindowPos(m_pWnd->GetHwnd(), HWND_NOTOPMOST, x, y, w, h,SWP_SHOWWINDOW);
		SetWindowLong(m_pWnd->GetHwnd(), GWL_STYLE, m_wndInfo.dwStyle);
		newWidth = w;
		newHeight = h;

	}
	
	ChangeSize(newWidth,newHeight,m_bwinMode);

}