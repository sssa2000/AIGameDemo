/********************************************************************
created:	2014/05/21
created:	21:5:2014   15:53
filename: 	E:\TankGameExe\TankGameExe\GameEntity.h
file path:	E:\TankGameExe\TankGameExe
file base:	GameEntity
file ext:	h
author:		sssa2000

purpose:	game entity是游戏中的逻辑实体
*********************************************************************/
#pragma once
#include <d3dx9math.h>
#include <vector>
#include "BoundingVol.h"
#include <memory>
#include "EntityFactory.h"
class GameRenderable;
struct SceneObjCreateInfo;
class HippoRay;

class GameEntity
{
public:
	friend class EntityFactory;

	GameEntity();
	virtual ~GameEntity();

	virtual void Update(unsigned int escapeTime);
	virtual void Render(unsigned int escapeTime);
	D3DXMATRIX* GetWorldTransform(){return &m_world_matrix;}
	
	virtual bool RayHit(HippoRay* ray,D3DXVECTOR3* insertPoint);
	sBoundingSphere* GetBoundSphere();
	void SetWorldTransform(D3DXMATRIX* p);
	void SetPostion(float x, float y, float z);
	D3DXVECTOR3* GetPos();
	void SetTargetPos(float x,float y,float z);

protected:
	void UpdateMatrixFromVar();
	D3DXVECTOR3 m_Pos;
	D3DXQUATERNION m_Quat;
	D3DXVECTOR3 m_Scale;
	
	sBoundingSphere m_boundingsphere;
	D3DXMATRIX m_world_matrix;

private:
	void InitRenderable(SceneObjCreateInfo& info);
	std::unique_ptr<GameRenderable> m_pRenderable;

};