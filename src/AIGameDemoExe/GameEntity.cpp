#include "GameEntity.h"
#include "GameRenderable.h"
#include "Globals.h"

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

void  GameEntity::Update(unsigned int escapeTime)
{

}

void DecomposeMatrix(D3DXMATRIX* m, D3DXVECTOR3& pos, D3DXQUATERNION& rot, D3DXVECTOR3& scl)
{
	//pos = D3DXVECTOR3(m->_41, m->_42, m->_43);

	////scale
	//D3DXVECTOR3 col1(m->_11, m->_21, m->_31);
	//D3DXVECTOR3 col2(m->_12, m->_22, m->_32);
	//D3DXVECTOR3 col3(m->_13, m->_23, m->_33);
	//scl = D3DXVECTOR3(D3DXVec3Length(&col1), D3DXVec3Length(&col2), D3DXVec3Length(&col3));

	////rol
	//if (scl.x != 0)
	//{
	//	col1 /= scl.x;
	//}
	//if (scl.y != 0)
	//{
	//	col2 /= scl.y;
	//}
	//if (scl.z != 0)
	//{
	//	col3 /= scl.z;
	//}
	//D3DXMATRIX mRot;
	//D3DXMatrixIdentity(&mRot);
	//mRot._11 = col1.x; mRot._12 = col2.x; mRot._13 = col3.x;
	//mRot._21 = col2.y; mRot._22 = col2.y; mRot._23 = col3.y;
	//mRot._31 = col3.z; mRot._32 = col2.z; mRot._33 = col3.z;

	//D3DXQuaternionRotationMatrix(&rot, &mRot);


	D3DXMatrixDecompose(&scl, &rot, &pos, m);
}

void GameEntity::SetWorldTransform(D3DXMATRIX* p)
{
	DecomposeMatrix(p, m_Pos, m_Quat, m_Scale);
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