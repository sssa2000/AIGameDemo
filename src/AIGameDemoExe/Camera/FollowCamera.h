/********************************************************************
	created:	2014/06/24
	created:	24:6:2014   15:38
	filename: 	E:\AIGameDemo\src\AIGameDemoExe\Camera\FollowCamera.h
	file path:	E:\AIGameDemo\src\AIGameDemoExe\Camera
	file base:	FollowCamera
	file ext:	h
	author:		sssa2000
	
	purpose:	
*********************************************************************/
#pragma once

#include "FPSCamera.h"
class GameEntity;



class FollowCamera:public FPSCamera
{
public:
	FollowCamera();
	~FollowCamera();
	void SetFollowTarget(GameEntity* p);
	int FrameUpdate(unsigned int dElapsedTime);
protected:
	void SetLookAtPos(float x, float y, float z);
	void CalcCameraPosRotation();
	GameEntity* m_pTarget;
private:
};