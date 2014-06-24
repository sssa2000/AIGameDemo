#include "RobotEntity.h"
#include "Globals.h"
#include "RobotRenderable.h"
#include "TerrainEntity.h"
#include "IGameWordContex.h"


RobotEntity::RobotEntity()
{
	m_YawPitchRoll = D3DXVECTOR3(0, 0, 0);
	m_v = 0;
	m_pLogic = std::tr1::make_shared<RobotProxyImpl>(this);
	InitRenderable();
}

RobotEntity::~RobotEntity()
{
	auto itr = m_pRenderables.begin();
	while (itr!=m_pRenderables.end())
	{
		auto p = *itr;
		delete p;
		++itr;
	}
	m_pRenderables.clear();

	if(m_pTerrainPatch)
		Globals::GetWorld()->GetFloor()->GetRenderable()->ReleasePatch(m_pTerrainPatch);
	m_pTerrainPatch=0;
}

void RobotEntity::InitRenderable()
{
	RobotRenderable* m1 = new RobotRenderable(this);
	m_pRenderables.push_back(m1);
	m_boundingsphere.Merge(&m1->m_bs);; //包围球不考虑箭头

	TerrainEntity* terrain=Globals::GetWorld()->GetFloor();
	D3DXVECTOR3* pos=GetPos();
	m_pTerrainPatch=terrain->GetRenderable()->CreatePatch(pos->x,pos->y,pos->z,30.0f);
	//m_pTerrainPatch->ChangePos(pos->x, pos->y, pos->z);




}
void RobotEntity::ReCalcBoundingVol()
{
	auto itr = m_pRenderables.begin();
	auto itrend = m_pRenderables.end();
	while (itr != itrend)
	{
		GameRenderable* p = *itr;
		m_boundingsphere.Merge(&p->m_bs);
		++itr;
	}
}

void RobotEntity::SetVelocity(float f)
{
	m_v = f;
}

float RobotEntity::GetVelocity()
{
	return m_v;
}
void RobotEntity::SetDirection(float yaw)
{
	m_YawPitchRoll.x = yaw;
}

void RobotEntity::UpdateMatrixFromVar()
{
	GameEntity::UpdateMatrixFromVar();

}
float RobotEntity::GetDirection()
{
	return m_YawPitchRoll.x;
}

void  RobotEntity::Update(unsigned int escapeTime)
{
	//把欧拉角转化为方向
	D3DXMATRIX rotmat, tranMat;
	D3DXMatrixRotationYawPitchRoll(&rotmat, m_YawPitchRoll.x, 0, 0);
	
	//位移=速度*t
	D3DXVECTOR3 InitDir = D3DXVECTOR3(0, 0, 1);
	D3DXVec3TransformCoord(&InitDir, &InitDir, &rotmat);
	m_Pos += InitDir* m_v*(float)(escapeTime * 0.001f);
	D3DXMatrixTranslation(&tranMat, m_Pos.x, m_Pos.y, m_Pos.z);
	
	//combine rotation and translate
	D3DXMatrixMultiply(&m_world_matrix, &rotmat, &tranMat);

	UpdateVarFromMatrix();

	//update terrain patch
	D3DXVECTOR3* pos=GetPos();
	//m_pTerrainPatch->ChangePos(pos->x,pos->y,pos->z);
}
