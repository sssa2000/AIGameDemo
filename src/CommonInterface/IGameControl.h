/********************************************************************
	created:	2014/05/23
	created:	23:5:2014   17:18
	filename: 	E:\TankGameExe\TankGameExe\IGameControl.h
	file path:	E:\TankGameExe\TankGameExe
	file base:	IGameControl
	file ext:	h
	author:		sssa2000
	
	purpose:	游戏控制器的基类，通过扩展该类实现对游戏的控制
*********************************************************************/
#pragma once

class IGameWorld;
class IGameControl
{
public:
	virtual void OnTick(IGameWorld* p,unsigned int frameTime)=0;

};