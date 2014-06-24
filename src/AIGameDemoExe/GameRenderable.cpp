#include "GameRenderable.h"
#include "GameEntity.h"

#include "HippoD3d9Device.h"
GameRenderable::GameRenderable(GameEntity* parent)
{
	m_parent = parent;
	D3DXMatrixIdentity(&m_local_matrix);
}
GameRenderable::~GameRenderable()
{
	if (m_pD3dxMesh)
	{
		m_pD3dxMesh->Release();
		m_pD3dxMesh = 0;
	}

	for (size_t i = 0; i < m_mats.size();++i)
	{
		delete m_mats[i];
	}
	m_mats.clear();
}
void GameRenderable::Render(HippoD3d9Device* pdevice, unsigned int t)
{
	auto d3d9device = pdevice->GetDeviceD3D9();
	D3DXMATRIX tmpMatrix;
	D3DXMatrixMultiply(&tmpMatrix, &m_local_matrix,m_parent->GetWorldTransform());

	d3d9device->SetTransform(D3DTS_WORLD, &tmpMatrix);
	
	for (size_t i = 0; i < m_mats.size(); ++i)
	{
		d3d9device->SetMaterial(m_mats.at(i)->m_d3d_mat);
		d3d9device->SetTexture(0, m_mats.at(i)->m_d3d_texture);

		m_pD3dxMesh->DrawSubset(i);
	}
}
void GameRenderable::CreateDefaultMat()
{
	D3DXMATERIAL m;
	m.MatD3D.Ambient.r = 0.5;
	m.MatD3D.Ambient.g = 0.5;
	m.MatD3D.Ambient.b = 0.5;
	m.MatD3D.Ambient.a = 0.5;

	m.MatD3D.Diffuse.r = 1;
	m.MatD3D.Diffuse.g = 1;
	m.MatD3D.Diffuse.b = 1;
	m.MatD3D.Diffuse.a = 1;

	m.MatD3D.Emissive.r = 0.5;
	m.MatD3D.Emissive.g = 0.5;
	m.MatD3D.Emissive.b = 0.5;
	m.MatD3D.Emissive.a = 0.5;

	m.MatD3D.Specular.r = 1;
	m.MatD3D.Specular.g = 1;
	m.MatD3D.Specular.b = 1;
	m.MatD3D.Specular.a = 1;

	m.MatD3D.Power= 128;
	sMaterial* p = new sMaterial(m);
	m_mats.push_back(p);
}
//////////////////////////////////////////////////////////////////////////

sMaterial::sMaterial(D3DXMATERIAL& pmat)
{
	m_d3d_mat = 0;
	m_d3d_texture = 0;

	m_d3d_mat = new D3DMATERIAL9;
	*m_d3d_mat = pmat.MatD3D;
	m_d3d_mat->Ambient = m_d3d_mat->Diffuse;
}
sMaterial::~sMaterial()
{
	if (m_d3d_texture)
	{
		m_d3d_texture->Release();
		m_d3d_texture = 0;
	}

	if (m_d3d_texture)
	{
		m_d3d_texture->Release();
		m_d3d_texture = 0;
	}
}

