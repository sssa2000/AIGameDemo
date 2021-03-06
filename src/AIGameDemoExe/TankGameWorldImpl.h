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
#include <memory>

class TerrainEntity;
class SkyEntity;
class SkyDoomEntity;
class IRobotProxy;
class TiXmlElement;
struct SceneObjCreateInfo;
struct ModelCreateInfo;
class StartPoint;
class TargetPoint;

class TankGameWorldImpl:public IGameWordContex
{
public:
	TankGameWorldImpl();
	~TankGameWorldImpl();
	virtual void LoadFromFile(const char* filename);
	virtual void Release(){ delete this; }
	virtual void Update(unsigned int escapeTime);

	virtual const PlayerPtr& GetPlayer(){ return m_pPlayer; }
	virtual const GameEntityPtrCon& GetAllTower(){ return m_towers; }
	virtual const GameEntityPtrCon& GetAllSceneObj(){ return m_scene_objs; }
	virtual const StartPointptr& GetStartPointPtr(){return m_start;}
	virtual const Targetptr& GetTargetPointPtr(){return m_end;}
	virtual TerrainEntityPtr GetFloor(){ return m_pWorld; }
	virtual SkyDoomEntityPtr GetSky(){ return  m_pSky; }

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
	virtual bool OnPlayerMove(RobotEntity* p,const D3DXVECTOR3& dir,float v,DWORD timeInMs);
protected:
	void ResetScene();
	void ParseTower(TiXmlElement* e);
	void ParseStartPoint(TiXmlElement* e);
	void ParseEndPoint(TiXmlElement* e);
	void ParseSceneObj(TiXmlElement* e);
	void ParseModelElem(ModelCreateInfo* out, TiXmlElement* e);
	void ParseMatrixElem(D3DXMATRIX* out, TiXmlElement* e);
	void SetObjIntoTerrain(float x,float z,GameEntity* obj);
	PlayerPtr m_pPlayer;
	GameEntityPtrCon m_towers;
	GameEntityPtrCon m_scene_objs;
	TerrainEntityPtr m_pWorld;
	SkyDoomEntityPtr m_pSky;

	StartPointptr m_start;
	Targetptr m_end;
};