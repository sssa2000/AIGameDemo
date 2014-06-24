#include "AIControlImplExample.h"
#include "IGameWorld.h"
#include "ITowerProxy.h"
#include "IRobotProxy.h"
#include <d3dx9math.h>


AIControlImplExample::AIControlImplExample()
{

}
AIControlImplExample::~AIControlImplExample()
{

}

void AIControlImplExample::OnTick(IGameWorld* p, unsigned int frameTime)
{
	IRobotProxy* player = p->GetPlayerRobot();
	D3DXVECTOR2 born = player->GetPos();
	D3DXVECTOR2 end = p->GetTargetPoint();

	//计算从终点到起点的直线方向
	D3DXVECTOR2 dir = end - born;
	float angle = atanf(dir.y / dir.x);
	player->SetDirection(angle);
	player->SetVelocity(1.0f);

}