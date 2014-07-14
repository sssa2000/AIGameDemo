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

enum BORDER_DEFINE
{
	POSITIVE_X=0,
	NEGATIVE_X,

	POSITIVE_Y,
	NEGATIVE_Y,

	BD_END
};
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
	virtual ~TerrainRenderable();
	virtual void Render(HippoD3d9Device* pdevice, unsigned int escapeTime);
	TerrainPatch* CreatePatch(float x,float y,float z,float range);
	void ReleasePatch(TerrainPatch* p);
	float GetTerrainHeight(float x,float z);
	virtual void LoadFromFile();
	virtual bool RayHit(HippoRay* ray,D3DXVECTOR3* insertPoint){return false;}
	virtual const D3DXPLANE* GetBorderPlane(BORDER_DEFINE d){ return 0; }
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
class TerrainBorderRenderable
{
public:
	TerrainBorderRenderable();
	~TerrainBorderRenderable();

	//参数是把地形看作一个2维矩形，该矩形的四个顶点
	void Init(float left, float top, float right, float bottom);
	void Render(HippoD3d9Device* pdevice, unsigned int escapeTime);
	const D3DXPLANE* GetBorderPlane(BORDER_DEFINE d){ return &m_border_planes[d]; }
private:
	void MakeVertexBuffer(float left, float top, float right, float bottom);
	void MakeIndexBuffer();
	void MakeVertexDeclaration();
	void LoadTexture();
	void LoadFx();
	//border plane of terrain
	EffHandle m_fxhandle;
	std::shared_ptr<IDirect3DVertexBuffer9>      m_pVB;//顶点缓存
	std::shared_ptr<IDirect3DIndexBuffer9>       m_pIB;//索引缓存
	std::shared_ptr<IDirect3DTexture9>           m_pTextrue;
	std::shared_ptr<IDirect3DVertexDeclaration9> m_pVertexDecl;
	D3DXPLANE m_border_planes[BD_END];
};
class TerrainRenderablePlane :TerrainRenderable
{
public:
	TerrainRenderablePlane(GameEntity* parent);
	~TerrainRenderablePlane();
	void Render(HippoD3d9Device* pdevice, unsigned int escapeTime);
	virtual bool RayHit(HippoRay* ray,D3DXVECTOR3* insertPoint);
	const D3DXPLANE* GetBorderPlane(BORDER_DEFINE d){ return m_border_renderable->GetBorderPlane(d); }
protected:
	virtual int GetVertexNum();
	virtual int GetTriangleNum();
	virtual int GetIndexNum();
	virtual void MakeVertexBuffer();
	virtual void MakeIndexBuffer();
	virtual void LoadTexture();
	virtual void LoadFx();
	virtual void LoadFromFile();
private:
	D3DXPLANE m_phy_shape;
	//border plane of terrain
	std::shared_ptr<TerrainBorderRenderable> m_border_renderable;

};

class TerrainEntity:public GameEntity
{
public:
	TerrainEntity();
	~TerrainEntity();
	void InitRenderable();
	void Render(unsigned int escapeTime);
	TerrainRenderable* GetRenderable(){ return m_pRenderable; }
	float GetHeight(float x,float z){return m_pRenderable->GetTerrainHeight(x,z);}
	virtual bool RayHit(HippoRay* ray,D3DXVECTOR3* insertPoint);

	const D3DXPLANE* GetBorderPlane(BORDER_DEFINE d);
protected:
	TerrainRenderable* m_pRenderable;
private:
};