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

class GameEntity;
class HippoD3d9Device;

struct QuadVertex
{
	QuadVertex(float x, float y, float z, float u, float v)
	: m_pos(x, y, z), m_tex0(u, v)
	{
	}

	D3DXVECTOR3 m_pos;
	D3DXVECTOR2 m_tex0;
};

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
	void InitPatchQuadVB(float range,float terrainHeight);
	void InitPatchQuadIB();
	void InitPatchQuadVBDeclare();
	GameEntity* m_parent;
	ID3DXMesh* m_pD3dxMesh;
	IDirect3DTexture9* m_d3d_texture;
	IDirect3DTexture9* m_range_texture;

	D3DXMATRIX m_local_matrix;
	EffHandle m_fxhandle;

	//patch quad
	IDirect3DVertexBuffer9*      m_pQuadVB;
	IDirect3DIndexBuffer9*       m_pQuadIB;
	IDirect3DTexture9*           m_pQuadTextrue;
	IDirect3DVertexDeclaration9* m_pQuadVertexDecl;
	EffHandle m_QuadFxhandle;

};