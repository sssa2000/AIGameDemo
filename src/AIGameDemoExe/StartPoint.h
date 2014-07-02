/********************************************************************
	created:	2014/07/02
	created:	2:7:2014   16:35
	filename: 	E:\AIGameDemo\src\AIGameDemoExe\StartPoint.h
	file path:	E:\AIGameDemo\src\AIGameDemoExe
	file base:	StartPoint
	file ext:	h
	author:		sssa2000
	
	purpose:	
*********************************************************************/
#pragma once

#include "GameEntity.h"
#include "QuadRenderable.h"
#include <memory>
class StartPoint:public GameEntity
{
public:
	friend class EntityFactory;

	StartPoint(float posX,float posZ);
	~StartPoint();

	virtual void Update(unsigned int escapeTime);
	virtual void Render(unsigned int escapeTime);
protected:
private:
	
	void InitRenderable(float posX,float posZ);
	std::unique_ptr<QuadMesh> m_quad_mesh;
	EffHandle m_QuadFxhandle;
	std::shared_ptr<IDirect3DTexture9> m_texture;
};