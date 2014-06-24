/********************************************************************
	created:	2014/05/23
	created:	23:5:2014   16:22
	filename: 	E:\TankGameExe\TankGameExe\RobotRenderable.h
	file path:	E:\TankGameExe\TankGameExe
	file base:	RobotRenderable
	file ext:	h
	author:		sssa2000
	
	purpose:	机器人在绘制上有不同于其他普通物体的行为：
	1、机器人随机上下浮动
	2、机器人有一个箭头，一直指向运动方向
*********************************************************************/
#pragma once

#include "GameRenderable.h"
#include "FxEffectManager.h"


class RobotRenderable:public GameRenderable
{
public:
	RobotRenderable(GameEntity* parent);
	~RobotRenderable();
	void Render(HippoD3d9Device* pdevice, unsigned int escapeTime);
	GameRenderable* m_arrow_renderable;
private:
	void BuildRenderable(GameEntity* parent);
	void CalcPos(unsigned int escapeTime);
	D3DXMATRIX m_RobotRenderableMatrix;
	EffHandle m_fxhandle;
	IDirect3DTexture9* m_d3d_texture;
	void LoadFromFile();

};