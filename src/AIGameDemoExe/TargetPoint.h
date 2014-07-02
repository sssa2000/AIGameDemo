/********************************************************************
	created:	2014/07/02
	created:	2:7:2014   15:46
	filename: 	E:\AIGameDemo\src\AIGameDemoExe\TargetPoint.h
	file path:	E:\AIGameDemo\src\AIGameDemoExe
	file base:	TargetPoint
	file ext:	h
	author:		sssa2000
	
	purpose:	
*********************************************************************/
#pragma once

#include "GameEntity.h"

class TargetPoint:public GameEntity
{
public:
	friend class EntityFactory;

	TargetPoint();
	~TargetPoint();

	virtual void Update(unsigned int escapeTime);
	virtual void Render(unsigned int escapeTime);
protected:
private:
};