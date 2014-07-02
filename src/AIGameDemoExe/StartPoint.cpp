#include "StartPoint.h"
#include "HippoD3d9Device.h"
#include "Globals.h"
#include "TankGameRender.h"
StartPoint::StartPoint(float posX,float posZ)
{

}
StartPoint::~StartPoint()
{

}

void StartPoint::Update(unsigned int escapeTime)
{

}
void StartPoint::Render(unsigned int escapeTime)
{
	auto d3d9device = Globals::GetDevice()->GetDeviceD3D9();
	D3DXMATRIX tmpMatrix = *GetWorldTransform();
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
	v = m_QuadFxhandle->SetTexture("g_DiffuseTex", m_texture.get());

	//range
	float range = 20.f;
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

void StartPoint::InitRenderable(float posX,float posZ)
{
	auto device = Globals::GetDevice()->GetDeviceD3D9();
	SetPostion(posX,0,posZ);

	//mesh
	m_quad_mesh.reset(new QuadMesh);
	m_quad_mesh->Init(0);

	//fx
	m_QuadFxhandle = Globals::GetFxManager()->RequireEffectFormFile("../media/fx/terrain_patch.fx");
	m_QuadFxhandle ->SetTechnique("RenderSceneWithTexture1Light");

	//texture
	IDirect3DTexture9* p = 0;
	HRESULT v = D3DXCreateTextureFromFileA(device, "../media/start_point.tga", &p);
	m_texture.reset(p,[&](IDirect3DTexture9* pT){pT->Release();});
}