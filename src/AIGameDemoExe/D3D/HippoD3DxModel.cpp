#include "HippoD3DxModel.h"
#include "HippoFilePath.h"
HippoD3DxModel::HippoD3DxModel()
{
	m_pD3dxMesh = 0;
	D3DXMatrixIdentity(&m_matWorld);
}
HippoD3DxModel::~HippoD3DxModel()
{
	if (m_pD3dxMesh)
		m_pD3dxMesh->Release();
}
void HippoD3DxModel::Release()
{
	delete this;
}

void HippoD3DxModel::Update(unsigned int timeMs)
{

}
void HippoD3DxModel::Render(IDirect3DDevice9* device)
{
	device->SetTransform(D3DTS_WORLD, &m_matWorld);
	for (size_t i=0;i<m_mats.size();++i)
	{
		device->SetMaterial( m_mats.at(i)->m_d3d_mat);
		device->SetTexture( 0, m_mats.at(i)->m_d3d_texture );

		m_pD3dxMesh->DrawSubset(i);
	}

}

void HippoD3DxModel::SetPos(float x, float y, float z)
{
	D3DXMatrixTranslation(&m_matWorld, x, y, z);

}
XMaterial::XMaterial(D3DXMATERIAL& pmat)
{
	m_d3d_mat=0;
	m_d3d_texture=0;

	m_d3d_mat=new D3DMATERIAL9;
	*m_d3d_mat=pmat.MatD3D;
	m_d3d_mat->Ambient = m_d3d_mat->Diffuse;
}
XMaterial::~XMaterial()
{
	if(m_d3d_texture)
	{
		m_d3d_texture->Release();
		m_d3d_texture=0;
	}

	if(m_d3d_texture)
	{
		m_d3d_texture->Release();
		m_d3d_texture=0;
	}
}

HippoD3DxModel* HippoD3DxModelFactory::CreateTeapotModel(IDirect3DDevice9* device)
{
	auto* p = new HippoD3DxModel;
	HRESULT v = D3DXCreateTeapot(device, &(p->m_pD3dxMesh), 0);
	return p;
}

HippoD3DxModel* HippoD3DxModelFactory::CreateFromXFile(const char* fn,IDirect3DDevice9* device)
{
	auto* pmodel = new HippoD3DxModel;
	LPD3DXBUFFER pD3DXMtrlBuffer;
	DWORD g_dwNumMaterials;
	HRESULT v=D3DXLoadMeshFromX( fn, D3DXMESH_SYSTEMMEM,
		device, NULL,
		&pD3DXMtrlBuffer, NULL, &g_dwNumMaterials,
		&(pmodel->m_pD3dxMesh) );
	
	//copy material
	D3DXMATERIAL* d3dxMaterials = ( D3DXMATERIAL* )pD3DXMtrlBuffer->GetBufferPointer();
	for (size_t i=0;i<g_dwNumMaterials;++i)
	{
		XMaterial* p = new XMaterial(d3dxMaterials[i]);
		if( d3dxMaterials[i].pTextureFilename != NULL &&
			lstrlenA( d3dxMaterials[i].pTextureFilename ) > 0 )
		{
			std::string texfilename=d3dxMaterials[i].pTextureFilename;
			IDirect3DTexture9* pTex=0;
			HippoFilePath texpath1(fn);
			HippoFilePath texpath2(d3dxMaterials[i].pTextureFilename);
			if(!texpath2.isFileExist())
			{
				std::string f="";
				texpath2.GetFileNameWithExtension(f);
				texfilename=texpath1.GetFileDir();
				texfilename+=f;

			}

			D3DXCreateTextureFromFileA( device,
				texfilename.c_str(),
				&p->m_d3d_texture) ;
		}
		pmodel->m_mats.push_back(p);
	}
	

	return pmodel;
}