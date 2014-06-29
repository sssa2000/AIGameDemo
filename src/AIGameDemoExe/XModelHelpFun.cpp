#include "XModelHelpFun.h"
#include <d3dx9.h>
#include <string>
#include <vector>
#include "BoundingVol.h"
#include "GameRenderable.h"
#include "HippoFilePath.h"

bool ComputeBoundingSphere(ID3DXMesh* mesh, sBoundingSphere* sphere)
{
	HRESULT hr = 0;
	BYTE* v = 0;
	mesh->LockVertexBuffer(0, (void**)&v);
	hr = D3DXComputeBoundingSphere(
		(D3DXVECTOR3*)v,
		mesh->GetNumVertices(),
		D3DXGetFVFVertexSize(mesh->GetFVF()),
		&sphere->_center,
		&sphere->_radius);
	mesh->UnlockVertexBuffer();
	if (FAILED(hr))
		return false;
	return true;
}
bool ComputeBoundingBox(ID3DXMesh* mesh, sBoundingBox* box)
{
	HRESULT hr = 0;
	BYTE* v = 0;
	mesh->LockVertexBuffer(0, (void**)&v);
	hr = D3DXComputeBoundingBox(
		(D3DXVECTOR3*)v,
		mesh->GetNumVertices(),
		D3DXGetFVFVertexSize(mesh->GetFVF()),
		&box->_min,
		&box->_max);
	mesh->UnlockVertexBuffer();
	if (FAILED(hr))
		return false;
	return true;
}

void FillRenderableMeshWithXFile(const char* fn, IDirect3DDevice9* device, ID3DXMesh** pMesh)
{
	LPD3DXBUFFER pD3DXMtrlBuffer;
	DWORD g_dwNumMaterials;
	HRESULT v = D3DXLoadMeshFromX(fn, D3DXMESH_SYSTEMMEM,
		device, NULL,
		&pD3DXMtrlBuffer, NULL, &g_dwNumMaterials,
		pMesh);

	
}
