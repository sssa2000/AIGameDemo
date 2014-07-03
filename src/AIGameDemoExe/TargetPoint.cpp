#include "TargetPoint.h"
#include "HippoD3d9Device.h"
#include "Globals.h"
#include "TankGameRender.h"

TargetPoint::TargetPoint(float posX,float posZ)
{
	m_target_range=100.f;
	m_cylinder_height=300.f;
}
TargetPoint::~TargetPoint()
{

}

void TargetPoint::Update(unsigned int escapeTime)
{

}

void TargetPoint::InitRenderable(float posX,float posZ)
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

	//cylinder
	ID3DXMesh* tmpmesh=0;
	D3DXCreateCylinder(device,m_target_range,m_target_range,m_cylinder_height,20,20,&tmpmesh,0);
	m_cylinder_mesh.reset(tmpmesh,[&](ID3DXMesh* pT){pT->Release();});

	D3DXMatrixIdentity(&m_cylinder_local_matrix);
	D3DXMatrixRotationX(&m_cylinder_local_matrix,D3DXToRadian(90));

	m_cylinder_local_matrix._41=posX;
	m_cylinder_local_matrix._42=0.5f*m_cylinder_height;
	m_cylinder_local_matrix._43=posZ;


	m_cylinder_fxhandle = Globals::GetFxManager()->RequireEffectFormFile("../media/fx/default_fx.fx");
	m_cylinder_fxhandle ->SetTechnique("RenderSceneWithTexture1Light");

	IDirect3DTexture9* p1 = 0;
	v = D3DXCreateTextureFromFileA(device, "../media/tower/range.tga", &p1);
	m_cylinder_texture.reset(p1,[&](IDirect3DTexture9* pT){pT->Release();});
	
}

void TargetPoint::RenderQuad()
{
	auto d3d9device = Globals::GetDevice()->GetDeviceD3D9();
	RenderStateHelper::ResetRenderState(d3d9device);

	D3DXMATRIX tmpMatrix = *GetWorldTransform();
	tmpMatrix._42 = 0.1f;
	HRESULT v = m_QuadFxhandle->SetMatrix("g_mWorld", &tmpMatrix);

	RS_HELP_OBJ(d3d9device,D3DRS_ALPHABLENDENABLE, TRUE);
	RS_HELP_OBJ(d3d9device,D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	RS_HELP_OBJ(d3d9device,D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

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
	v = m_QuadFxhandle->SetValue("range", &m_target_range, sizeof(float));

	m_quad_mesh->SetVertexIndex(d3d9device);


	UINT totalPasses;
	m_QuadFxhandle->Begin(&totalPasses, 0);
	m_QuadFxhandle->BeginPass(0);

	m_quad_mesh->DrawMesh(d3d9device);

	m_QuadFxhandle->EndPass();
	m_QuadFxhandle->End();
}
void TargetPoint::Render(unsigned int escapeTime)
{

	RenderQuad();
	RenderCylider();
}

void TargetPoint::RenderCylider()
{
	auto d3d9device = Globals::GetDevice()->GetDeviceD3D9();

	HRESULT v = m_cylinder_fxhandle->SetMatrix("g_mWorld", &m_cylinder_local_matrix);

	//RS_HELP_OBJ(d3d9device,D3DRS_ALPHABLENDENABLE, TRUE);
	//RS_HELP_OBJ(d3d9device,D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//RS_HELP_OBJ(d3d9device,D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//wvp matrix
	auto proj = Globals::GetRender()->GetProjMatrix();
	auto view = Globals::GetRender()->GetViewMatrix();
	D3DXMATRIX vp = (*view)*(*proj);
	v = m_cylinder_fxhandle->SetMatrix("g_mViewProjection", &vp);

	//material
	D3DXCOLOR vWhite = D3DXCOLOR(1, 1, 1, 1);
	v = m_cylinder_fxhandle->SetValue("g_MaterialDiffuseColor", &vWhite, sizeof(D3DXCOLOR));

	//texture
	v = m_cylinder_fxhandle->SetTexture("g_DiffuseTex", m_cylinder_texture.get());

	//range
	float range = 20.f;
	v = m_cylinder_fxhandle->SetValue("range", &range, sizeof(float));

	UINT totalPasses;
	m_cylinder_fxhandle->Begin(&totalPasses, 0);
	m_cylinder_fxhandle->BeginPass(0);

	m_cylinder_mesh->DrawSubset(0);

	m_cylinder_fxhandle->EndPass();
	m_cylinder_fxhandle->End();
}