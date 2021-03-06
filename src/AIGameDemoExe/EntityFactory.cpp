#include "RobotEntity.h"
#include "EntityFactory.h"
#include "HippoD3d9Device.h"
#include "GameRenderable.h"
#include "GameEntity.h"
#include <d3dx9.h>
#include "HippoFilePath.h"
#include "RobotEntity.h"
#include "RobotRenderable.h"
#include "TowerEntity.h"
#include "TowerRenderable.h"
#include "XModelHelpFun.h"
#include "TerrainEntity.h"
#include "StartPoint.h"
#include "TargetPoint.h"
EntityFactory::EntityFactory(HippoD3d9Device* pDevice)
{
	m_pDevice=pDevice;
}

RobotEntity* EntityFactory::CreateTank()
{
	RobotEntity* probot=new RobotEntity;
	probot->InitRenderable();
	return probot;
}

TowerEntity* EntityFactory::CreateTower(TowerCreateInfo& towerInfo)
{

	TowerEntity* p1 = new TowerEntity();
	p1->m_range = towerInfo.range;
	p1->InitRenderable(towerInfo);
	p1->SetWorldTransform(&towerInfo.worldTransform);
	return p1;

}

TerrainEntity* EntityFactory::CreateFloor()
{
	TerrainEntity* p = new TerrainEntity();
	p->InitRenderable();
	return p;
}


GameEntity* EntityFactory::CreateFromXFile(const char* fn)
{

	return 0;
}

GameEntity* EntityFactory::CreateSceneObj(SceneObjCreateInfo& objinfo)
{
	GameEntity* p1 = new GameEntity();
	p1->InitRenderable(objinfo);
	p1->SetWorldTransform(&objinfo.worldTransform);
	return p1;
}

StartPoint* EntityFactory::CreateStartPoint(float x, float y)
{
	StartPoint* p=new StartPoint(x,y);
	p->InitRenderable(x,y);

	return p;

}
TargetPoint* EntityFactory::CreateTargetPoint(float x, float y)
{
	TargetPoint* p=new TargetPoint(x,y);
	p->InitRenderable(x,y);

	return p;

}