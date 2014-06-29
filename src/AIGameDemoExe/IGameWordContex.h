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
#include <memory>
class GameEntity;
class TerrainEntity;
class SkyEntity;
class SkyDoomEntity;

typedef std::shared_ptr<GameEntity> GameEntityPtr;
typedef std::vector<GameEntityPtr> GameEntityPtrCon;

typedef std::shared_ptr<TerrainEntity> TerrainEntityPtr;
typedef std::shared_ptr<SkyDoomEntity> SkyDoomEntityPtr;


class IGameWordContex:public IGameWorld
{
public:
	virtual void LoadFromFile(const char* f)=0;
	virtual void Release()=0;
	virtual void Update(unsigned int escapeTime)=0;
	virtual const GameEntityPtrCon& GetAllRobots() = 0;
	virtual const GameEntityPtrCon& GetAllTower() = 0;
	virtual const GameEntityPtrCon& GetAllSceneObj() = 0;
	virtual TerrainEntityPtr GetFloor() = 0;
	virtual SkyDoomEntityPtr GetSky() = 0;
	virtual void NotifyGameOver() = 0;
};