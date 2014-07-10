#include "TankGameRender.h"
#include "IGameWordContex.h"
#include "HippoD3d9Device.h"
#include "GameEntity.h"
#include "GameRenderable.h"
#include <algorithm>
#include <d3dx9.h>
#include "ErrReport.h"
#include "TowerRenderable.h"
#include "RobotRenderable.h"
#include "RobotEntity.h"
#include "TowerEntity.h"
#include "TerrainEntity.h"
#include "ScopeGuard.h"
#include "Globals.h"
#include "FxEffectManager.h"
#include "FPSCamera.h"
#include "SkyEntity.h"
#include "StartPoint.h"
#include "TargetPoint.h"
TankGameRender::TankGameRender(HippoD3d9Device* pDevice)
{
	m_pDevice=pDevice;
}

TankGameRender::~TankGameRender()
{

}

void TankGameRender::Init(int w, int h)
{
	D3DXMatrixPerspectiveFovLH(&m_proj_matrix, 45.f, (float)w / (float)h, 1.0f, 5000.0f);
}

void TankGameRender::Update(unsigned int escapeTime)
{

}
void TankGameRender::DrawTanks(IGameWordContex* p, unsigned int t)
{
	p->GetPlayer()->Render(t);
}
void TankGameRender::DrawTowers(IGameWordContex* p, unsigned int t)
{
	auto alltowers=p->GetAllTower();
	auto itr=alltowers.begin();
	while (itr!=alltowers.end())
	{
		auto pEntity = (*itr);
		pEntity->Render(t);
		++itr;
	}
}

void TankGameRender::DrawTerrain(IGameWordContex* pc, unsigned int t)
{
	pc->GetFloor()->Render( t);
}
void TankGameRender::DrawSceneObj(IGameWordContex* p, unsigned int t)
{
	auto allsceneobj = p->GetAllSceneObj();
	auto itr = allsceneobj.begin();
	while (itr != allsceneobj.end())
	{
		auto pEntity = (*itr);
		pEntity->Render(t);
		++itr;
	}
}

void TankGameRender::DrawStartEnd(IGameWordContex* p, unsigned int t)
{
	p->GetStartPointPtr()->Render(t);
	p->GetTargetPointPtr()->Render(t);
}
void TankGameRender::Render(IGameWordContex* p,unsigned int t)
{
	DrawSky(p, t);
	DrawTerrain(p, t);
	DrawTanks(p, t);
	DrawTowers(p, t);
	DrawSceneObj(p, t);
	DrawStartEnd(p, t);
}
void TankGameRender::DrawSky(IGameWordContex* p, unsigned int t)
{
	p->GetSky()->Render(t);
	
}

const D3DXMATRIX* TankGameRender::GetViewMatrix()
{
	auto* camera = Globals::GetCurrentCamera();
	const D3DXVECTOR3* pos = camera->GetPos();
	const D3DXVECTOR3* dir = camera->GetDir();
	const D3DXVECTOR3* up = camera->GetUp();
	D3DXMatrixLookAtLH(&m_view_matrix, pos, &(*pos + *dir), up);
	return &m_view_matrix;
}

HippoRay TankGameRender::GetMouseRay(int mouse_x,int mouse_y)
{
	D3DVIEWPORT9 vp;
	Globals::GetDevice()->GetDeviceD3D9()->GetViewport(&vp);
	unsigned int screen_width=vp.Width;
	unsigned int screen_height=vp.Height;

	mouse_y=screen_height-mouse_y;

	D3DXVECTOR3 rayPos; 
	rayPos.x = ( ( ( 2.0f * mouse_x ) / screen_width ) - 1 ) / m_proj_matrix._11;
	rayPos.y = ( ( ( 2.0f * mouse_y ) / screen_height ) - 1 ) / m_proj_matrix._22;
	rayPos.z =  1;

	D3DXMATRIX matInverse;
	D3DXMatrixInverse(&matInverse,0,&m_view_matrix);

	//D3DXVECTOR3 dir;
	//D3DXVec3TransformCoord(&dir,&rayPos,&matInverse);
	//D3DXVec4Normalize(&dir4,&dir4);
	//D3DXVECTOR3 dir(dir4.x,dir4.y,dir4.z);

	float dirx  = rayPos.x*matInverse._11 + rayPos.y*matInverse._21 + rayPos.z*matInverse._31;
	float diry  = rayPos.x*matInverse._12 + rayPos.y*matInverse._22 + rayPos.z*matInverse._32;
	float dirz  = rayPos.x*matInverse._13+ rayPos.y*matInverse._23 + rayPos.z*matInverse._33;
	D3DXVECTOR3 dir(dirx,diry,dirz);
	D3DXVec3Normalize(&dir,&dir);

	D3DXVECTOR3 pos(matInverse._41,matInverse._42,matInverse._43);
	return HippoRay(pos,dir);
}