#include "GameRenderable.h"
#include "GameEntity.h"
#include "HippoD3d9Device.h"
#include "Globals.h"
#include "XModelHelpFun.h"
#include "TankGameRender.h"
#include <d3dx9.h>
#include <string>
#include "ErrReport.h"
#include "EntityFactory.h"
GameRenderable::GameRenderable(GameEntity* parent)
{
	m_parent = parent;
	
}
GameRenderable::~GameRenderable()
{

}
void GameRenderable::Render(HippoD3d9Device* pdevice, unsigned int t)
{
	auto d3d9device = pdevice->GetDeviceD3D9();

	//world matrix
	HRESULT v = m_fxhandle->SetMatrix("g_mWorld", m_parent->GetWorldTransform());

	auto proj = Globals::GetRender()->GetProjMatrix();
	auto view = Globals::GetRender()->GetViewMatrix();
	D3DXMATRIX vp = (*view)*(*proj);
	v = m_fxhandle->SetMatrix("g_mViewProjection", &vp);

	//material
	D3DXCOLOR vWhite = D3DXCOLOR(1, 1, 1, 1);
	v = m_fxhandle->SetValue("g_MaterialDiffuseColor", &vWhite, sizeof(D3DXCOLOR));

	//texture
	v = m_fxhandle->SetTexture("g_MeshTexture", m_d3d_texture.get());

	UINT iPass, totalPasses;
	m_fxhandle->Begin(&totalPasses, 0);

	for (iPass = 0; iPass < totalPasses; iPass++)
	{
		m_fxhandle->BeginPass(iPass);
		m_pD3dxMesh->DrawSubset(0);
		m_fxhandle->EndPass();
	}

	m_fxhandle->End();
}

void GameRenderable::LoadFromFile(SceneObjCreateInfo& sceneobj)
{
	auto device = Globals::GetDevice()->GetDeviceD3D9();
	
	ID3DXMesh* meshptr = 0;
	FillRenderableMeshWithXFile(sceneobj.sobj.meshFilename.c_str(), device, &meshptr);
	m_pD3dxMesh.reset(meshptr);

	IDirect3DTexture9* textureptr = 0;
	HRESULT v = D3DXCreateTextureFromFileA(device, sceneobj.sobj.textureFilename.at(0).c_str(), &textureptr);
	m_d3d_texture.reset(textureptr);

	m_fxhandle = Globals::GetFxManager()->RequireEffectFormFile(sceneobj.sobj.fxFilename.c_str());
	m_fxhandle->SetTechnique("RenderSceneWithTexture1Light");
	
}