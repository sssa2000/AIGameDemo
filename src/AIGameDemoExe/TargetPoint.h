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
#include "QuadRenderable.h"
#include <memory>

class TargetPoint:public GameEntity
{
public:
	friend class EntityFactory;

	TargetPoint(float posX,float posZ);
	~TargetPoint();

	virtual void Update(unsigned int escapeTime);
	virtual void Render(unsigned int escapeTime);
protected:
private:
	float m_target_range;
	float m_cylinder_height;

	void InitRenderable(float posX,float posZ);
	void RenderCylider();
	void RenderQuad();

	std::unique_ptr<QuadMesh> m_quad_mesh;
	EffHandle m_QuadFxhandle;
	std::shared_ptr<IDirect3DTexture9> m_texture;
	
	//cylinder 
	D3DXMATRIX m_cylinder_local_matrix;
	std::shared_ptr<ID3DXMesh> m_cylinder_mesh;
	EffHandle m_cylinder_fxhandle;
	std::shared_ptr<IDirect3DTexture9> m_cylinder_texture;
};