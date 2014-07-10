/********************************************************************
	created:	2014/05/21
	created:	21:5:2014   16:20
	filename: 	E:\TankGameExe\TankGameExe\TankFactory.h
	file path:	E:\TankGameExe\TankGameExe
	file base:	TankFactory
	file ext:	h
	author:		sssa2000
	
	purpose:	
*********************************************************************/
#pragma once
#include <vector>
#include <string>
#include <d3dx9math.h>

class GameEntity;
class HippoD3d9Device;
class TowerEntity;
class TerrainEntity;

struct ModelCreateInfo
{
	std::string meshFilename;
	std::string fxFilename;
	std::vector<std::string> textureFilename;
};

struct TowerCreateInfo
{
	float range;
	ModelCreateInfo sobj;
	D3DXMATRIX worldTransform;
};

struct SceneObjCreateInfo
{
	ModelCreateInfo sobj;
	D3DXMATRIX worldTransform;
};

class StartPoint;
class TargetPoint;
class RobotEntity;
class EntityFactory
{
public:
	void Release(){delete this;}
	EntityFactory(HippoD3d9Device* pDevice);
	RobotEntity* CreateTank();
	TowerEntity* CreateTower(TowerCreateInfo& towerInfo);
	TerrainEntity* CreateFloor();
	GameEntity* CreateSceneObj(SceneObjCreateInfo& objinfo);
	StartPoint* CreateStartPoint(float x, float y);
	TargetPoint* CreateTargetPoint(float x, float y);
private:
	GameEntity* CreateFromXFile(const char* s);
	HippoD3d9Device* m_pDevice;

};