#include "TowerEntity.h"
#include "IGameWordContex.h"
#include "TowerRenderable.h"
#include <vector>
#include "Globals.h"
#include "GameEntity.h"
#include <algorithm>
#include "RobotEntity.h"
TowerEntity::TowerEntity():
GameEntity()
{
	m_range = 1;
}
TowerEntity::~TowerEntity()
{
	auto itr = m_pRenderables.begin();
	while (itr != m_pRenderables.end())
	{
		auto p = *itr;
		delete p;
		++itr;
	}
	m_pRenderables.clear();
}
void TowerEntity::InitRenderable(TowerCreateInfo& towerInfo)
{

	TowerRenderable* m1 = new TowerRenderable(this);
	m1->LoadFromFile(towerInfo);

	m_pRenderables.push_back(m1);

}
bool TowerEntity::IsPointInRange(const D3DXVECTOR3* point)
{
	D3DXVECTOR2 robotpos = D3DXVECTOR2(point->x, point->z);
	D3DXVECTOR2 towerpos = D3DXVECTOR2(m_Pos.x, m_Pos.z);
	D3DXVECTOR2 t = robotpos - towerpos;
	float len = D3DXVec2Length(&t);
	if (len < m_range)
		return true;
	return false;
}

void TowerEntity::Update(unsigned int escapeTime)
{
	GameEntity::Update(escapeTime);

	//向世界拿到player的位置
	auto robot = Globals::GetWorld()->GetPlayer();
	if (IsPointInRange(robot->GetPos()))
	{
		Globals::GetWorld()->NotifyGameOver();
	}
}

void TowerEntity::Render(unsigned int escapeTime)
{
	auto d3ddevice = Globals::GetDevice();
	std::for_each(
		GetRenderable()->begin(),
		GetRenderable()->end(),
		[&](TowerRenderable* pr){pr->Render(d3ddevice, escapeTime); }
	);
}