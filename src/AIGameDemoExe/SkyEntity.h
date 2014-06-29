/*!
 * \file SkyEntity.h
 * \date 2014/06/07 13:43
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
#include "GameEntity.h"
#include <d3dx9.h>
#include "FxEffectManager.h"
class HippoD3d9Device;

class SkyRenderable
{
public:
	SkyRenderable(GameEntity* p);
	~SkyRenderable();
	void Render(HippoD3d9Device* pdevice, unsigned int escapeTime);
protected:
	void MakeVertexBuffer();
	void MakeIndexBuffer();
	void MakeVertexDeclaration();
	void LoadTexture();
	void LoadFromFile();
	void LoadFx();
	GameEntity* m_parent;
	D3DXMATRIX m_local_matrix; //用于表达renderable在entity坐标系中的变换
	EffHandle m_fxhandle;
	IDirect3DVertexBuffer9*      m_pVB;//顶点缓存
	IDirect3DIndexBuffer9*       m_pIB;//索引缓存
	IDirect3DTexture9*           m_pTextrue[6];
	IDirect3DVertexDeclaration9* m_pVertexDecl;

};

class CSkyColor
{
public:

	CSkyColor();

	void GetVertexColor(float Theta, float Phi, float SunPhi, D3DXCOLOR* color);
	void SetInfo(float SunTheta, float Turbidity);


private:

	void ToRGBA(float x, float y, float Y, D3DXCOLOR* rgb);
	float Angle(float theta1, float phi1, float theta2, float phi2);

	inline float PerezFunction_x(float, float, float);
	inline float PerezFunction_y(float, float, float);
	inline float PerezFunction_Y(float, float, float);

	float Ax, Bx, Cx, Dx, Ex;
	float Ay, By, Cy, Dy, Ey;
	float AY, BY, CY, DY, EY;

	float T, suntheta, T2, suntheta3, suntheta2;
	float xz, yz, Yz;
	float cosSTheta;
};


class SkyEntity :public GameEntity
{
public:
	SkyEntity();
	~SkyEntity();

	SkyRenderable* GetRenderable(){ return m_pRenderable; }
	void Render(unsigned int escapeTime);
protected:
	SkyRenderable* m_pRenderable;
	void InitRenderable();
private:
};


class SkyDoomRenderable
{
public:
	SkyDoomRenderable(GameEntity* p,float Radius, int Slices, int Sides);
	~SkyDoomRenderable();
	void Render(HippoD3d9Device* pdevice, unsigned int escapeTime);
protected:
	D3DXCOLOR CalcVertexColor(int yidx, int xz_idx);
	int GetVertexCount();
	int GetTriangleCount();
	int GetIndexCount_TriangleList();
	int GetIndexCount_TriangleStrip();

	void MakeVertexBuffer();
	void MakeIndexBuffer();
	void MakeIndexBufferTriangleStrip();

	void MakeVertexDeclaration();
	void LoadTexture();
	void LoadFromFile();
	void LoadFx();
	float m_Radius;
	int xz_ver_num;
	int y_ver_num;
	GameEntity* m_parent;
	D3DXMATRIX m_local_matrix; //用于表达renderable在entity坐标系中的变换
	EffHandle m_fxhandle;
	IDirect3DVertexBuffer9*      m_pVB;//顶点缓存
	IDirect3DIndexBuffer9*       m_pIB;//索引缓存
	IDirect3DTexture9*           m_pTextrue[2];
	IDirect3DVertexDeclaration9* m_pVertexDecl;
	CSkyColor m_SkyColor;
};

class SkyDoomEntity :public GameEntity
{
public :
	SkyDoomEntity();
	~SkyDoomEntity();
	SkyDoomRenderable* GetRenderable(){ return m_pRenderable; }
	void Render(unsigned int escapeTime);
protected:
	
	SkyDoomRenderable* m_pRenderable;
	void InitRenderable();
};
