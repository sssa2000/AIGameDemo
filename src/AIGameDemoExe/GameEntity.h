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
class GameRenderable;



class GameEntity
{
public:
	friend class EntityFactory;

	GameEntity();
	~GameEntity();

	virtual void  Update(unsigned int escapeTime);

	D3DXMATRIX* GetWorldTransform(){return &m_world_matrix;}

	sBoundingSphere* GetBoundSphere();
	void SetWorldTransform(D3DXMATRIX* p);
	void ApplyWorldTransform(D3DXMATRIX& mat);
	void SetPostion(float x, float y, float z);
	D3DXVECTOR3* GetPos();
protected:
	void UpdateVarFromMatrix();
	virtual void UpdateMatrixFromVar();

	bool m_bNeedUpdatePosRot; //控制是否需要更新变量

	D3DXMATRIX m_world_matrix;
	D3DXVECTOR3 m_Pos;

	sBoundingSphere m_boundingsphere;
};