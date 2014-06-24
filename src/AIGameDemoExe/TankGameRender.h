/********************************************************************
	created:	2014/05/21
	created:	21:5:2014   11:09
	filename: 	E:\TankGameExe\TankGameExe\TankGameRender.h
	file path:	E:\TankGameExe\TankGameExe
	file base:	TankGameRender
	file ext:	h
	author:		sssa2000
	
	purpose:	
*********************************************************************/
#pragma once
#include <map>
#include <d3dx9.h>
class IGameWordContex;
class HippoD3d9Device;
struct ID3DXEffect;


class TankGameRender
{
public:
	void Init(int w, int h);
	void Release(){delete this;}
	TankGameRender(HippoD3d9Device* pDevice);
	~TankGameRender();
	void Render(IGameWordContex* p,unsigned int escapeTime);
	const D3DXMATRIX* GetProjMatrix(){ return &m_proj_matrix; }
	const D3DXMATRIX* GetViewMatrix();
protected:
	void Update(unsigned int escapeTime);
	void DrawTanks(IGameWordContex* p, unsigned int t);
	void DrawTowers(IGameWordContex* p, unsigned int t);
	void DrawTerrain(IGameWordContex* p, unsigned int t);
	void DrawTerrainInRange(IGameWordContex* p, unsigned int t);
	void DrawSky(IGameWordContex* p, unsigned int t);
	void DrawRangeMesh(IGameWordContex* p, unsigned int t);

private:
	HippoD3d9Device* m_pDevice;
	D3DXMATRIX m_proj_matrix;
	D3DXMATRIX m_view_matrix;

};