#include "RobotRenderable.h"
#include "RobotEntity.h"
#include "HippoD3d9Device.h"
#include "Globals.h"
#include "XModelHelpFun.h"
#include "TankGameRender.h"
RobotRenderable::RobotRenderable(GameEntity* parent):
GameRenderable(parent)
{
	m_d3d_texture=0;
	
}
RobotRenderable::~RobotRenderable()
{

	m_d3d_texture->Release();
}

void RobotRenderable::BuildRenderable(GameEntity* parent)
{
	auto d3d9device = Globals::GetDevice()->GetDeviceD3D9();
	FillRenderableMeshWithXFile("../media/btankcruiser_tex.x", d3d9device, &m_pD3dxMesh);

	m_fxhandle=Globals::GetFxManager()->RequireEffectFormFile("../media/fx/default_fx.fx");
	m_fxhandle->SetTechnique("RenderSceneWithTexture1Light");

	HRESULT v=D3DXCreateTextureFromFileA(d3d9device,"../media/btankcruiser_tex.png",&m_d3d_texture);

}


void RobotRenderable::Render(HippoD3d9Device* pdevice, unsigned int escapeTime)
{
	CalcPos(escapeTime);
	auto device = Globals::GetDevice()->GetDeviceD3D9();

	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	device->SetRenderState(D3DRS_ZWRITEENABLE, true);
	device->SetRenderState(D3DRS_ZENABLE, true);
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	//world matrix
	HRESULT v = m_fxhandle->SetMatrix("g_mWorld", &m_RobotRenderableMatrix);

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

	for (iPass = 0; iPass < totalPasses; iPass++)
	{
		m_fxhandle->BeginPass(iPass);
		m_pD3dxMesh->DrawSubset(0);
		m_fxhandle->EndPass();
	}

	m_fxhandle->End();


	
}

void RobotRenderable::CalcPos(unsigned int escapet)
{
	D3DXMATRIX* m1=m_parent->GetWorldTransform();
	//D3DXMatrixMultiply(&m_RobotRenderableMatrix, &m_local_matrix, m_parent->GetWorldTransform());
	const float fly_height = 15.f;
	m_RobotRenderableMatrix = *m1;
	DWORD t=timeGetTime();
	m_RobotRenderableMatrix._42 = fly_height  + 1.6f*sin(0.1f*D3DXToRadian(t));
	
	//scale it
	m_RobotRenderableMatrix._11 = 0.1f;
	m_RobotRenderableMatrix._22 = 0.1f;
	m_RobotRenderableMatrix._33 = 0.1f;
}

void RobotRenderable::LoadFromFile()
{

}