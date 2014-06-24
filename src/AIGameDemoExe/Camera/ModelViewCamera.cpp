#include "ModelViewCamera.h"

ModelViewCamera::ModelViewCamera()
{
	D3DXMatrixIdentity(&m_rotation);
	m_Pos=D3DXVECTOR3(0,0,-1);
	m_ViewAtPoint=D3DXVECTOR3(0,0,0);
	m_ViewAtDir=m_ViewAtPoint-m_Pos;

	m_UpDir=D3DXVECTOR3(0,1,0);
	m_RightDir=D3DXVECTOR3(1,0,0);
	m_xz_delta=0;
	m_y_delta=0;

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
	m_ViewAtDir=D3DXVECTOR3(x,y,z)-m_Pos;
	m_UpDir=D3DXVECTOR3(0,1,0);
	D3DXVec3Cross(&m_RightDir,&m_ViewAtDir,&m_UpDir);
}
void ModelViewCamera::CalcMouseMove(unsigned int dElapsedTime)
{
	POINT ptCurMousePos;
	GetCursorPos(&ptCurMousePos);
	float g_fMoveSpeed = 3.2f;
	D3DXVECTOR3 tmpRight = m_RightDir;
	D3DXVECTOR3 tmpLook = m_ViewAtDir;
	tmpRight.y = tmpLook.y = 0;

	if (ptCurMousePos.x > 1000)
	{
		m_ViewAtPoint += (tmpRight*g_fMoveSpeed);
	}
	else if (ptCurMousePos.x < 20)
	{
		m_ViewAtPoint -= (tmpRight*g_fMoveSpeed);
	}
	if (ptCurMousePos.y < 10)
	{
		m_ViewAtPoint -= tmpLook*-g_fMoveSpeed;
	}
	else if (ptCurMousePos.y >750)
	{
		m_ViewAtPoint += tmpLook*-g_fMoveSpeed;
	}
}

int ModelViewCamera::FrameUpdate(unsigned int dElapsedTime)
{
	//计算target point
	CalcMouseMove(dElapsedTime);

	m_Pos.x=m_r*cosf(m_xz_delta)*sinf(m_y_delta);
	m_Pos.z=m_r*sinf(m_xz_delta)*sinf(m_y_delta);
	m_Pos.y=m_r*cosf(m_y_delta);

	m_Pos+=m_ViewAtPoint;

	//先绕竖直向上的轴转
	m_ViewAtDir=m_ViewAtPoint-m_Pos;
	D3DXVECTOR3 up(0,1,0);
	D3DXMATRIX _rotation;
	D3DXMatrixRotationAxis( &_rotation, &up, m_xz_delta);
	D3DXVec3TransformCoord( &m_RightDir,&m_RightDir, &m_rotation );

	//再绕x转
	D3DXMatrixRotationAxis( &_rotation, &m_RightDir, m_y_delta);
	m_UpDir = D3DXVECTOR3(0, 1, 0);
	D3DXVec3TransformCoord( &m_UpDir,&m_UpDir, &m_rotation );

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
		if (m_y_delta >= D3DXToRadian(75))
			m_y_delta = D3DXToRadian(75);

		if (m_y_delta <= D3DXToRadian(15))
			m_y_delta = D3DXToRadian(15);

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