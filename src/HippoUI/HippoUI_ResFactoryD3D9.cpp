#include "HippoUI_ResFactoryD3D9.h"
#include "HippoUI_TextureD3D9.h"
#include "HippoUI_Global.h"
#include "d3dx9.h"
#include "d3d9.h"

HippoUI_TextureResource* HippoUI_ResFactoryD3D9::CreateTextureFromFile(const char* filename)
{
	HippoUI_TextureD3D9* pTex=new HippoUI_TextureD3D9();
	IDirect3DDevice9* pDevice=HippoUI_Global::GetIns()->GetDevice9();
	HRESULT hr = D3DXCreateTextureFromFileEx( pDevice, filename, D3DX_DEFAULT, D3DX_DEFAULT,
		1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
		D3DX_DEFAULT, D3DX_DEFAULT, 0,
		&pTex->m_info, NULL, &pTex->m_pTextureD3D9 );

	return pTex;
}

HippoUI_ShaderResource* HippoUI_ResFactoryD3D9::CreateShaderFromFile(const char* filename)
{
	return 0;
}