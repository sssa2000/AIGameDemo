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
protected:
	void ReCalcBoundingVol();
	void InitRenderable();
	GameRenderableCon m_pRenderables;
	virtual void UpdateMatrixFromVar();
	std::tr1::shared_ptr<RobotProxyImpl> m_pLogic;
	D3DXVECTOR3 m_YawPitchRoll;
	float m_v;

private:
};