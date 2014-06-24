/*!
 * \file TerrainEntity.h
 * \date 2014/06/04 1:04
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

#pragma once
#include "GameEntity.h"
#include <d3dx9.h>
#include "FxEffectManager.h"
class HippoD3d9Device;
class TerrainRenderable;

struct TerrainVertex
{
	TerrainVertex(float x, float y, float z, float u, float v)
		: m_pos(x, y, z), m_tex0(u, v), m_normal(0, 1, 0)
	{
	}

	D3DXVECTOR3 m_pos;
	D3DXVECTOR2 m_tex0;
	D3DXVECTOR3 m_normal;

};
//贴地面片
class TerrainPatch
{
public:
	TerrainPatch(TerrainRenderable* parent,float x,float y,float z,float range);
	~TerrainPatch();
	void ChangePos(float x,float y,float z);
	EffHandle& GetPatchFx(){ return m_fxhandle; }
	IDirect3DIndexBuffer9* GetIB(){ return m_pIB; }
	int GetIndexCount(){ return m_IndexData.size(); }
private:
	void CalcIndexBuffer();
	void LoadFx();
	TerrainRenderable* m_parent;
	D3DXVECTOR3 m_Pos;
	float m_r;
	IDirect3DIndexBuffer9*       m_pIB;
	std::vector<WORD> m_IndexData;
	EffHandle m_fxhandle;

};

class TerrainHeightField
{
public:
	TerrainHeightField();
	~TerrainHeightField();

	void LoadFromHeightMap(const char* filename, int dstW, int dstH);
	float GetHeight(int x, int y);

private:
	int m_width;
	int m_height;
	unsigned char* m_pData;

};
class TerrainRenderable
{
public:
	friend class TerrainPatch;

	TerrainRenderable(GameEntity* parent);
	~TerrainRenderable();
	virtual void Render(HippoD3d9Device* pdevice, unsigned int escapeTime);
	TerrainPatch* CreatePatch(float x,float y,float z,float range);
	void ReleasePatch(TerrainPatch* p);
	float GetTerrainHeight(float x,float z);
	void LoadFromFile();

protected:
	virtual int GetVertexNum();
	virtual int GetTriangleNum();
	virtual int GetIndexNum();

	void DrawPatch(HippoD3d9Device* pdevice, unsigned int escapeTime);
	virtual void MakeVertexBuffer();
	virtual void MakeIndexBuffer();
	void MakeVertexDeclaration();
	virtual void LoadTexture();
	virtual void LoadFx();
	void ConvertPos(float& x,float& z);
	ID3DXEffect* m_pEffect;
	GameEntity* m_parent;
	EffHandle m_fxhandle;
	D3DXMATRIX m_local_matrix; //用于表达renderable在entity坐标系中的变换
	IDirect3DVertexBuffer9*      m_pVB;//顶点缓存
	IDirect3DIndexBuffer9*       m_pIB;//索引缓存
	IDirect3DTexture9*           m_pTextrue[3];
	IDirect3DVertexDeclaration9* m_pVertexDecl;
	TerrainHeightField* m_pHeightField;
	std::vector<TerrainVertex> m_VertexData;
	std::vector<WORD> m_IndexData;
	std::vector<TerrainPatch*> m_all_patch;
};

class TerrainRenderablePlane :TerrainRenderable
{
public:
	TerrainRenderablePlane(GameEntity* parent);
	~TerrainRenderablePlane();
	void Render(HippoD3d9Device* pdevice, unsigned int escapeTime);
protected:
	virtual int GetVertexNum();
	virtual int GetTriangleNum();
	virtual int GetIndexNum();
	virtual void MakeVertexBuffer();
	virtual void MakeIndexBuffer();
	virtual void LoadTexture();
	virtual void LoadFx();
};
class TerrainEntity:public GameEntity
{
public:
	TerrainEntity();
	~TerrainEntity();
	void InitRenderable();

	TerrainRenderable* GetRenderable(){ return m_pRenderable; }
	float GetHeight(float x,float z){return m_pRenderable->GetTerrainHeight(x,z);}
protected:
	TerrainRenderable* m_pRenderable;
private:
};