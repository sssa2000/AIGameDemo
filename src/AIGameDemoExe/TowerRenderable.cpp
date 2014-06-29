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
void TowerRenderable::InitPatchQuadVB(float scale,float terrainHeight)
{
	auto device = Globals::GetDevice()->GetDeviceD3D9();
	HRESULT res = device->CreateVertexBuffer(4 * sizeof(QuadVertex), D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED, &m_pQuadVB, 0);
	if (res != S_OK)
		ReportErr("Quad CreateVB Failed");
	QuadVertex* v = 0;
	m_pQuadVB->Lock(0, 0, (void**)&v, 0);
	v[0] = QuadVertex(-1.0f, terrainHeight, 1.0f, 0.0f, 1.0f);
	v[1] = QuadVertex(-1.0f, terrainHeight, -1.0f, 0.0f, 0.0f);
	v[2] = QuadVertex(1.0f, terrainHeight, -1.0f, 1.0f, 0.0f);
	v[3] = QuadVertex(1.0f, terrainHeight, 1.0f, 1.0f, 1.0f);
	m_pQuadVB->Unlock();
}

void TowerRenderable::InitPatchQuadIB()
{
	auto device = Globals::GetDevice()->GetDeviceD3D9();
	HRESULT res = device->CreateIndexBuffer(6 * sizeof(WORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pQuadIB, 0);
	if (res != S_OK)
		ReportErr("Quad CreateIB Failed");

	WORD* g_Indices = 0;
	m_pQuadIB->Lock(0, 0, (void**)&g_Indices, 0);
	g_Indices[0] = 0; g_Indices[1] = 1; g_Indices[2] = 2;
	g_Indices[3] = 0; g_Indices[4] = 2; g_Indices[5] = 3;
	m_pQuadIB->Unlock();
}
void TowerRenderable::InitPatchQuadVBDeclare()
{
	auto device = Globals::GetDevice()->GetDeviceD3D9();
	D3DVERTEXELEMENT9 vbDecl[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },

		{ 0xFF, 0, D3DDECLTYPE_UNUSED, 0, 0, 0 }
	};
	HRESULT res = device->CreateVertexDeclaration(vbDecl, &m_pQuadVertexDecl);
}
void TowerRenderable::InitPatchQuad(float scale)
{
	auto device = Globals::GetDevice()->GetDeviceD3D9();
	//vb
	InitPatchQuadVB(scale,0);
	//ib
	InitPatchQuadIB();
	//vb dec
	InitPatchQuadVBDeclare();
	//fx
	m_QuadFxhandle = Globals::GetFxManager()->RequireEffectFormFile("../media/fx/terrain_patch.fx");
	m_QuadFxhandle ->SetTechnique("RenderSceneWithTexture1Light");
	//texture
	HRESULT v = D3DXCreateTextureFromFileA(device, "../media/tower/range.tga", &m_range_texture);

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
	tmpMatrix._42 = 0;
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
	v = m_QuadFxhandle->SetTexture("g_DiffuseTex", m_range_texture);

	//range
	float range = pTower->GetRange();
	v = m_QuadFxhandle->SetValue("range", &range, sizeof(float));

	d3d9device->SetStreamSource(0, m_pQuadVB, 0, sizeof(QuadVertex));
	d3d9device->SetVertexDeclaration(m_pQuadVertexDecl);
	d3d9device->SetIndices(m_pQuadIB);

	UINT totalPasses;
	m_QuadFxhandle->Begin(&totalPasses, 0);
	m_QuadFxhandle->BeginPass(0);
	d3d9device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
		0,   // 将要绘制的索引缓冲区的起始地址
		0, //　索引数组中最小的索引值
		4,   // 要绘制的索引数组中的顶点数量
		0, // 从索引数组中的第几个元素开始绘制图元
		2); // 绘制的图元数量
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
	//d3d9device->SetTransform(D3DTS_WORLD, &tmpMatrix);

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