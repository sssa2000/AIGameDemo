#include "GameKeyBoardControl.h"
#include "IRobotProxy.h"
#include "IGameWorld.h"
#include "HippoRay.h"
#include "Globals.h"
#include "HippoD3d9Device.h"
#include "TankGameRender.h"
#include "IGameWorld.h"
#include "IGameWordContex.h"
#include "TerrainEntity.h"
#include "RobotEntity.h"
GameKeyBoardControl::GameKeyBoardControl(IGameWorld* pworld)
{
	m_pWorld=pworld;
		
}
GameKeyBoardControl::~GameKeyBoardControl()
{

}

void GameKeyBoardControl::OnTick(IGameWorld* p, unsigned int frameTime)
{

}

KeyCallback GameKeyBoardControl::GetKeyDownCallback()
{
	KeyCallback cb= std::tr1::bind(&GameKeyBoardControl::OnKeyDown,this,std::placeholders::_1);
	return cb;

}
MouseKeyCallback GameKeyBoardControl::GetMouseLeftDownCallback()
{
	MouseKeyCallback cb= std::tr1::bind(&GameKeyBoardControl::OnMouseLeftDown,this,std::placeholders::_1);
	return cb;
}
int GameKeyBoardControl::OnKeyDown(unsigned int k)
{
	IRobotProxy* p = m_pWorld->GetPlayerRobot();
	//加速
	//停止
	if (k == VK_UP)
	{
		p->SetVelocity(5.5f);
	}
	else if (k == VK_DOWN)
	{
		p->SetVelocity(0);
	}
	else if (k == VK_LEFT)
	{
		//转向
		float yaw = (p->GetDirection());
		p->SetDirection(yaw+0.1f);
	}
	else if (k == VK_RIGHT)
	{
		float yaw = (p->GetDirection());
		p->SetDirection(yaw - 0.1f);
	}
	return 1;
}

int GameKeyBoardControl::OnMouseLeftDown(HippoMouseEvent& e)
{
	//构造ray
	HippoRay ray=Globals::GetRender()->GetMouseRay(e.PosX,e.PosY);
	//和地形查询交点
	D3DXVECTOR3 po;
	bool b=Globals::GetWorld()->GetFloor()->RayHit(&ray,&po);

	//设置player的target point
	if(b)
	{
		Globals::GetWorld()->GetPlayer()->SetTargetPos(po);
	}
	return 1;
}