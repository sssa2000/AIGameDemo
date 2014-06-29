#include "GameEntity.h"
#include "GameRenderable.h"
#include "Globals.h"

GameEntity::GameEntity()
{
	D3DXMatrixIdentity(&m_world_matrix);
	UpdateVarFromMatrix();
	//m_bNeedUpdatePosRot = true;
}

GameEntity::~GameEntity()
{

}

void  GameEntity::Update(unsigned int escapeTime)
{

}
void GameEntity::SetWorldTransform(D3DXMATRIX* p)
{
	m_world_matrix = *p;
	UpdateVarFromMatrix();
}

void GameEntity::SetPostion(float x, float y, float z)
{
	m_world_matrix._41 = x;
	m_world_matrix._42 = y;
	m_world_matrix._43 = z;
	UpdateVarFromMatrix();
}
void GameEntity::UpdateVarFromMatrix()
{
	m_Pos = D3DXVECTOR3(m_world_matrix._41, m_world_matrix._42, m_world_matrix._43);
}
void GameEntity::UpdateMatrixFromVar()
{
	m_world_matrix._41 = m_Pos.x;
	m_world_matrix._42 = m_Pos.y;
	m_world_matrix._43 = m_Pos.z;
}

D3DXVECTOR3* GameEntity::GetPos()
{
	return &m_Pos;
}

sBoundingSphere* GameEntity::GetBoundSphere()
{
	return &m_boundingsphere;
}

void GameEntity::Render(unsigned int escapeTime)
{
	auto d3ddevice = Globals::GetDevice();
	m_pRenderable->Render(d3ddevice, escapeTime);
}

void GameEntity::InitRenderable(SceneObjCreateInfo& info)
{
	m_pRenderable.reset(new GameRenderable(this));
	m_pRenderable->LoadFromFile(info);

}