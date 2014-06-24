/********************************************************************
	created:	2014/05/21
	created:	21:5:2014   11:54
	filename: 	E:\TankGameExe\TankGameExe\IRobotLogic.h
	file path:	E:\TankGameExe\TankGameExe
	file base:	IGameTank
	file ext:	h
	author:		sssa2000
	
	purpose:	 保存robot的逻辑数据，维护逻辑上的行为
*********************************************************************/
#pragma once
#include <d3dx9math.h>

class IRobotProxy
{
public:
	//得到当前机器人的位置
	virtual D3DXVECTOR2 GetPos()=0;

	//设置、获取机器人的速度值
	virtual void SetVelocity(float v)=0;
	virtual float GetVelocity()=0;

	//设置、获取机器人的方向，该方向是一个角度值
	//是绕Y轴(3维空间中竖直向上的轴)的角度，等同于欧拉角中的Yaw值
	virtual void SetDirection(float angle)=0; 
	virtual float GetDirection() = 0;

};