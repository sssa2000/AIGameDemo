#include "ModelViewCamera.h"
#include "Globals.h"
ModelViewCamera::ModelViewCamera()
{
	D3DXMatrixIdentity(&m_rotation);
	m_Pos=D3DXVECTOR3(0,0,-1);
	m_ViewAtPoint=D3DXVECTOR3(0,0,0);
	m_ViewAtDir=m_ViewAtPoint-m_Pos;

	m_UpDir=D3DXVECTOR3(0,1,0);
	m_RightDir=D3DXVECTOR3(1,0,0);
	m_xz_delta=0;
	m_y_delta=D3DXToRadian(45);

	UpdateCameraCoord();
	m_bRightHasDown=false;
	m_nLastMouseX=-1;
	m_nLastMouseY=-1;
	m_r=100.f;
}
ModelViewCamera::~ModelViewCamera()
{

}

void ModelViewCamera::SetPos(float x,float y,float z)
{
	m_Pos=D3DXVECTOR3(x,y,z);
}
void ModelViewCamera::SetLookAtPos(float x,float y,float z)
{
	D3DXVECTOR3 at(x,y,z);
	m_ViewAtPoint=at;
	m_ViewAtDir=D3DXVECTOR3(x,y,z)-m_Pos;
	m_UpDir=D3DXVECTOR3(0,1,0);
	D3DXVec3Cross(&m_RightDir,&m_ViewAtDir,&m_UpDir);
}
void ModelViewCamera::CalcMouseMove(unsigned int dElapsedTime)
{
	POINT ptCurMousePos;
	GetCursorPos(&ptCurMousePos);

	//转化为相对窗口原点（wnd左上角）的坐标
	RECT wndRect;

	GetWindowRect(Globals::GetWndHwnd(),&wndRect);
	int caption_w=GetSystemMetrics(SM_CYSMCAPTION);
	int xborder_w=GetSystemMetrics(SM_CXBORDER);
	int yborder_w=GetSystemMetrics(SM_CYBORDER);


	int rel_mouse_x=ptCurMousePos.x;
	int rel_mouse_y=ptCurMousePos.y;


	float g_fMoveSpeed = 3.2f;
	D3DXVECTOR3 tmpRight = m_RightDir;
	D3DXVECTOR3 tmpLook = m_ViewAtDir;
	tmpRight.y = tmpLook.y = 0;

	const int offset=35;
	if (rel_mouse_x > (wndRect.right-xborder_w-offset) && rel_mouse_x < wndRect.right)
	{
		m_ViewAtPoint += (tmpRight*g_fMoveSpeed);
	}
	else if (rel_mouse_x < (wndRect.left+xborder_w+offset) && rel_mouse_x > wndRect.left)
	{
		m_ViewAtPoint -= (tmpRight*g_fMoveSpeed);
	}
	
	if (rel_mouse_y <(wndRect.top+caption_w+offset+yborder_w) && rel_mouse_y > wndRect.top)
	{
		m_ViewAtPoint -= tmpLook*-g_fMoveSpeed;
	}
	else if (rel_mouse_y > (wndRect.bottom -offset-yborder_w) && rel_mouse_y < wndRect.bottom)
	{
		m_ViewAtPoint += tmpLook*-g_fMoveSpeed;
	}
}

int ModelViewCamera::FrameUpdate(unsigned int dElapsedTime)
{


	m_Pos.x=m_r*cosf(m_xz_delta)*sinf(m_y_delta);
	m_Pos.z=m_r*sinf(m_xz_delta)*sinf(m_y_delta);
	m_Pos.y=m_r*cosf(m_y_delta);

	m_Pos+=m_ViewAtPoint;
	CalcMouseMove(dElapsedTime);
	
	m_ViewAtDir=m_ViewAtPoint-m_Pos;
	m_UpDir = D3DXVECTOR3(0, 1, 0);
	D3DXVec3Cross(&m_RightDir,&m_UpDir,&m_ViewAtDir);
	D3DXVec3Cross(&m_UpDir,&m_ViewAtDir,&m_RightDir);

	D3DXVec3Normalize(&m_ViewAtDir,&m_ViewAtDir);
	D3DXVec3Normalize(&m_RightDir,&m_RightDir);
	D3DXVec3Normalize(&m_UpDir,&m_UpDir);

	return 1;

}
void ModelViewCamera::LookAt(IDirect3DDevice9* p)
{
	D3DXMATRIX matView;
	D3DXMatrixLookAtLH(&matView, &m_Pos, &m_ViewAtPoint, &(m_UpDir));
	//p->SetTransform(D3DTS_VIEW, &matView);
}

void ModelViewCamera::UpdateCameraCoord()
{
	D3DXVec3Normalize(&m_ViewAtDir,&m_ViewAtDir);
	D3DXVec3Normalize(&m_RightDir,&m_RightDir);
	D3DXVec3Normalize(&m_UpDir,&m_UpDir);
}


MouseKeyCallback ModelViewCamera::GetMouseRightDownCallback()
{
	MouseKeyCallback cb= std::tr1::bind(&ModelViewCamera::OnMouseRightDown,this,std::placeholders::_1);
	return cb;
}
MouseKeyCallback ModelViewCamera::GetMouseMoveCallback()
{
	MouseKeyCallback cb= std::tr1::bind(&ModelViewCamera::OnMouseMove,this,std::placeholders::_1);
	return cb;
}
MouseKeyCallback ModelViewCamera::GetMouseRightUpCallback()
{
	MouseKeyCallback cb= std::tr1::bind(&ModelViewCamera::OnMouseRightUp,this,std::placeholders::_1);
	return cb;
}
MouseWheelCallback ModelViewCamera::GetMouseWheelCallback()
{
	MouseWheelCallback cb= std::tr1::bind(&ModelViewCamera::OnMouseWheel,this,std::placeholders::_1);
	return cb;
}
KeyCallback ModelViewCamera::GetKeyDownCallback()
{
	KeyCallback cb= std::tr1::bind(&ModelViewCamera::OnKeyDown,this,std::placeholders::_1);
	return cb;
}

int ModelViewCamera::OnMouseRightDown(HippoMouseEvent& e)
{
	m_bRightHasDown=true;
	POINT ptCurMousePos;
	GetCursorPos( &ptCurMousePos );
	m_nLastMouseX=ptCurMousePos.x;
	m_nLastMouseY=ptCurMousePos.y;

	return 1;
}
int ModelViewCamera::OnMouseRightUp(HippoMouseEvent& e)
{
	m_bRightHasDown=false;
	return 1;
}
int ModelViewCamera::OnMouseMove(HippoMouseEvent& e)
{
	if (m_bRightHasDown)
	{
		POINT ptCurMousePos;
		GetCursorPos(&ptCurMousePos);

		int mouseX_dis = ptCurMousePos.x - m_nLastMouseX;
		int mousey_dis = ptCurMousePos.y - m_nLastMouseY;

		m_xz_delta += mouseX_dis*0.03f;
		m_y_delta += -mousey_dis*0.03f;
		if (m_y_delta >= D3DXToRadian(85))
			m_y_delta = D3DXToRadian(85);

		if (m_y_delta <= D3DXToRadian(5))
			m_y_delta = D3DXToRadian(5);

		m_nLastMouseX = ptCurMousePos.x;
		m_nLastMouseY = ptCurMousePos.y;
	}

	return 1;
}

int ModelViewCamera::OnMouseWheel(HippoWheelEvent& w)
{
	m_r -= w.wheel_delta*0.1f;
	return 1;
}
int ModelViewCamera::OnKeyDown(unsigned int k)
{
	D3DXVECTOR3 tmpLook  = m_ViewAtDir;
	D3DXVECTOR3 tmpRight = m_RightDir;
	float g_fMoveSpeed=3.2f;
	if( k==('W'))
	{
		m_ViewAtPoint -= tmpLook*-g_fMoveSpeed;
	}
	if( k==('S'))
		m_ViewAtPoint += (tmpLook*-g_fMoveSpeed);

	if( k==('Q'))
		m_ViewAtPoint.y += g_fMoveSpeed; 
	if( k==('E' ))
		m_ViewAtPoint.y -= g_fMoveSpeed;

	if( k==('D'))
		m_ViewAtPoint += (tmpRight*g_fMoveSpeed);

	if( k==('A'))
		m_ViewAtPoint -= (tmpRight*g_fMoveSpeed);

	return 1;
}