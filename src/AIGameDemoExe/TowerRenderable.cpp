#include "TowerRenderable.h"
#include "TowerEntity.h"
#include "HippoD3d9Device.h"
#include "Globals.h"
#include "XModelHelpFun.h"
#include "GameRenderable.h"
#include "TankGameRender.h"
#include <d3dx9.h>
#include "ErrReport.h"

TowerRenderable::TowerRenderable(GameEntity* parent)
{
	m_parent = parent;
}

TowerRenderable::~TowerRenderable()
{
	if (m_pD3dxMesh)
	{
		m_pD3dxMesh->Release();
		m_pD3dxMesh = 0;
	}

	m_d3d_texture->Release();
}

void TowerRenderable::InitPatchQuad(float scale)
{
	auto device = Globals::GetDevice()->GetDeviceD3D9();
	m_quad_mesh.reset(new QuadMesh);
	m_quad_mesh->Init(0);
	
	//fx
	m_QuadFxhandle = Globals::GetFxManager()->RequireEffectFormFile("../media/fx/terrain_patch.fx");
	m_QuadFxhandle ->SetTechnique("RenderSceneWithTexture1Light");
	
	//texture
	IDirect3DTexture9* p = 0;
	HRESULT v = D3DXCreateTextureFromFileA(device, "../media/tower/range.tga", &p);
	m_range_texture.reset(p,[&](IDirect3DTexture9* pT){pT->Release();});
}

void TowerRenderable::LoadFromFile(TowerCreateInfo& towerInfo)
{
	auto device = Globals::GetDevice()->GetDeviceD3D9();
	FillRenderableMeshWithXFile(towerInfo.sobj.meshFilename.c_str(), device, &m_pD3dxMesh);

	HRESULT v = D3DXCreateTextureFromFileA(device, towerInfo.sobj.textureFilename.at(0).c_str(), &m_d3d_texture);

	m_fxhandle = Globals::GetFxManager()->RequireEffectFormFile(towerInfo.sobj.fxFilename.c_str());
	m_fxhandle->SetTechnique("RenderSceneWithTexture1Light");
	TowerEntity* pTower = (TowerEntity *)m_parent;
	InitPatchQuad(pTower->GetRange());
}


void TowerRenderable::RenderRangeMesh(HippoD3d9Device* pdevice, unsigned int escapeTime)
{
	auto d3d9device = pdevice->GetDeviceD3D9();
	TowerEntity* pTower = (TowerEntity *)m_parent;
	D3DXMATRIX tmpMatrix = *m_parent->GetWorldTransform();
	tmpMatrix._42 = 0.1f;
	HRESULT v = m_QuadFxhandle->SetMatrix("g_mWorld", &tmpMatrix);
	
	d3d9device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	d3d9device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	d3d9device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	//wvp matrix

	//wvp matrix
	auto proj = Globals::GetRender()->GetProjMatrix();
	auto view = Globals::GetRender()->GetViewMatrix();
	D3DXMATRIX vp = (*view)*(*proj);
	v = m_QuadFxhandle->SetMatrix("g_mViewProjection", &vp);

	//material
	D3DXCOLOR vWhite = D3DXCOLOR(1, 1, 1, 1);
	v = m_QuadFxhandle->SetValue("g_MaterialDiffuseColor", &vWhite, sizeof(D3DXCOLOR));

	//texture
	v = m_QuadFxhandle->SetTexture("g_DiffuseTex", m_range_texture.get());

	//range
	float range = pTower->GetRange();
	v = m_QuadFxhandle->SetValue("range", &range, sizeof(float));

	m_quad_mesh->SetVertexIndex(d3d9device);
	

	UINT totalPasses;
	m_QuadFxhandle->Begin(&totalPasses, 0);
	m_QuadFxhandle->BeginPass(0);
	m_quad_mesh->DrawMesh(d3d9device);
	
	m_QuadFxhandle->EndPass();
	m_QuadFxhandle->End();

	d3d9device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

}
void TowerRenderable::Render(HippoD3d9Device* pdevice, unsigned int escapeTime)
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
	v = m_fxhandle->SetTexture("g_MeshTexture", m_d3d_texture);

	UINT iPass, totalPasses;
	m_fxhandle->Begin(&totalPasses, 0);

	for (iPass = 0; iPass < totalPasses; iPass++)
	{
		m_fxhandle->BeginPass(iPass);
		m_pD3dxMesh->DrawSubset(0);
		m_fxhandle->EndPass();
	}

	m_fxhandle->End();

	////绘制炮塔的范围
	RenderRangeMesh(pdevice, escapeTime);
}