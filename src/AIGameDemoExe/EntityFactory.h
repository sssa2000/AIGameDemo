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

class GameEntity;
class HippoD3d9Device;
class TowerEntity;
class TerrainEntity;
class EntityFactory
{
public:
	void Release(){delete this;}
	EntityFactory(HippoD3d9Device* pDevice);
	GameEntity* CreateTank();
	TowerEntity* CreateTower(float range);
	TerrainEntity* CreateFloor();
private:
	GameEntity* CreateFromXFile(const char* s);
	HippoD3d9Device* m_pDevice;

};