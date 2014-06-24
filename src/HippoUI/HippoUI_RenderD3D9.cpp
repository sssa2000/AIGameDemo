#include "HippoUI_RenderD3D9.h"
#include "HippoUI_Global.h"
#include "HippoUI_TextureD3D9.h"
#include "ErrReport.h"
#include "d3dx9.h"
#include "d3d9.h"

//绘制rect用的顶点数据
struct HIPPOUI_SCREEN_VERTEX
{
	float x, y, z, h;
	D3DCOLOR color;
	float tu, tv;
	static DWORD FVF;
};
DWORD HIPPOUI_SCREEN_VERTEX::FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;

//绘制rect用的顶点数据（无纹理坐标）
struct HIPPOUI_SCREEN_VERTEX_UNTEX
{
	float x, y, z, h;
	D3DCOLOR color;
	static DWORD FVF;
};
DWORD HIPPOUI_SCREEN_VERTEX_UNTEX::FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE;


HippoUI_RenderD3D9::HippoUI_RenderD3D9(void* pDevice):
HippoUI_Render()
{
	m_pd3dDevice=(IDirect3DDevice9*)pDevice;
	Init();
}

HippoUI_RenderD3D9::~HippoUI_RenderD3D9()
{
	Destroy();
}

void HippoUI_RenderD3D9::Init()
{
	
	//create sprite
	HRESULT hr = D3DXCreateSprite( m_pd3dDevice, &m_pSprite );
	if( FAILED( hr ) )
	{
		m_pSprite=0;
		ReportErr("D3DXCreateSprite Failed");
	}
	const int default_font_h=10;
	hr = D3DXCreateFont( m_pd3dDevice,
		16,					// Height
		0,                     // Width
		FW_NORMAL,               // Weight
		1,                     // MipLevels, 0 = autogen mipmaps
		FALSE,                 // Italic
		DEFAULT_CHARSET,       // CharSet
		OUT_DEFAULT_PRECIS,    // OutputPrecision
		DEFAULT_QUALITY,       // Quality
		DEFAULT_PITCH | FF_DONTCARE, // PitchAndFamily
		"Arial",              // pFaceName
		&m_pFont );              // ppFont
	if( FAILED( hr ) )
	{
		m_pFont=0;
		ReportErr("D3DXCreateFont Failed");
	}
}

void HippoUI_RenderD3D9::OnLostDevice()
{
	if(m_pSprite)
	{
		m_pSprite->OnLostDevice();
	}

	if(m_pFont)
	{
		m_pFont->OnLostDevice();
	}
}

void HippoUI_RenderD3D9::OnResetDevice()
{
	if(m_pSprite)
	{
		m_pSprite->OnResetDevice();
	}

	if(m_pFont)
	{
		m_pFont->OnResetDevice();
	}
}

void HippoUI_RenderD3D9::Destroy()
{
	if(m_pSprite)
		m_pSprite->Release();
	m_pSprite=0;

	if(m_pFont)
		m_pFont->Release();
	m_pFont=0;
}

void HippoUI_RenderD3D9::BeginRender()
{
	if(m_pSprite)
	{
		//m_pSprite->Begin(D3DXSPRITE_DONOTSAVESTATE);
		m_pSprite->Begin( D3DXSPRITE_ALPHABLEND  );

	}
}

void HippoUI_RenderD3D9::EndRender()
{
	if(m_pSprite)
	{
		//m_pSprite->Flush();
		m_pSprite->End();
	}
}

void HippoUI_RenderD3D9::DrawSprite(HippoUI_Rect& rect,HippoUI_TextureProxy* pTexProxy,HippoUi_Color& color)
{
	//return;
	IDirect3DTexture9* pTex=(IDirect3DTexture9*)(pTexProxy->GetHardwareTexturePtr());
	//calc scale
	D3DXMATRIXA16 matTransform;
	D3DXMatrixIdentity(&matTransform);

	float scale_x=( float )rect.GetWidth()/pTexProxy->GetRect().GetWidth();
	float scale_y=( float )rect.GetHeight()/pTexProxy->GetRect().GetHeight();
	D3DXMatrixScaling( &matTransform, scale_x, scale_y, 1.0f );
	m_pSprite->SetTransform(&matTransform);
	
	D3DXVECTOR3 vPos( ( float )rect.left, ( float )rect.top, 0.0f );
	vPos.x /= scale_x;
	vPos.y /= scale_y;

	HRESULT res=m_pSprite->Draw(pTex,pTexProxy->GetRect().GetRECT(),0,&vPos,D3DXCOLOR( color.r, color.g, color.b, color.a ));
	if(FAILED(res))
	{
		ReportErr("sprite draw failed");
	}
}

int ConvertTextAG2D3dxFont(TEXT_AG ag)
{
	if(ag==TEXT_AG_CENTER)
		return DT_VCENTER|DT_CENTER;
	else if(ag==TEXT_AG_LEFT)
		return DT_VCENTER|DT_LEFT;
	else if(TEXT_AG_AUTO==TEXT_AG_AUTO)
		return DT_LEFT|DT_WORDBREAK;
	else
		return DT_VCENTER|DT_LEFT;
}
void HippoUI_RenderD3D9::DrawString(HippoUI_Rect& rect,const char* str,HippoUi_Color& color,TEXT_AG ag)
{
	D3DXMATRIX matTransform;
	D3DXMatrixIdentity( &matTransform );
	m_pSprite->SetTransform( &matTransform );

	HRESULT res = m_pFont->DrawTextA(
		m_pSprite, 
		str, 
		-1, 
		&rect, //rect
		ConvertTextAG2D3dxFont(ag),//Format
		D3DXCOLOR( color.r, color.g, color.b, color.a ) );

	if(FAILED(res))
	{
		ReportErr("DrawString draw failed");
	}
}
void HippoUI_RenderD3D9::DrawString(HippoUI_Rect& rect,const WCHAR* str,HippoUi_Color& color,TEXT_AG ag)
{
	D3DXMATRIX matTransform;
	D3DXMatrixIdentity( &matTransform );
	m_pSprite->SetTransform( &matTransform );

	HRESULT res = m_pFont->DrawTextW(
		m_pSprite, 
		str, 
		-1, 
		&rect, //rect
		ConvertTextAG2D3dxFont(ag),//Format
		D3DXCOLOR( color.r, color.g, color.b, color.a ) );

	if(FAILED(res))
	{
		ReportErr("DrawString draw failed");
	}
}

void HippoUI_RenderD3D9::DrawRect(HippoUI_Rect&rect,HippoUi_Color& color)
{
	HIPPOUI_SCREEN_VERTEX_UNTEX vertices[4] =
	{
		( float )rect.left - 0.5f,  ( float )rect.top - 0.5f,    0.5f, 1.0f, D3DXCOLOR( color.r, color.g, color.b, color.a ),
		( float )rect.right - 0.5f, ( float )rect.top - 0.5f,    0.5f, 1.0f, D3DXCOLOR( color.r, color.g, color.b, color.a ),
		( float )rect.right - 0.5f, ( float )rect.bottom - 0.5f, 0.5f, 1.0f, D3DXCOLOR( color.r, color.g, color.b, color.a ),
		( float )rect.left - 0.5f,  ( float )rect.bottom - 0.5f, 0.5f, 1.0f, D3DXCOLOR( color.r, color.g, color.b, color.a ),
	};

	// Since we're doing our own drawing here we need to flush the sprites
	m_pSprite->Flush();
	//IDirect3DVertexDeclaration9* pDecl = NULL;
	//m_pd3dDevice->GetVertexDeclaration( &pDecl );  // Preserve the sprite's current vertex decl
	m_pd3dDevice->SetFVF( HIPPOUI_SCREEN_VERTEX_UNTEX::FVF );
	m_pd3dDevice->SetTexture(0,0);
	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,color.a<1.0f?TRUE:FALSE);
	m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
	m_pd3dDevice->SetRenderState(D3DRS_ZENABLE,FALSE);
	m_pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, vertices, sizeof( HIPPOUI_SCREEN_VERTEX_UNTEX ) );

	// Restore the vertex decl
	//m_pd3dDevice->SetVertexDeclaration( pDecl );
	//pDecl->Release();
}

HDC HippoUI_RenderD3D9:: GetFontDC()
{
	if (m_pFont)
	{
		return m_pFont->GetDC();
	}
	return 0;
}