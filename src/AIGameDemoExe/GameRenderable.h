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
#include <memory>
#include "FxEffectManager.h"


class HippoD3d9Device;
class GameEntity;
struct SceneObjCreateInfo;

struct IDirect3DTexture9;
struct ID3DXMesh;

class GameRenderable
{
public:
	friend class GameEntity;
	GameRenderable(GameEntity* parent);
	virtual ~GameRenderable();
	virtual void Render(HippoD3d9Device* pdevice, unsigned int escapeTime);

protected:
	GameEntity* m_parent;

private:
	void LoadFromFile(SceneObjCreateInfo& objInfo);
	EffHandle m_fxhandle;
	std::shared_ptr<IDirect3DTexture9> m_d3d_texture;
	std::shared_ptr<ID3DXMesh> m_pD3dxMesh;
	DWORD m_nNumMesh;
};