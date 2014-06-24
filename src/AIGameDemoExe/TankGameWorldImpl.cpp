#include "TankGameWorldImpl.h"
#include "GameEntity.h"
#include "Globals.h"
#include "EntityFactory.h"
#include "RobotEntity.h"
#include <functional>
#include <algorithm>
#include "TowerEntity.h"
#include "HippoD3DxModel.h"
#include "HippoD3d9Device.h"
#include "SkyEntity.h"
#include "CameraBase.h"
#include "FollowCamera.h"
#include "TerrainEntity.h"
void CreateWorldTmp()
{
	
}

TankGameWorldImpl::TankGameWorldImpl()
{


}


TankGameWorldImpl::~TankGameWorldImpl()
{

}
void TankGameWorldImpl::SetObjIntoTerrain(float x,float z,GameEntity* obj)
{
	float y=m_pWorld->GetHeight(x,z);
	obj->SetPostion(x, y, z);

}
void TankGameWorldImpl::InitWorld()
{
	m_pWorld = Globals::GetEntityFactory()->CreateFloor();
	m_pSky =  new SkyDoomEntity;

	auto* tank=Globals::GetEntityFactory()->CreateTank();
	AddTank(tank);
	float x=0;
	float z=0;
	float y=m_pWorld->GetHeight(x,z);
	tank->SetPostion(x,y+5,z);


	auto* tower=Globals::GetEntityFactory()->CreateTower(23);
	AddTower(tower);
	x=125;
	z=234;
	y=m_pWorld->GetHeight(x,z);
	tower->SetPostion(x,y+2,z);
	//tower->SetPostion(15, 0, 12);

	tower = Globals::GetEntityFactory()->CreateTower(33);
	AddTower(tower);
	x=225;
	z=134;
	y=m_pWorld->GetHeight(x,z);
	tower->SetPostion(x,y+2,z);

	 Globals::GetFollowCamera()->SetFollowTarget(GetAllRobots()->at(0));

}
void TankGameWorldImpl::Update(unsigned int escapeTime)
{
	std::for_each(
		m_tanks.begin(),
		m_tanks.end(),
		[&](GameEntity* entity){entity->Update(escapeTime); }
	);

	std::for_each(
		m_towers.begin(),
		m_towers.end(),
		[&](GameEntity* m_towers){m_towers->Update(escapeTime); }
	);
}

std::vector<GameEntity*>* TankGameWorldImpl::GetAllRobots()
{
	return &m_tanks;
}

std::vector<GameEntity*>* TankGameWorldImpl::GetAllTower()
{
	return &m_towers;
}

void TankGameWorldImpl::AddTank(GameEntity* p)
{
	m_tanks.push_back(p);
}
void TankGameWorldImpl::AddTower(GameEntity* p)
{
	m_towers.push_back(p);
}

IRobotProxy* TankGameWorldImpl::GetPlayerRobot()
{
	RobotEntity* e = (RobotEntity*)m_tanks.at(0);
	return e->GetProxy();
}

void TankGameWorldImpl::NotifyGameOver()
{
	::MessageBoxA(0, "你被炮塔发现，游戏结束!", 0, 0);
}

//得到出生点的位置
D3DXVECTOR2 TankGameWorldImpl::GetBornPoint()
{
	return D3DXVECTOR2(m_bornPoint.x, m_bornPoint.z);
}

//得到目标点(即终点)的位置
D3DXVECTOR2 TankGameWorldImpl::GetTargetPoint()
{
	return D3DXVECTOR2(m_targetPoint.x, m_targetPoint.z);
}

//得到tower的数量
unsigned int TankGameWorldImpl::GetTowerNumber()
{
	return m_towers.size();
}

//通过idx检索得到tower的实例，传入的idx不能大于等于GetTowerNumber返回的值
ITowerProxy* TankGameWorldImpl::GetTowerByIdx(unsigned int idx)
{
	if (idx < 0 || idx >= m_towers.size())
		return 0;

	TowerEntity* p = (TowerEntity*)(m_towers.at(idx));
	return p->GetProxy();
}