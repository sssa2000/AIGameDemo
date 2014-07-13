/********************************************************************
	created:	2014/05/23
	created:	23:5:2014   11:51
	filename: 	E:\TankGameExe\TankGameExe\RobotEntity.h
	file path:	E:\TankGameExe\TankGameExe
	file base:	RobotEntity
	file ext:	h
	author:		sssa2000
	
	purpose:	
*********************************************************************/
#pragma once
#include "GameEntity.h"
#include "RobotProxyImpl.h"
#include <memory>
class IRobotProxy;
class RobotRenderable;

typedef std::vector<GameRenderable*> GameRenderableCon;

class TerrainPatch;

enum RobotState
{
	RS_STOP,
	RS_RUN
};
class RobotEntity:public GameEntity
{
public:
	friend class EntityFactory;
	friend class RobotProxyImpl;

	RobotEntity();
	~RobotEntity();
	IRobotProxy* GetProxy(){ return (IRobotProxy*)m_pLogic.get(); }
	virtual void  Update(unsigned int escapeTime);
	virtual void SetVelocity(float);
	virtual float GetVelocity();
	virtual void SetDirection(float angle);
	virtual float GetDirection();
	GameRenderableCon* GetRenderable(){ return &m_pRenderables; }
	void Render(unsigned int escapeTime);
	void SetTargetPos(const D3DXVECTOR3& v);
	
protected:
	void ReCalcBoundingVol();
	void InitRenderable();
	GameRenderableCon m_pRenderables;
	std::tr1::shared_ptr<RobotProxyImpl> m_pLogic;
	D3DXVECTOR3 m_YawPitchRoll;
	float m_v;
	std::vector<D3DXVECTOR3> m_Targets;
	RobotState m_state;

private:
};