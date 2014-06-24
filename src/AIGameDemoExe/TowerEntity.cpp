#include "TowerEntity.h"
#include "Globals.h"
#include "IGameWordContex.h"
#include "TowerRenderable.h"
TowerEntity::TowerEntity(float range) :
GameEntity()
{
	m_range = range;
	InitRenderable();
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
void TowerEntity::InitRenderable()
{

	TowerRenderable* m1 = new TowerRenderable(this);
	m_pRenderables.push_back(m1);

}
bool TowerEntity::IsPointInRange(const D3DXVECTOR3* point)
{
	D3DXVECTOR2 robotpos = D3DXVECTOR2(point->x, point->z);
	D3DXVECTOR2 towerpos = D3DXVECTOR2(m_Pos.x, m_Pos.z);
	D3DXVECTOR2 t = robotpos - towerpos;
	float len=D3DXVec2Length(&t);
	if (len < m_range)
		return true;
	return false;
}

void TowerEntity::Update(unsigned int escapeTime)
{
	//向世界拿到player的位置
	std::vector<GameEntity*>* all=Globals::GetWorld()->GetAllRobots();
	auto itr = all->begin();
	auto itrend = all->end();
	while (itr!=itrend)
	{
		GameEntity* robot = all->at(0);
		if (IsPointInRange(robot->GetPos()))
		{
			Globals::GetWorld()->NotifyGameOver();
		}
		++itr;
	}
}

