#include "GameKeyBoardControl.h"
#include "IRobotProxy.h"
#include "IGameWorld.h"
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