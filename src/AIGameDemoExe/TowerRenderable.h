/*!
 * \file TowerRenderable.h
 * \date 2014/05/25 17:40
 *
 * \author BooSTLoki
 * Contact: user@company.com
 *
 * \brief sssa2000
 *
 * TODO: long description
 *
 * \note
*/

#pragma once
#include <vector>
#include <d3dx9.h>
#include "FxEffectManager.h"
class sMaterial;
class GameEntity;
class HippoD3d9Device;

class TowerRenderable
{
public:
	TowerRenderable(GameEntity* parent);
	~TowerRenderable();
	void Render(HippoD3d9Device* pdevice, unsigned int escapeTime);
	void RenderRangeMesh(HippoD3d9Device* pdevice, unsigned int escapeTime);
	ID3DXMesh* GetRangeMesh(){ return m_pRangeMesh; }
protected:
	void LoadFromFile();
	ID3DXEffect* m_pEffect;
	GameEntity* m_parent;
	ID3DXMesh* m_pD3dxMesh;
	IDirect3DTexture9* m_d3d_texture;
	D3DXMATRIX m_local_matrix; //用于表达renderable在entity坐标系中的变换
	EffHandle m_fxhandle;
	ID3DXMesh* m_pRangeMesh;
};