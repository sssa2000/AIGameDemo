/********************************************************************
	created:	2014/05/23
	created:	23:5:2014   17:21
	filename: 	E:\TankGameExe\TankGameExe\IGameKeyBoardControl.h
	file path:	E:\TankGameExe\TankGameExe
	file base:	IGameKeyBoardControl
	file ext:	h
	author:		sssa2000
	
	purpose:	键盘控制器，通过响应键盘按键控制游戏
*********************************************************************/
#pragma once

#include "IGameControl.h"
#include "Win32MsgUtil.h"

class IGameWorld;
class GameKeyBoardControl:public IGameControl
{
public:
	void Release(){delete this;}
	GameKeyBoardControl(IGameWorld* pworld);
	~GameKeyBoardControl();

	void OnTick(IGameWorld* p, unsigned int frameTime);
	KeyCallback GetKeyDownCallback();
protected:
	int OnKeyDown(unsigned int);
	IGameWorld* m_pWorld;
private:
};