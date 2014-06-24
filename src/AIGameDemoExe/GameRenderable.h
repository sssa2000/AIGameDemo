/********************************************************************
	created:	2014/05/21
	created:	21:5:2014   15:55
	filename: 	E:\TankGameExe\TankGameExe\Renderable.h
	file path:	E:\TankGameExe\TankGameExe
	file base:	Renderable
	file ext:	h
	author:		sssa2000
	
	purpose:	
*********************************************************************/
#pragma once
#include <d3dx9mesh.h>
#include "BoundingVol.h"
#include <vector>
class HippoD3d9Device;
class GameEntity;

struct IDirect3DTexture9;
struct ID3DXMesh;

class sMaterial
{
public:
	sMaterial(D3DXMATERIAL& pmat);
	~sMaterial();
	D3DMATERIAL9* m_d3d_mat;
	IDirect3DTexture9* m_d3d_texture;

};

class GameRenderable
{
public:

	GameRenderable(GameEntity* parent);
	~GameRenderable();
	void CreateDefaultMat();
	virtual void Render(HippoD3d9Device* pdevice, unsigned int escapeTime);
	GameEntity* m_parent;
	ID3DXMesh* m_pD3dxMesh;
	std::vector<sMaterial*> m_mats;
	sBoundingSphere m_bs;
	D3DXMATRIX m_local_matrix; //用于表达renderable在entity坐标系中的变换
	D3DXMATRIX m_final_matrix;//表示最后绘制的矩阵，多个矩阵最后运算的结果
private:
};