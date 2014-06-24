/********************************************************************
created:	2014/05/21
created:	21:5:2014   14:29
filename: 	E:\TankGameExe\TankGameExe\TankGameWorldImpl.h
file path:	E:\TankGameExe\TankGameExe
file base:	TankGameWorldImpl
file ext:	h
author:		sssa2000

purpose:	
*********************************************************************/
#pragma once

#include "IGameWordContex.h"
#include <vector>
#include "GameEntity.h"

class TerrainEntity;
class SkyEntity;
class SkyDoomEntity;

class TankGameWorldImpl:public IGameWordContex
{
public:
	TankGameWorldImpl();
	~TankGameWorldImpl();
	virtual void InitWorld();
	virtual void Release(){delete this;}
	virtual void Update(unsigned int escapeTime);

	virtual std::vector<GameEntity*>* GetAllRobots();
	virtual std::vector<GameEntity*>* GetAllTower();
	TerrainEntity* GetFloor(){ return m_pWorld; }
	SkyDoomEntity* GetSky(){ return  m_pSky; }
	void NotifyGameOver();

	void AddTank(GameEntity* p);
	void AddTower(GameEntity* p);

	//得到出生点的位置
	virtual D3DXVECTOR2 GetBornPoint();

	//得到目标点(即终点)的位置
	virtual D3DXVECTOR2 GetTargetPoint();

	//得到操作的机器人的实例
	virtual IRobotProxy* GetPlayerRobot();

	//得到tower的数量
	virtual unsigned int GetTowerNumber();

	//通过idx检索得到tower的实例，传入的idx不能大于等于GetTowerNumber返回的值
	virtual ITowerProxy* GetTowerByIdx(unsigned int idx);

protected:
	void SetObjIntoTerrain(float x,float z,GameEntity* obj);
	std::vector<GameEntity*> m_tanks;
	std::vector<GameEntity*> m_towers;
	TerrainEntity* m_pWorld;
	SkyDoomEntity* m_pSky;
	D3DXVECTOR3 m_bornPoint;
	D3DXVECTOR3 m_targetPoint;

};