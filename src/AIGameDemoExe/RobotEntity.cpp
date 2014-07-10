#include "RobotEntity.h"
#include "Globals.h"
#include "RobotRenderable.h"
#include "TerrainEntity.h"
#include "IGameWordContex.h"
#include <algorithm>

RobotEntity::RobotEntity()
{
	m_YawPitchRoll = D3DXVECTOR3(0, 0, 0);
	m_v = 5;
	m_pLogic = std::tr1::make_shared<RobotProxyImpl>(this);
	m_state=RS_STOP;
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
	m_Targets.clear();

}

void RobotEntity::InitRenderable()
{
	RobotRenderable* m1 = new RobotRenderable(this);
	m1->BuildRenderable(this);
	m_pRenderables.push_back(m1);
	//m_boundingsphere.Merge(&m1->m_bs);; //包围球不考虑箭头


}
void RobotEntity::ReCalcBoundingVol()
{
	auto itr = m_pRenderables.begin();
	auto itrend = m_pRenderables.end();
	while (itr != itrend)
	{
		GameRenderable* p = *itr;
		//m_boundingsphere.Merge(&p->m_bs);
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
	//update state machine
	if(m_state==RS_RUN && !m_Targets.empty())
	{
		//调整方位
		D3DXVECTOR3 InitDir = D3DXVECTOR3(0, 0, 1);

		D3DXVECTOR3 v=m_Targets.at(0);
		D3DXVec3Normalize(&v,&v);

		float cosv=D3DXVec3Dot(&InitDir,&v);
		m_YawPitchRoll.x=acosf(cosv); //弧度
		
		//把欧拉角转化为方向
		D3DXMATRIX rotmat, tranMat;
		D3DXMatrixIdentity((&rotmat));
		//D3DXMatrixRotationYawPitchRoll(&rotmat, m_YawPitchRoll.x, 0, 0);

		//位移=速度*t
		D3DXVec3TransformCoord(&InitDir, &InitDir, &rotmat);
		m_Pos += (m_Targets.at(0)-m_Pos)* m_v*(float)(escapeTime * 0.001f);
		D3DXMatrixTranslation(&tranMat, m_Pos.x, m_Pos.y, m_Pos.z);

		//combine rotation and translate
		D3DXMatrixMultiply(&m_world_matrix, &rotmat, &tranMat);

		UpdateVarFromMatrix();

		D3DXVECTOR3 dis=(m_Pos-v);
		if (D3DXVec3Length(&dis)<=5.f)
		{
			m_state=RS_STOP;
			m_Targets.clear();
		}
		
	}
}

void RobotEntity::Render(unsigned int escapeTime)
{
	auto d3ddevice = Globals::GetDevice();
	std::for_each(
		GetRenderable()->begin(),
		GetRenderable()->end(),
		[&](GameRenderable* pr){pr->Render(d3ddevice, escapeTime); }
	);
}

void RobotEntity::SetTargetPos(const D3DXVECTOR3& v)
{
	//目前只接受一个目标点
	m_Targets.clear();
	m_Targets.push_back(v);
	m_state=RS_RUN;
}