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
	std::vector<GameEntity*>* alltanks = p->GetAllRobots();
	auto itr=alltanks->begin();
	while (itr!=alltanks->end())
	{
		RobotEntity* pEntity = (RobotEntity*)(*itr);
		std::for_each(
			pEntity->GetRenderable()->begin(), 
			pEntity->GetRenderable()->end(), 
			[&](GameRenderable* pr){pr->Render(m_pDevice,t); }
		);
		
		++itr;
	}
}
void TankGameRender::DrawTowers(IGameWordContex* p, unsigned int t)
{
	std::vector<GameEntity*>* alltowers=p->GetAllTower();
	auto itr=alltowers->begin();
	while (itr!=alltowers->end())
	{
		TowerEntity* pEntity = (TowerEntity*)(*itr);
		std::for_each(
			pEntity->GetRenderable()->begin(),
			pEntity->GetRenderable()->end(),
			[&](TowerRenderable* pr){pr->Render(m_pDevice,t); }
		);
		++itr;

	}
}
void TankGameRender::DrawRangeMesh(IGameWordContex* p, unsigned int t)
{
	std::vector<GameEntity*>* alltowers = p->GetAllTower();
	auto itr = alltowers->begin();
	while (itr != alltowers->end())
	{
		TowerEntity* pEntity = (TowerEntity*)(*itr);
		std::for_each(
			pEntity->GetRenderable()->begin(),
			pEntity->GetRenderable()->end(),
			[&](TowerRenderable* pr){pr->RenderRangeMesh(m_pDevice, t); }
		);
		++itr;

	}
}
void TankGameRender::DrawTerrain(IGameWordContex* pc, unsigned int t)
{
	TerrainEntity* floor=pc->GetFloor();
	floor->GetRenderable()->Render(m_pDevice, t);
}
void TankGameRender::DrawTerrainInRange(IGameWordContex* p, unsigned int t)
{


}


void TankGameRender::Render(IGameWordContex* p,unsigned int t)
{
	DrawSky(p, t);
	DrawTerrain(p, t);
	DrawTanks(p, t);
	DrawTowers(p, t);


}
void TankGameRender::DrawSky(IGameWordContex* p, unsigned int t)
{
	SkyDoomEntity* pSky = p->GetSky();
	if (pSky)
		pSky->GetRenderable()->Render(m_pDevice, t);
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