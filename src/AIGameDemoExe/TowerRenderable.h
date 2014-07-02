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
#include "EntityFactory.h"
#include "TowerEntity.h"
#include "FxEffectManager.h"
#include <d3dx9.h>
#include "QuadRenderable.h"
#include <memory>

class GameEntity;
class HippoD3d9Device;



class TowerRenderable
{
public:
	friend class TowerEntity;

	TowerRenderable(GameEntity* parent);
	~TowerRenderable();
	void Render(HippoD3d9Device* pdevice, unsigned int escapeTime);
	void RenderRangeMesh(HippoD3d9Device* pdevice, unsigned int escapeTime);
protected:
	void LoadFromFile(TowerCreateInfo& towerInfo);
	void InitPatchQuad(float range);
	GameEntity* m_parent;
	ID3DXMesh* m_pD3dxMesh;
	IDirect3DTexture9* m_d3d_texture;

	D3DXMATRIX m_local_matrix;
	EffHandle m_fxhandle;

	//patch quad
	std::unique_ptr<QuadMesh> m_quad_mesh;
	EffHandle m_QuadFxhandle;
	std::shared_ptr<IDirect3DTexture9> m_range_texture;
};