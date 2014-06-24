#include "TowerRenderable.h"
#include "TowerEntity.h"
#include "HippoD3d9Device.h"
#include "Globals.h"
#include "XModelHelpFun.h"
#include "GameRenderable.h"
#include "FxEffectManager.h"
#include "TankGameRender.h"
TowerRenderable::TowerRenderable(GameEntity* parent) 
{
	m_parent = parent;
	D3DXMatrixIdentity(&m_local_matrix);
	m_local_matrix._42 += 30;
	LoadFromFile();
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

void TowerRenderable::LoadFromFile()
{
	auto device=Globals::GetDevice()->GetDeviceD3D9();
	FillRenderableMeshWithXFile("../media/tower/kdroneminingtower_auv512.X", device, &m_pD3dxMesh,0);

	HRESULT v=D3DXCreateTextureFromFileA(device,"../media/tower/kdroneminingtower_auv512.jpg",&m_d3d_texture);

	//range mesh
	float range = ((TowerEntity*)(m_parent))->GetRange();
	D3DXCreateCylinder(device, range, range, 220, 36, 22, &m_pRangeMesh, 0);


	m_fxhandle=Globals::GetFxManager()->RequireEffectFormFile("../media/fx/default_fx.fx");
	m_fxhandle->SetTechnique("RenderSceneWithTexture1Light");
}
void TowerRenderable::RenderRangeMesh(HippoD3d9Device* pdevice, unsigned int escapeTime)
{
	auto d3d9device = pdevice->GetDeviceD3D9();
	//绘制炮塔的范围
	TowerEntity* pTower = (TowerEntity *)m_parent;
	D3DXMATRIX tmpMatrix,s;
	D3DXMatrixRotationX(&tmpMatrix, D3DXToRadian(90));
	D3DXMatrixScaling(&s, pTower->GetRange(), 0.1, pTower->GetRange());
	D3DXMatrixMultiply(&s, &tmpMatrix, &s);
	s._41 = m_parent->GetPos()->x;
	s._42 = m_parent->GetPos()->y;
	s._43 = m_parent->GetPos()->z;
	
	//world matrix
	HRESULT v = m_fxhandle->SetMatrix("g_mWorld", &s);

	//wvp matrix
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
	m_fxhandle->BeginPass(0);
	m_pRangeMesh->DrawSubset(0);
	m_fxhandle->EndPass();
	m_fxhandle->End();
}
void TowerRenderable::Render(HippoD3d9Device* pdevice, unsigned int escapeTime)
{
	auto d3d9device = pdevice->GetDeviceD3D9();
	
	//world matrix
	D3DXMATRIX tmpMatrix;
	D3DXMatrixMultiply(&tmpMatrix, &m_local_matrix, m_parent->GetWorldTransform());
	HRESULT v=m_fxhandle->SetMatrix("g_mWorld", &tmpMatrix);
	
	//wvp matrix
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
	//d3d9device->SetTransform(D3DTS_WORLD, &tmpMatrix);

	for (iPass = 0; iPass < totalPasses; iPass++)
	{
		m_fxhandle->BeginPass(iPass);
		m_pD3dxMesh->DrawSubset(0);
		m_fxhandle->EndPass();
	}

	m_fxhandle->End();
	////绘制炮塔的范围
	//TowerEntity* pTower = (TowerEntity *)m_parent;
	//D3DXMATRIX tmpMatrix,s;
	//D3DXMatrixRotationX(&tmpMatrix, D3DXToRadian(90));
	//D3DXMatrixScaling(&s, pTower->GetRange(), 0.01, pTower->GetRange());
	//D3DXMatrixMultiply(&s, &tmpMatrix, &s);
	//s._41 = m_parent->GetPos()->x;
	//s._42 = m_parent->GetPos()->y - m_h*0.5f;
	//s._43 = m_parent->GetPos()->z;
	//d3d9device->SetTransform(D3DTS_WORLD, &s);

	//m_pD3dxMesh->DrawSubset(0);
}