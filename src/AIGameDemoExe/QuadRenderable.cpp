#include "QuadRenderable.h"
#include "Globals.h"
#include "HippoD3d9Device.h"
#include "ErrReport.h"
QuadMesh::QuadMesh()
{

}
QuadMesh::~QuadMesh()
{

}

void QuadMesh::SetVertexIndex(IDirect3DDevice9* d3d9device)
{
	d3d9device->SetStreamSource(0, m_pQuadVB, 0, sizeof(QuadVertex));
	d3d9device->SetVertexDeclaration(m_pQuadVertexDecl);
	d3d9device->SetIndices(m_pQuadIB);
}
void QuadMesh::DrawMesh(IDirect3DDevice9* d3d9device)
{
	d3d9device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
		0,   // 将要绘制的索引缓冲区的起始地址
		0, //　索引数组中最小的索引值
		4,   // 要绘制的索引数组中的顶点数量
		0, // 从索引数组中的第几个元素开始绘制图元
		2); // 绘制的图元数量
}

void QuadMesh::InitPatchQuadVB(float terrainHeight)
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

void QuadMesh::Init(float terrainHeight)
{
	InitPatchQuadVB(terrainHeight);
	InitPatchQuadIB();
	InitPatchQuadVBDeclare();
}
void QuadMesh::InitPatchQuadIB()
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


void QuadMesh::InitPatchQuadVBDeclare()
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