#include "SkyEntity.h"
#include "Globals.h"
#include "GameRenderable.h"
#include "HippoD3d9Device.h"
#include "GameRenderable.h"
#include "FxEffectManager.h"
#include "TankGameRender.h"
#include "IGameWordContex.h"
#include <d3d9.h>
#include <d3d9types.h>
#include "ErrReport.h"
#include "CameraBase.h"
#include <iostream>
SkyEntity::SkyEntity() :
GameEntity()
{
	m_pRenderable = 0;
	InitRenderable();
}
SkyEntity::~SkyEntity()
{
	delete m_pRenderable;
}
void SkyEntity::InitRenderable()
{
	m_pRenderable = new SkyRenderable(this);
}

void SkyEntity::Render(unsigned int escapeTime)
{
	auto d3ddevice = Globals::GetDevice();
	m_pRenderable->Render(d3ddevice, escapeTime);
}

//////////////////////////////////////////////////////////////////////////
//天空盒的顶点
struct SkyBoxVertex
{
	SkyBoxVertex(float x, float y, float z, float u, float v)
	: m_pos(x, y, z), m_tex0(u, v)
	{
	}

	D3DXVECTOR3 m_pos;
	D3DXVECTOR2 m_tex0;
};

SkyRenderable::SkyRenderable(GameEntity* p)
{
	m_parent = p;
	D3DXMatrixIdentity(&m_local_matrix);
	LoadFromFile();
}
SkyRenderable::~SkyRenderable()
{

}
void SkyRenderable::Render(HippoD3d9Device* pdevice, unsigned int escapeTime)
{
	auto device = Globals::GetDevice()->GetDeviceD3D9();

	device->SetRenderState(D3DRS_ZWRITEENABLE, false);
	device->SetRenderState(D3DRS_ZENABLE, false);
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	//skybox位置
	const D3DXVECTOR3* campos = Globals::GetCurrentCamera()->GetPos();
	m_local_matrix._41 = campos->x;
	m_local_matrix._42 = campos->y;
	m_local_matrix._43 = campos->z;
	HRESULT v = m_fxhandle->SetMatrix("g_mWorld", &m_local_matrix);

	//wvp matrix
	auto proj = Globals::GetRender()->GetProjMatrix();
	auto view = Globals::GetRender()->GetViewMatrix();
	D3DXMATRIX vp = (*view)*(*proj);
	v = m_fxhandle->SetMatrix("g_mViewProjection", &vp);


	device->SetStreamSource(0, m_pVB, 0, sizeof(SkyBoxVertex));
	device->SetVertexDeclaration(m_pVertexDecl);
	device->SetIndices(m_pIB);

	UINT iPass, totalPasses;
	m_fxhandle->Begin(&totalPasses, 0);
	m_fxhandle->BeginPass(0);

	for (int i = 0; i < 6; ++i)
	{
		//texture
		v = m_fxhandle->SetTexture("g_MeshTexture", m_pTextrue[i]);
		m_fxhandle->CommitChanges();
		device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
			0,   // 将要绘制的索引缓冲区的起始地址
			i * 4, //　索引数组中最小的索引值
			4,   // 要绘制的索引数组中的顶点数量
			i * 6, // 从索引数组中的第几个元素开始绘制图元
			2); // 绘制的图元数量
	}

	//device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
	//	0,   // 将要绘制的索引缓冲区的起始地址
	//	0, //　索引数组中最小的索引值
	//	24,   // 要绘制的索引数组中的顶点数量
	//	0, // 从索引数组中的第几个元素开始绘制图元
	//	12); // 绘制的图元数量

	m_fxhandle->EndPass();
	m_fxhandle->End();

	device->SetRenderState(D3DRS_ZWRITEENABLE, true);
	device->SetRenderState(D3DRS_ZENABLE, true);
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);

}
void SkyRenderable::LoadFx()
{
	m_fxhandle = Globals::GetFxManager()->RequireEffectFormFile("../media/fx/sky.hlsl");
	m_fxhandle->SetTechnique("RenderSceneWithTexture1Light");

}
void SkyRenderable::LoadFromFile()
{

	LoadTexture();
	MakeVertexBuffer();
	MakeIndexBuffer();
	MakeVertexDeclaration();
	LoadFx();
}

void SkyRenderable::LoadTexture()
{
	auto device = Globals::GetDevice()->GetDeviceD3D9();
	HRESULT v = S_OK;
	//v += D3DXCreateTextureFromFileA(device, "../media/skybox/left.tga", &m_pTextrue[0]);
	//v += D3DXCreateTextureFromFileA(device, "../media/skybox/right.tga", &m_pTextrue[1]);
	//v += D3DXCreateTextureFromFileA(device, "../media/skybox/clouds.tga", &m_pTextrue[2]);
	//v += D3DXCreateTextureFromFileA(device, "../media/skybox/Bottom.tga", &m_pTextrue[3]);
	//v += D3DXCreateTextureFromFileA(device, "../media/skybox/Front.tga", &m_pTextrue[4]);
	//v += D3DXCreateTextureFromFileA(device, "../media/skybox/Back.tga", &m_pTextrue[5]);

	v += D3DXCreateTextureFromFileA(device, "../media/skybox/2/left.tga", &m_pTextrue[0]);
	v += D3DXCreateTextureFromFileA(device, "../media/skybox/2/right.tga", &m_pTextrue[1]);
	v += D3DXCreateTextureFromFileA(device, "../media/skybox/2/top.tga", &m_pTextrue[2]);
	v += D3DXCreateTextureFromFileA(device, "../media/skybox/2/Bottom.tga", &m_pTextrue[3]);
	v += D3DXCreateTextureFromFileA(device, "../media/skybox/2/Back.tga", &m_pTextrue[4]);
	v += D3DXCreateTextureFromFileA(device, "../media/skybox/2/Front.tga", &m_pTextrue[5]);

	//v += D3DXCreateTextureFromFileA(device, "../media/skybox/sky_side.dds", &m_pTextrue[0]);
	//m_pTextrue[1] = m_pTextrue[0];
	//v += D3DXCreateTextureFromFileA(device, "../media/skybox/sky_top.dds", &m_pTextrue[2]);
	//v += D3DXCreateTextureFromFileA(device, "../media/skybox/sky_bot.dds", &m_pTextrue[3]);
	//m_pTextrue[4] = m_pTextrue[0];
	//m_pTextrue[5] = m_pTextrue[0];

	if (v != S_OK)
		ReportErr("SkyBox create texture Failed");

}

void SkyRenderable::MakeVertexDeclaration()
{
	auto device = Globals::GetDevice()->GetDeviceD3D9();
	D3DVERTEXELEMENT9 vbDecl[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },

		{ 0xFF, 0, D3DDECLTYPE_UNUSED, 0, 0, 0 }
	};
	HRESULT res = device->CreateVertexDeclaration(vbDecl, &m_pVertexDecl);
}

void SkyRenderable::MakeIndexBuffer()
{
	auto device = Globals::GetDevice()->GetDeviceD3D9();

	HRESULT res = device->CreateIndexBuffer(36 * sizeof(WORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB, 0);
	if (res != S_OK)
		ReportErr("SkyBox CreateIB Failed");

	WORD* g_Indices = 0;
	m_pIB->Lock(0, 0, (void**)&g_Indices, 0);
	// positive x
	g_Indices[0] = 0; g_Indices[1] = 1; g_Indices[2] = 2;
	g_Indices[3] = 0; g_Indices[4] = 2; g_Indices[5] = 3;
	// negative x
	g_Indices[6] = 4; g_Indices[7] = 5; g_Indices[8] = 6;
	g_Indices[9] = 4; g_Indices[10] = 6; g_Indices[11] = 7;
	// positive y
	g_Indices[12] = 8; g_Indices[13] = 9; g_Indices[14] = 10;
	g_Indices[15] = 8; g_Indices[16] = 10; g_Indices[17] = 11;
	// negative y
	g_Indices[18] = 12; g_Indices[19] = 13; g_Indices[20] = 14;
	g_Indices[21] = 12; g_Indices[22] = 14; g_Indices[23] = 15;
	// positive z
	g_Indices[24] = 16; g_Indices[25] = 17; g_Indices[26] = 18;
	g_Indices[27] = 16; g_Indices[28] = 18; g_Indices[29] = 19;
	// negative z
	g_Indices[30] = 20; g_Indices[31] = 21; g_Indices[32] = 22;
	g_Indices[33] = 20; g_Indices[34] = 22; g_Indices[35] = 23;
	m_pIB->Unlock();
}

void SkyRenderable::MakeVertexBuffer()
{
	auto device = Globals::GetDevice()->GetDeviceD3D9();
	HRESULT res = device->CreateVertexBuffer(24 * sizeof(SkyBoxVertex), D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED, &m_pVB, 0);
	if (res != S_OK)
		ReportErr("SkyBox CreateVB Failed");
	SkyBoxVertex* v = 0;
	float scale = 500.0f;
	m_pVB->Lock(0, 0, (void**)&v, 0);
	// positive x
	v[0] = SkyBoxVertex(1.0f*scale, -1.0f*scale, 1.0f*scale, 0.0f, 1.0f);
	v[1] = SkyBoxVertex(1.0f*scale, 1.0f*scale, 1.0f*scale, 0.0f, 0.0f);
	v[2] = SkyBoxVertex(1.0f*scale, 1.0f*scale, -1.0f*scale, 1.0f, 0.0f);
	v[3] = SkyBoxVertex(1.0f*scale, -1.0f*scale, -1.0f*scale, 1.0f, 1.0f);
	// negative x
	v[4] = SkyBoxVertex(-1.0f*scale, -1.0f*scale, -1.0f*scale, 0.0f, 1.0f);
	v[5] = SkyBoxVertex(-1.0f*scale, 1.0f*scale, -1.0f*scale, 0.0f, 0.0f);
	v[6] = SkyBoxVertex(-1.0f*scale, 1.0f*scale, 1.0f*scale, 1.0f, 0.0f);
	v[7] = SkyBoxVertex(-1.0f*scale, -1.0f*scale, 1.0f*scale, 1.0f, 1.0f);
	// positive y
	v[8] = SkyBoxVertex(-1.0f*scale, 1.0f*scale, 1.0f*scale, 0.0f, 1.0f);
	v[9] = SkyBoxVertex(-1.0f*scale, 1.0f*scale, -1.0f*scale, 0.0f, 0.0f);
	v[10] = SkyBoxVertex(1.0f*scale, 1.0f*scale, -1.0f*scale, 1.0f, 0.0f);
	v[11] = SkyBoxVertex(1.0f*scale, 1.0f*scale, 1.0f*scale, 1.0f, 1.0f);
	// negative y
	v[12] = SkyBoxVertex(-1.0f*scale, -1.0f*scale, -1.0f*scale, 0.0f, 1.0f);
	v[13] = SkyBoxVertex(-1.0f*scale, -1.0f*scale, 1.0f*scale, 0.0f, 0.0f);
	v[14] = SkyBoxVertex(1.0f*scale, -1.0f*scale, 1.0f*scale, 1.0f, 0.0f);
	v[15] = SkyBoxVertex(1.0f*scale, -1.0f*scale, -1.0f*scale, 1.0f, 1.0f);
	// positive z
	v[16] = SkyBoxVertex(-1.0f*scale, -1.0f*scale, 1.0f*scale, 0.0f, 1.0f);
	v[17] = SkyBoxVertex(-1.0f*scale, 1.0f*scale, 1.0f*scale, 0.0f, 0.0f);
	v[18] = SkyBoxVertex(1.0f*scale, 1.0f*scale, 1.0f*scale, 1.0f, 0.0f);
	v[19] = SkyBoxVertex(1.0f*scale, -1.0f*scale, 1.0f*scale, 1.0f, 1.0f);
	// negative z
	v[20] = SkyBoxVertex(1.0f*scale, -1.0f*scale, -1.0f*scale, 0.0f, 1.0f);
	v[21] = SkyBoxVertex(1.0f*scale, 1.0f*scale, -1.0f*scale, 0.0f, 0.0f);
	v[22] = SkyBoxVertex(-1.0f*scale, 1.0f*scale, -1.0f*scale, 1.0f, 0.0f);
	v[23] = SkyBoxVertex(-1.0f*scale, -1.0f*scale, -1.0f*scale, 1.0f, 1.0f);
	m_pVB->Unlock();

}


//////////////////////////////////////////////////////////////////////////

struct SkyDomeVertex
{
	SkyDomeVertex(float x, float y, float z, float u, float v,D3DXCOLOR& c)
	: m_pos(x, y, z), m_tex0(u, v), m_color(c)
	{
	}

	D3DXVECTOR3 m_pos;
	D3DXVECTOR2 m_tex0;
	D3DXCOLOR m_color;
};

SkyDoomEntity::SkyDoomEntity() :
GameEntity()
{
	m_pRenderable = 0;
	InitRenderable();
}
SkyDoomEntity::~SkyDoomEntity()
{
	delete m_pRenderable;
}
void SkyDoomEntity::InitRenderable()
{
	m_pRenderable = new SkyDoomRenderable(this, 50, 50, 30);
}
void SkyDoomEntity::Render(unsigned int escapeTime)
{
	auto d3ddevice = Globals::GetDevice();
	m_pRenderable->Render(d3ddevice, escapeTime);
}

//////////////////////////////////////////////////////////////////////////

SkyDoomRenderable::SkyDoomRenderable(GameEntity* p, float Radius, int xy_num, int y_num)
{
	m_parent = p;
	m_Radius = Radius;
	xz_ver_num = xy_num;
	y_ver_num = y_num;
	D3DXMatrixIdentity(&m_local_matrix);
	LoadFromFile();
}
SkyDoomRenderable::~SkyDoomRenderable()
{

}
void SkyDoomRenderable::Render(HippoD3d9Device* pdevice, unsigned int escapeTime)
{
	auto device = Globals::GetDevice()->GetDeviceD3D9();
	//device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	device->SetRenderState(D3DRS_ZWRITEENABLE, false);
	device->SetRenderState(D3DRS_ZENABLE, false);

	//skybox位置
	const D3DXVECTOR3* campos = Globals::GetCurrentCamera()->GetPos();
	m_local_matrix._41 = campos->x;
	m_local_matrix._42 = campos->y;
	m_local_matrix._43 = campos->z;
	HRESULT v = m_fxhandle->SetMatrix("g_mWorld", &m_local_matrix);

	//wvp matrix
	auto proj = Globals::GetRender()->GetProjMatrix();
	auto view = Globals::GetRender()->GetViewMatrix();
	D3DXMATRIX vp = (*view)*(*proj);
	v = m_fxhandle->SetMatrix("g_mViewProjection", &vp);

	//texture
	v = m_fxhandle->SetTexture("g_MeshTexture", m_pTextrue[0]);
	v = m_fxhandle->SetTexture("g_CloudTexture", m_pTextrue[1]);

	//time
	float t = GetTickCount()*0.001f;
	v = m_fxhandle->SetFloat("g_fTime",t);

	device->SetStreamSource(0, m_pVB, 0, sizeof(SkyDomeVertex));
	device->SetVertexDeclaration(m_pVertexDecl);
	device->SetIndices(m_pIB);

	UINT iPass, totalPasses;
	m_fxhandle->Begin(&totalPasses, 0);
	m_fxhandle->BeginPass(0);
	//device->DrawPrimitive(D3DPT_POINTLIST, 0, GetVertexCount());
	device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
		0,   // 将要绘制的索引缓冲区的起始地址
		0, //　索引数组中最小的索引值
		GetVertexCount(),   // 要绘制的索引数组中的顶点数量
		0, // 从索引数组中的第几个元素开始绘制图元
		GetTriangleCount()); // 绘制的图元数量

	m_fxhandle->EndPass();
	m_fxhandle->End();

	device->SetRenderState(D3DRS_ZWRITEENABLE, true);
	device->SetRenderState(D3DRS_ZENABLE, true);
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}
D3DXCOLOR SkyDoomRenderable::CalcVertexColor(int yidx,int xz_idx)
{
	D3DXCOLOR out;
	float halfPI = D3DX_PI / 2;
	float SliceAng = (D3DX_PI / 2) / y_ver_num;
	float SideAng = (D3DX_PI * 2) / xz_ver_num;

	float theta, phi;
	int index = 0;
	float SunTheta = 1.0119f;
	float SunPhi = 0.947f;
	float turbidity = 2.0f;

	m_SkyColor.SetInfo(SunTheta, turbidity);

	theta = halfPI - SliceAng * yidx;
	phi = xz_idx * SideAng;

	m_SkyColor.GetVertexColor(theta, phi, SunPhi, &out);
	return out;
}

void SkyDoomRenderable::MakeVertexBuffer()
{
	auto device = Globals::GetDevice()->GetDeviceD3D9();
	HRESULT res = device->CreateVertexBuffer(GetVertexCount() * sizeof(SkyDomeVertex), D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED, &m_pVB, 0);
	if (res != S_OK)
		ReportErr("SkyBox CreateVB Failed");
	SkyDomeVertex* v = 0;
	m_pVB->Lock(0, 0, (void**)&v, 0);
	const float y_offset = -19.f;
	const float dampening = 0.7f;
	int idx = 0;

	for (int j = 0; j < y_ver_num+1; ++j)
	{
		float y_percent = (float)j / (y_ver_num);
		float delta2 = y_percent*0.5f*D3DX_PI;
		float sin_delta2 = sin(delta2);
		float cos_delta2 = cos(delta2);
		for (int i = 0; i < xz_ver_num+1; ++i)
		{
			float x_percent = (float)i / (xz_ver_num);
			float delta1 = x_percent * (2 * D3DX_PI);
			v[idx++] = SkyDomeVertex(
				m_Radius*cos(delta1)*cos_delta2,
				m_Radius*sin_delta2*dampening + y_offset,
				m_Radius*sin(delta1)*cos_delta2,
				x_percent,
				y_percent,
				CalcVertexColor(j,i));
		}

	}


	m_pVB->Unlock();
}
void SkyDoomRenderable::MakeIndexBufferTriangleStrip()
{
	auto device = Globals::GetDevice()->GetDeviceD3D9();
	int n=GetIndexCount_TriangleStrip();
	HRESULT res = device->CreateIndexBuffer(n * sizeof(WORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB, 0);
	if (res != S_OK)
		ReportErr("SkyDoom CreateIB Failed");

	WORD* g_Indices = 0;
	res = m_pIB->Lock(0, 0, (void**)&g_Indices, 0);
	int idx = 0;
	for (int y = 1; y < y_ver_num+1; ++y)
	{
		for (int x = 0; x < xz_ver_num + 1; ++x)
		{
			g_Indices[idx++] = x + (y + 0)*(xz_ver_num+1);
			g_Indices[idx++] = x + (y - 1)*(xz_ver_num+1);

		}
	}


	res = m_pIB->Unlock();
}
void SkyDoomRenderable::MakeIndexBuffer()
{
	auto device = Globals::GetDevice()->GetDeviceD3D9();
	int n=GetIndexCount_TriangleList();
	HRESULT res = device->CreateIndexBuffer(n * sizeof(WORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB, 0);
	if (res != S_OK)
		ReportErr("SkyDoom CreateIB Failed");

	WORD* g_Indices = 0;
	res = m_pIB->Lock(0, 0, (void**)&g_Indices, 0);
	int idx = 0;
	for (int y = 0; y < y_ver_num; ++y)
	{
		for (int x = 0; x < xz_ver_num+1; ++x)
		{
			g_Indices[idx++] = (x + 0) + (y + 0)*(xz_ver_num + 1);
			g_Indices[idx++] = (x + 0) + (y + 1)*(xz_ver_num + 1);
			g_Indices[idx++] = (x + 1) + (y + 1)*(xz_ver_num + 1);
											 
			g_Indices[idx++] = (x + 0) + (y + 0)*(xz_ver_num + 1);
			g_Indices[idx++] = (x + 1) + (y + 1)*(xz_ver_num + 1);
			g_Indices[idx++] = (x + 1) + (y + 0)*(xz_ver_num + 1);
		}
	}

	//g_Indices[0] = 0; g_Indices[1] = 1; g_Indices[2] = 2;
	//g_Indices[3] = 0; g_Indices[4] = 2; g_Indices[5] = 3;
	res = m_pIB->Unlock();
}
void SkyDoomRenderable::MakeVertexDeclaration()
{
	auto device = Globals::GetDevice()->GetDeviceD3D9();
	D3DVERTEXELEMENT9 vbDecl[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		{ 0, 20, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
		{ 0xFF, 0, D3DDECLTYPE_UNUSED, 0, 0, 0 }
	};
	HRESULT res = device->CreateVertexDeclaration(vbDecl, &m_pVertexDecl);

}
void SkyDoomRenderable::LoadTexture()
{
	auto device = Globals::GetDevice()->GetDeviceD3D9();
	HRESULT v = D3DXCreateTextureFromFileExA(device, "../media/skydome/horizon.png",
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		1, //mip level
		0,
		D3DFMT_FROM_FILE,
		D3DPOOL_MANAGED,
		D3DX_DEFAULT, //filter
		D3DX_DEFAULT, //mip filter
		0,
		0,
		0,
		&m_pTextrue[0]);
	v = D3DXCreateTextureFromFileA(device, "../media/skydome/clouds.png", &m_pTextrue[1]);


}
void SkyDoomRenderable::LoadFromFile()
{
	MakeVertexDeclaration();

	LoadTexture();
	MakeVertexBuffer();
	MakeIndexBuffer();
	//MakeIndexBufferTriangleStrip();
	LoadFx();
}
void SkyDoomRenderable::LoadFx()
{
	m_fxhandle = Globals::GetFxManager()->RequireEffectFormFile("../media/fx/skydome.hlsl");
	m_fxhandle->SetTechnique("RenderSceneWithTexture1Light");

}
int SkyDoomRenderable::GetVertexCount()
{
	return (xz_ver_num+1)* (y_ver_num+1);
}
int SkyDoomRenderable::GetTriangleCount()
{
	return (xz_ver_num+1 )* (y_ver_num) * 2; 
}
int SkyDoomRenderable::GetIndexCount_TriangleList()
{
	return 3 * GetTriangleCount();
}

int SkyDoomRenderable::GetIndexCount_TriangleStrip()
{
	return (xz_ver_num + 1)* (y_ver_num)* 2;
}


//////////////////////////////////////////////////////////////////////////


#define SQR(x) ( (x) * (x) )
#define PI		(3.14159265359f)

CSkyColor::CSkyColor()
{
	suntheta = 0.0f;
	T = 0.0f;
}

float CSkyColor::PerezFunction_x(float cosTheta, float cosGamma, float gamma)
{
	float val1 = (1 + Ax * exp(Bx / cosTheta)) * (1 + Cx * exp(Dx * gamma) + Ex * SQR(cosGamma));
	float val2 = (1 + Ax * exp(Bx)) * (1 + Cx * exp(Dx * suntheta) + Ex * SQR(cosSTheta));

	return val1 / val2;
}

float CSkyColor::PerezFunction_y(float cosTheta, float cosGamma, float gamma)
{
	float val1 = (1 + Ay * exp(By / cosTheta)) * (1 + Cy * exp(Dy * gamma) + Ey * SQR(cosGamma));
	float val2 = (1 + Ay * exp(By)) * (1 + Cy * exp(Dy * suntheta) + Ey * SQR(cosSTheta));

	return val1 / val2;
}

float CSkyColor::PerezFunction_Y(float cosTheta, float cosGamma, float gamma)
{
	float val1 = (1 + AY * exp(BY / cosTheta)) * (1 + CY * exp(DY * gamma) + EY * SQR(cosGamma));
	float val2 = (1 + AY * exp(BY)) * (1 + CY * exp(DY * suntheta) + EY * SQR(cosSTheta));

	return val1 / val2;
}

void CSkyColor::SetInfo(float SunTheta, float Turbidity)
{
	suntheta = SunTheta;
	T = Turbidity;

	T2 = T * T;
	suntheta2 = suntheta * suntheta;
	suntheta3 = suntheta * suntheta2;

	Ax = -0.01925 * T - 0.25922;
	Bx = -0.06651 * T + 0.00081;
	Cx = -0.00041 * T + 0.21247;
	Dx = -0.06409 * T - 0.89887;
	Ex = -0.00325 * T + 0.04517;

	Ay = -0.01669 * T - 0.26078;
	By = -0.09495 * T + 0.00921;
	Cy = -0.00792 * T + 0.21023;
	Dy = -0.04405 * T - 1.65369;
	Ey = -0.01092 * T + 0.05291;

	AY = 0.17872 * T - 1.46303;
	BY = -0.35540 * T + 0.42749;
	CY = -0.02266 * T + 5.32505;
	DY = 0.12064 * T - 2.57705;
	EY = -0.06696 * T + 0.37027;

	xz = (0.00165 * suntheta3 - 0.00375 * suntheta2 + 0.00209 * suntheta + 0.00000) * T2 +
		(-0.02903 * suntheta3 + 0.06377 * suntheta2 - 0.03202 * suntheta + 0.00394) * T +
		(0.11693 * suntheta3 - 0.21196 * suntheta2 + 0.06052 * suntheta + 0.25886);

	yz = (0.00275 * suntheta3 - 0.00610 * suntheta2 + 0.00317 * suntheta + 0.00000) * T2 +
		(-0.04214 * suntheta3 + 0.08970 * suntheta2 - 0.04153 * suntheta + 0.00516) * T +
		(0.15346 * suntheta3 - 0.26756 * suntheta2 + 0.06670 * suntheta + 0.26688);

	float X = (4.0f / 9.0f - T / 120.0f) * (PI - 2.f * suntheta);

	Yz = ((4.0453f * T - 4.9710) * tan(X) - 0.2155 * T + 2.4192) * 1000.0f;
}

void CSkyColor::ToRGBA(float x, float y, float Y, D3DXCOLOR* rgb)
{
	float fX, fY, fZ;

	fY = Y;
	fX = x / y * Y;
	fZ = ((1.0f - x - y) / y) * Y;

	float r, g, b;

	r = 3.240479f * fX - 1.537150f * fY - 0.498535f * fZ;
	g = -0.969256f * fX + 1.875991f * fY + 0.041556f * fZ;
	b = 0.055648f * fX - 0.204043f * fY + 1.057311f * fZ;

	float expo = -(1.0f / 15000.0f);
	r = 1.0f - exp(expo * r);
	g = 1.0f - exp(expo * g);
	b = 1.0f - exp(expo * b);

	if (r < 0.0f) r = 0.0f;
	if (g < 0.0f) g = 0.0f;
	if (b < 0.0f) b = 0.0f;
	if (r > 1.0f) r = 1.0f;
	if (g > 1.0f) g = 1.0f;
	if (b > 1.0f) b = 1.0f;

	rgb->r = r;
	rgb->g = g;
	rgb->b = b;
	rgb->a = 1.0f;
}

float CSkyColor::Angle(float theta1, float phi1, float theta2, float phi2)
{
	float cospsi = sin(theta1) * sin(theta2) * cos(phi2 - phi1) + cos(theta1) * cos(theta2);
	if (cospsi > 1)  return 0;
	if (cospsi < -1) return PI;
	float ac = acos(cospsi);
	return ac;
}

void CSkyColor::GetVertexColor(float th, float phi, float sunphi, D3DXCOLOR* color)
{
	float x, y, Y;
	float cosTheta, cosGamma;

	float gm = Angle(th, phi, suntheta, sunphi);
	float das = fabs(th - (PI / 2.0f));
	if (fabs(th - (PI / 2.0f)) < 0.001f)
		cosTheta = 0.00001f;
	else
		cosTheta = cos(th);

	cosGamma = cos(gm);
	cosSTheta = cos(suntheta);

	x = xz * PerezFunction_x(cosTheta, cosGamma, gm);
	y = yz * PerezFunction_y(cosTheta, cosGamma, gm);
	Y = Yz * PerezFunction_Y(cosTheta, cosGamma, gm);

	ToRGBA(x, y, Y, color);

	// Include alpha transparency to fade out skydome on sunset to see the stars. 
	// Comment these if fading needed is not needed. cosSTheta is the angle of the Sun from the Horizon.
	if (cosSTheta <= 0.1)
		color->a = cosSTheta / 0.1;
	if (cosSTheta < 0.04)      // Dont let it get completely transparent
		color->a = 0.4;
}
