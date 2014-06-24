/********************************************************************
	created:	2014/05/21
	created:	21:5:2014   11:20
	filename: 	E:\TankGameExe\TankGameExe\IGameWorldInternal.h
	file path:	E:\TankGameExe\TankGameExe
	file base:	IGameWorldInternal
	file ext:	h
	author:		sssa2000
	
	purpose:	
*********************************************************************/
#pragma once

#include "IGameWorld.h"
#include <vector>
class GameEntity;
class TerrainEntity;
class SkyEntity;
class SkyDoomEntity;
class IGameWordContex:public IGameWorld
{
public:
	virtual void InitWorld()=0;
	virtual void Release()=0;
	virtual void Update(unsigned int escapeTime)=0;
	virtual std::vector<GameEntity*>* GetAllRobots()=0;
	virtual std::vector<GameEntity*>* GetAllTower()=0;
	virtual TerrainEntity* GetFloor() = 0;
	virtual SkyDoomEntity* GetSky() = 0;
	virtual void NotifyGameOver() = 0;
};