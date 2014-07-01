/*!
 * \file QuadRenderable.h
 * \date 2014/07/02 0:14
 *
 * \author BooSTLoki
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#pragma once

#include "GameRenderable.h"
#include <d3d9.h>


struct QuadVertex
{
	QuadVertex(float x, float y, float z, float u, float v)
	: m_pos(x, y, z), m_tex0(u, v)
	{
	}

	D3DXVECTOR3 m_pos;
	D3DXVECTOR2 m_tex0;
};

class QuadMesh
{
public:
	QuadMesh();
	~QuadMesh();

	void SetVertexIndex(IDirect3DDevice9* d3d9device);
	void DrawMesh(IDirect3DDevice9* d3d9device);
	void Init(float terrainHeight);
private:
	void InitPatchQuadVB(float terrainHeight);
	void InitPatchQuadIB();
	void InitPatchQuadVBDeclare();

	IDirect3DVertexBuffer9*      m_pQuadVB;
	IDirect3DIndexBuffer9*       m_pQuadIB;
	IDirect3DVertexDeclaration9* m_pQuadVertexDecl;

};
