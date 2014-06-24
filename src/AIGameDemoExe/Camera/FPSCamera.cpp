#include "FPSCamera.h"
#include <functional>
#include "HippoD3d9Device.h"
using namespace std;



FPSCamera::FPSCamera()
{
	//D3DXQuaternionIdentity(&m_quat);
	D3DXMatrixIdentity(&m_rotation);
	m_Pos=D3DXVECTOR3(0,0,0);
	m_ViewAtDir=D3DXVECTOR3(0,0,1);
	m_UpDir=D3DXVECTOR3(0,1,0);
	m_RightDir=D3DXVECTOR3(1,0,0);
	
	m_vDirection=D3DXVECTOR3(0,0,0);
	m_vVelocity=D3DXVECTOR3(0,0,0);

	UpdateCameraCoord();
	m_bRightHasDown=false;
	m_nLastMouseX=-1;
	m_nLastMouseY=-1;

}
FPSCamera::~FPSCamera()
{

}


void FPSCamera::SetPos(float x,float y,float z)
{
	m_Pos=D3DXVECTOR3(x,y,z);
}


void FPSCamera::SetLookAtPos(float x,float y,float z)
{
	D3DXVECTOR3 at(x,y,z);
	D3DXVECTOR3 tmp=at-m_Pos; //dir是从pos射向目标
	D3DXVec3Normalize(&tmp,&tmp);
	m_ViewAtDir=D3DXVECTOR3(x,y,z)-m_Pos;
	m_UpDir=D3DXVECTOR3(0,1,0);
	D3DXVec3Cross(&m_RightDir,&m_ViewAtDir,&m_UpDir);
}



int FPSCamera::FrameUpdate(unsigned int dElapsedTime)
{

	return 1;

}
//
//  |   rx     ux     lx    0 |
//  |   ry     uy     ly    0 |
//  |   rz     uz     lz    0 |
//  | -(r.e) -(u.e) -(l.e)  1 |
//
void FPSCamera::LookAt(IDirect3DDevice9* p)
{
	D3DXMATRIX matView;
	D3DXMatrixLookAtLH(&matView, &m_Pos, &(m_Pos+m_ViewAtDir), &(m_UpDir));
	p->SetTransform(D3DTS_VIEW, &matView);
	//D3DXMATRIX view;
	//D3DXVec3Normalize( &m_ViewAtDir, &m_ViewAtDir );
	//D3DXVec3Cross( &m_RightDir, &m_UpDir, &m_ViewAtDir );
	//D3DXVec3Normalize( &m_RightDir, &m_RightDir );
	//D3DXVec3Cross( &m_UpDir, &m_ViewAtDir, &m_RightDir );
	//D3DXVec3Normalize( &m_UpDir, &m_UpDir );

	//view._11 = m_RightDir.x;
	//view._12 = m_UpDir.x;
	//view._13 = m_ViewAtDir.x;
	//view._14 = 0.0f;

	//view._21 = m_RightDir.y;
	//view._22 = m_UpDir.y;
	//view._23 = m_ViewAtDir.y;
	//view._24 = 0.0f;

	//view._31 = m_RightDir.z;
	//view._32 = m_UpDir.z;
	//view._33 = m_ViewAtDir.z;
	//view._34 = 0.0f;

	//view._41 = -D3DXVec3Dot( &m_Pos, &m_RightDir );
	//view._42 = -D3DXVec3Dot( &m_Pos, &m_UpDir );
	//view._43 = -D3DXVec3Dot( &m_Pos, &m_ViewAtDir );
	//view._44 =  1.0f;

	//p->GetDeviceD3D9()->SetTransform( D3DTS_VIEW, &view ); 
}



void FPSCamera::UpdateCameraCoord()
{
	D3DXVec3Normalize(&m_ViewAtDir,&m_ViewAtDir);
	D3DXVec3Normalize(&m_RightDir,&m_RightDir);
	D3DXVec3Normalize(&m_UpDir,&m_UpDir);
}


void FPSCamera::RoateWithYAix(float degreeInRad)
{ 
	//return;
	D3DXMatrixRotationAxis( &m_rotation, &m_UpDir, D3DXToRadian(degreeInRad));
	D3DXVec3TransformCoord( &m_ViewAtDir, &m_ViewAtDir, &m_rotation );
	D3DXVec3TransformCoord( &m_RightDir,&m_RightDir, &m_rotation );
	m_UpDir = D3DXVECTOR3(0, 1, 0);
	UpdateCameraCoord();
}

void FPSCamera::RoateWithXAix(float degreeInRad)
{

	D3DXMatrixRotationAxis( &m_rotation, &m_RightDir, D3DXToRadian(degreeInRad));
	D3DXVec3TransformCoord( &m_ViewAtDir, &m_ViewAtDir, &m_rotation );
	D3DXVec3TransformCoord( &m_UpDir, &m_UpDir, &m_rotation );
	D3DXVec3Cross(&m_RightDir, &m_UpDir, &m_ViewAtDir);
	UpdateCameraCoord();

}


//!设置移动的距离
void FPSCamera::SetPosDelta(const D3DXVECTOR3& posdelta)
{
	D3DXVECTOR3 vec3;
	D3DXVec3TransformCoord(&vec3,&posdelta,&m_rotation);
	m_Pos.x+=vec3.x;
	m_Pos.y+=vec3.y;
	m_Pos.z+=vec3.z;

}

void FPSCamera::SetPosDelta(float len)
{
	SetPosDelta(D3DXVECTOR3(len,len,len));
}
MouseKeyCallback FPSCamera::GetMouseRightDownCallback()
{
	MouseKeyCallback cb= std::tr1::bind(&FPSCamera::OnMouseRightDown,this,std::placeholders::_1);
	return cb;
}
MouseKeyCallback FPSCamera::GetMouseMoveCallback()
{
	MouseKeyCallback cb= std::tr1::bind(&FPSCamera::OnMouseMove,this,std::placeholders::_1);
	return cb;
}
MouseKeyCallback FPSCamera::GetMouseRightUpCallback()
{
	MouseKeyCallback cb= std::tr1::bind(&FPSCamera::OnMouseRightUp,this,std::placeholders::_1);
	return cb;
}
MouseWheelCallback FPSCamera::GetMouseWheelCallback()
{
	MouseWheelCallback cb= std::tr1::bind(&FPSCamera::OnMouseWheel,this,std::placeholders::_1);
	return cb;
}
KeyCallback FPSCamera::GetKeyDownCallback()
{
	KeyCallback cb= std::tr1::bind(&FPSCamera::OnKeyDown,this,std::placeholders::_1);
	return cb;
}

int FPSCamera::OnMouseRightDown(HippoMouseEvent& e)
{
	m_bRightHasDown=true;
	POINT ptCurMousePos;
	GetCursorPos( &ptCurMousePos );
	m_nLastMouseX=ptCurMousePos.x;
	m_nLastMouseY=ptCurMousePos.y;

	return 1;
}
int FPSCamera::OnMouseRightUp(HippoMouseEvent& e)
{
	m_bRightHasDown=false;
	return 1;
}
int FPSCamera::OnMouseMove(HippoMouseEvent& e)
{
	if(!m_bRightHasDown)
		return 1;

	POINT ptCurMousePos;
	GetCursorPos( &ptCurMousePos );

	int mouseX_dis = ptCurMousePos.x-m_nLastMouseX;
	int mousey_dis = ptCurMousePos.y-m_nLastMouseY;

	RoateWithYAix(mouseX_dis*0.3f);
	RoateWithXAix(-mousey_dis*0.3f);

	m_nLastMouseX=ptCurMousePos.x;
	m_nLastMouseY=ptCurMousePos.y;
	return 1;
}
int FPSCamera::OnMouseWheel(HippoWheelEvent& w)
{

	return 1;
}
int FPSCamera::OnKeyDown(unsigned int k)
{
	D3DXVECTOR3 tmpLook  = m_ViewAtDir;
	D3DXVECTOR3 tmpRight = m_RightDir;
	float g_fMoveSpeed=3.2f;
	if( k==('W'))
	{
		m_Pos -= tmpLook*-g_fMoveSpeed;
	}
	if( k==('S'))
		m_Pos += (tmpLook*-g_fMoveSpeed);

	if( k==('Q'))
		m_Pos.y += g_fMoveSpeed; 
	if( k==('E' ))
		m_Pos.y -= g_fMoveSpeed;

	if( k==('D'))
		m_Pos += (tmpRight*g_fMoveSpeed);

	if( k==('A'))
		m_Pos -= (tmpRight*g_fMoveSpeed);

	return 1;
}

