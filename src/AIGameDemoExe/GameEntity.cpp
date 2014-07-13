#include "GameEntity.h"
#include "GameRenderable.h"
#include "Globals.h"
#include "D3dxMathExtension.h"
GameEntity::GameEntity()
{
	D3DXMatrixIdentity(&m_world_matrix);
	m_Pos = D3DXVECTOR3(0, 0, 0);
	D3DXQuaternionIdentity(&m_Quat);
	m_Scale = D3DXVECTOR3(1, 1, 1);
	UpdateMatrixFromVar();
	//m_bNeedUpdatePosRot = true;
}

GameEntity::~GameEntity()
{

}

void GameEntity::UpdateMatrixFromVar()
{
	ComposeMatrix(&m_world_matrix,m_Pos,m_Quat,m_Scale);
}
void  GameEntity::Update(unsigned int escapeTime)
{
	UpdateMatrixFromVar();
}

void GameEntity::SetWorldTransform(D3DXMATRIX* p)
{
	D3DXMatrixDecompose(&m_Scale, &m_Quat, &m_Pos, p );
}

void GameEntity::SetPostion(float x, float y, float z)
{
	m_Pos = D3DXVECTOR3(x, y, z);
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
bool GameEntity::RayHit(HippoRay* ray,D3DXVECTOR3* insertPoint)
{
	return false;
}