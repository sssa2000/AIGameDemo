/********************************************************************
	created:	2014/05/23
	created:	23:5:2014   16:19
	filename: 	E:\TankGameExe\TankGameExe\RobotProxyImpl.h
	file path:	E:\TankGameExe\TankGameExe
	file base:	RobotProxyImpl
	file ext:	h
	author:		sssa2000
	
	purpose:	
*********************************************************************/
#pragma once
#include "IRobotProxy.h"

class RobotEntity;
class RobotProxyImpl :public IRobotProxy
{
public:
	RobotProxyImpl(RobotEntity* parent);
	virtual D3DXVECTOR2 GetPos();
	virtual void SetVelocity(float v);
	virtual float GetVelocity();
	virtual void SetDirection(float angle); //绕Y轴的角度，等同于欧拉角中的Yaw值
	virtual float GetDirection();
protected:
	
	RobotEntity* m_Parent;
private:
};