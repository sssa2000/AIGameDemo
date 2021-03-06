#include "TerrainEntity.h"
#include "Globals.h"
#include "GameRenderable.h"
#include "XModelHelpFun.h"
#include "HippoD3d9Device.h"
#include "XModelHelpFun.h"
#include "GameRenderable.h"
#include "FxEffectManager.h"
#include "TankGameRender.h"
#include "TowerEntity.h"
#include "IGameWordContex.h"
#include "FreeImage.h"
#include "ErrReport.h"
#include "CameraBase.h"
#include <memory>
#include <algorithm>
#include "IGameWordContex.h"
#include "RobotEntity.h"

float w = 2048;
float h = 2048;
int terrain_w = 64; //x方向的顶点数量
int terrain_h = 64;
float gHeightMapScale=22.f;
float vertex_stride = 10.0f;
float orgX_Offset =  terrain_w*vertex_stride*0.5f;
float orgY_Offset =  terrain_w*vertex_stride*0.5f;


TerrainEntity::TerrainEntity()
{
	m_pRenderable = 0;
	InitRenderable();
}
TerrainEntity::~TerrainEntity()
{
	delete m_pRenderable;
}

void TerrainEntity::InitRenderable()
{
	//m_pRenderable = new TerrainRenderable(this);
	m_pRenderable = (TerrainRenderable*)new TerrainRenderablePlane(this);
	m_pRenderable->LoadFromFile();
	SetPostion(0, 0, 0);
}
void TerrainEntity::Render(unsigned int escapeTime)
{
	auto d3ddevice = Globals::GetDevice();
	m_pRenderable->Render(d3ddevice, escapeTime);
}

bool TerrainEntity::RayHit(HippoRay* ray,D3DXVECTOR3* insertPoint)
{
	return m_pRenderable->RayHit(ray,insertPoint);
}

const D3DXPLANE* TerrainEntity::GetBorderPlane(BORDER_DEFINE d)
{
	return m_pRenderable->GetBorderPlane(d);
}
//////////////////////////////////////////////////////////////////////////
int TerrainRenderable::GetVertexNum()
{
	return terrain_w*terrain_h;
}

int TerrainRenderable::GetTriangleNum()
{
	return (terrain_w - 1)*(terrain_h - 1) * 2;
}
int TerrainRenderable::GetIndexNum()
{
	return 3 * GetTriangleNum();
}

TerrainRenderable::TerrainRenderable(GameEntity* parent)
{
	m_parent = parent;
	D3DXMatrixIdentity(&m_local_matrix);
	m_pTextrue[0] = 0;
	m_pTextrue[1] = 0;
}
TerrainRenderable::~TerrainRenderable()
{
	if (m_pHeightField)
		delete m_pHeightField;
	if (m_pTextrue[0])
		m_pTextrue[0]->Release();
	
	if (m_pTextrue[1])
		m_pTextrue[1]->Release();

	auto itr = m_all_patch.begin();
	while (itr != m_all_patch.end())
	{
		auto p = *itr;
		delete p;
		++itr;
	}
	m_all_patch.clear();

	
}


void TerrainRenderable::DrawPatch(HippoD3d9Device* pdevice, unsigned int escapeTime)
{
	

}
void TerrainRenderable::Render(HippoD3d9Device* pdevice, unsigned int escapeTime)
{
	auto d3d9device = pdevice->GetDeviceD3D9();

	//*d3d9device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);*/
	//world matrix
	D3DXMATRIX tmpMatrix;
	D3DXMatrixMultiply(&tmpMatrix, &m_local_matrix, m_parent->GetWorldTransform());
	HRESULT v = m_fxhandle->SetMatrix("g_mWorld", &tmpMatrix);

	//wvp matrix
	auto proj = Globals::GetRender()->GetProjMatrix();
	auto view = Globals::GetRender()->GetViewMatrix();
	D3DXMATRIX vp = (*view)*(*proj);
	v = m_fxhandle->SetMatrix("g_mViewProjection", &vp);

	//camera
	const D3DXVECTOR3* pos=Globals::GetCurrentCamera()->GetPos();
	v = m_fxhandle->SetValue("g_camera_pos", pos, sizeof(D3DXVECTOR3));


	//texture
	v = m_fxhandle->SetTexture("g_DiffuseTex", m_pTextrue[0]);
	v = m_fxhandle->SetTexture("g_DetailTex", m_pTextrue[1]);
	v = m_fxhandle->SetTexture("g_NormalTex", m_pTextrue[2]);

	//tower info
	const GameEntityPtrCon& con = Globals::GetWorld()->GetAllTower();
	GameEntityPtrCon::const_iterator itr = con.begin();
	GameEntityPtrCon::const_iterator itrend = con.end();
	D3DXVECTOR4 tmp[2];
	int idx = 0;
	while (itr != itrend)
	{
		auto towerptr = (TowerEntity*)itr->get();
		D3DXVECTOR3* pos = towerptr->GetPos();
		float r = towerptr->GetRange();
		tmp[idx] = D3DXVECTOR4(pos->x, pos->y, pos->z, r);
		++itr;
		++idx;
	}
	v=m_fxhandle->SetVectorArray("towerInfo", tmp, 2);

	//robot info
	auto robot=Globals::GetWorld()->GetPlayer();
	D3DXVECTOR4 robotpos = D3DXVECTOR4(robot->GetPos()->x, robot->GetPos()->z, 2, 1);
	v=m_fxhandle->SetVector("robotInfo", &robotpos);

	d3d9device->SetStreamSource(0, m_pVB, 0, sizeof(TerrainVertex));
	d3d9device->SetVertexDeclaration(m_pVertexDecl);
	d3d9device->SetIndices(m_pIB);	UINT iPass, totalPasses;
	m_fxhandle->Begin(&totalPasses, 0);

	for (iPass = 0; iPass < totalPasses; iPass++)
	{
		m_fxhandle->BeginPass(iPass);
		//d3d9device->DrawPrimitive(D3DPT_POINTLIST,
		//	0,   // 将要绘制的索引缓冲区的起始地址
		//	GetVertexNum()); // 绘制的图元数量

		d3d9device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
			0,   // 将要绘制的索引缓冲区的起始地址
			0, //　索引数组中最小的索引值
			GetVertexNum(),   // 要绘制的索引数组中的顶点数量
			0, // 从索引数组中的第几个元素开始绘制图元
			GetTriangleNum()); // 绘制的图元数量
		
		m_fxhandle->EndPass();
	}

	m_fxhandle->End();
	return;
	//patch
	auto pthitr = m_all_patch.begin();
	while (pthitr != m_all_patch.end())
	{
		auto patch_fx =  (*pthitr)->GetPatchFx();
		auto patch_ib = (*pthitr)->GetIB();
		int tri_count = (*pthitr)->GetIndexCount() / 3;
		d3d9device->SetStreamSource(0, m_pVB, 0, sizeof(TerrainVertex));
		d3d9device->SetVertexDeclaration(m_pVertexDecl);

		HRESULT v = d3d9device->SetIndices(patch_ib);
		
		v=patch_fx->SetMatrix("g_mWorld", &tmpMatrix);
		v = patch_fx->SetMatrix("g_mViewProjection", &vp);
		v = patch_fx->SetValue("g_camera_pos", pos, sizeof(D3DXVECTOR3));
		
		
		UINT totalPasses;
		v=patch_fx->Begin(&totalPasses, 0);
		v = patch_fx->BeginPass(0);
		v = d3d9device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
			0,   // 将要绘制的索引缓冲区的起始地址
			0, //　索引数组中最小的索引值
			GetVertexNum(),   // 要绘制的索引数组中的顶点数量
			0, // 从索引数组中的第几个元素开始绘制图元
			tri_count); // 绘制的图元数量
		v = patch_fx->EndPass();

		++pthitr;
	}
}
void TerrainRenderable::ConvertPos(float& x,float& z)
{


	x=x*vertex_stride- orgX_Offset;
	z=z*vertex_stride- orgY_Offset;

}

float TerrainRenderable::GetTerrainHeight(float x, float z)
{
	//ConvertPos(x,y);
	float tx=(x+orgX_Offset)/vertex_stride;
	float tz=(z+orgY_Offset)/vertex_stride;
	if (m_pHeightField)
		return (float)m_pHeightField->GetHeight(tx, tz);
	return 0;
}
void TerrainRenderable::MakeVertexBuffer()
{
	auto device = Globals::GetDevice()->GetDeviceD3D9();
	int n=GetVertexNum();
	HRESULT res = device->CreateVertexBuffer(n * sizeof(TerrainVertex), D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED, &m_pVB, 0);
	if (res != S_OK)
		ReportErr("Terrain CreateVB Failed");
	TerrainVertex* v = 0;
	int idx=0;
	m_pVB->Lock(0, 0, (void**)&v, 0);

	for (int y = 0; y < terrain_h; ++y)
	{
		for (int x = 0; x < terrain_w; ++x)
		{
			float posx=x;
			float posz=y;
			ConvertPos(posx,posz);
			m_VertexData.push_back(TerrainVertex(posx, m_pHeightField->GetHeight(x,y), posz, (float)x / terrain_w, (float)y / terrain_h));
			//v[idx++]=TerrainVertex(x*vertex_stride - orgX_Offset, GetTerrainHeight(x,y), y*vertex_stride - orgY_Offset, (float)x / terrain_w, (float)y / terrain_h);
		}
	}
	TerrainVertex* first=&(m_VertexData[0]);
	memcpy_s(v,n*sizeof(TerrainVertex),first,n*sizeof(TerrainVertex));
	m_pVB->Unlock();
}
void TerrainRenderable::MakeIndexBuffer()
{
	auto device = Globals::GetDevice()->GetDeviceD3D9();

	HRESULT res = device->CreateIndexBuffer(GetIndexNum() * sizeof(WORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB, 0);
	if (res != S_OK)
		ReportErr("Terrain CreateIB Failed");

	WORD* g_Indices = 0;
	res = m_pIB->Lock(0, 0, (void**)&g_Indices, 0);
	for (int y = 0; y < terrain_h-1; ++y)
	{
		for (int x = 0; x < terrain_w-1; ++x)
		{
			m_IndexData.push_back((x + 0) + (y + 0)*terrain_w);
			m_IndexData.push_back((x + 0) + (y + 1)*terrain_w);
			m_IndexData.push_back((x + 1) + (y + 1)*terrain_w);

			m_IndexData.push_back((x + 0) + (y + 0)*terrain_w);
			m_IndexData.push_back((x + 1) + (y + 1)*terrain_w);
			m_IndexData.push_back((x + 1) + (y + 0)*terrain_w);
		}
	}
	WORD* first=&(m_IndexData[0]);
	memcpy_s(g_Indices,GetIndexNum() * sizeof(WORD),first,GetIndexNum() * sizeof(WORD));
	res = m_pIB->Unlock();
}
void TerrainRenderable::MakeVertexDeclaration()
{
	auto device = Globals::GetDevice()->GetDeviceD3D9();
	D3DVERTEXELEMENT9 vbDecl[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		{ 0, 20, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },

		{ 0xFF, 0, D3DDECLTYPE_UNUSED, 0, 0, 0 }
	};
	HRESULT res = device->CreateVertexDeclaration(vbDecl, &m_pVertexDecl);
}
void TerrainRenderable::LoadTexture()
{
	auto device = Globals::GetDevice()->GetDeviceD3D9();
	HRESULT v=S_OK;
	v += D3DXCreateTextureFromFileA(device, "../media/terrain/texture_0.jpg", &m_pTextrue[0]);
	v += D3DXCreateTextureFromFileA(device, "../media/terrain/texture_1.jpg", &m_pTextrue[1]);
	v += D3DXCreateTextureFromFileA(device, "../media/terrain/normal.tga", &m_pTextrue[2]);

}
void TerrainRenderable::LoadFromFile()
{
	m_pHeightField = new TerrainHeightField;
	m_pHeightField->LoadFromHeightMap("../media/terrain/height.png", terrain_w, terrain_h);
	LoadTexture();
	MakeVertexBuffer();
	MakeIndexBuffer();
	MakeVertexDeclaration();
	LoadFx();
}

void TerrainRenderable::LoadFx()
{
	m_fxhandle = Globals::GetFxManager()->RequireEffectFormFile("../media/fx/terrain2.fx");
	m_fxhandle->SetTechnique("RenderSceneWithTexture1Light");

}
TerrainPatch* TerrainRenderable::CreatePatch(float x,float y,float z,float range)
{
	TerrainPatch* p=new TerrainPatch(this,x,y,z,range);
	m_all_patch.push_back(p);
	return p;
}

void TerrainRenderable::ReleasePatch(TerrainPatch* p)
{
	auto itr=std::find(m_all_patch.begin(),m_all_patch.end(),p);
	if(itr!=m_all_patch.end())
		m_all_patch.erase(itr);
	delete p;
}


//////////////////////////////////////////////////////////////////////////

TerrainHeightField::TerrainHeightField()
{

}
TerrainHeightField::~TerrainHeightField()
{
	if (m_pData)
		delete[] m_pData;
}

void TerrainHeightField::LoadFromHeightMap(const char* filename, int dstW, int dstH)
{
	m_width = dstW;
	m_height = dstH;
	FREE_IMAGE_FORMAT ft=FreeImage_GetFileType(filename);
	FIBITMAP* pdata=FreeImage_Load(ft, filename);
	FIBITMAP* presdata = pdata;
	unsigned int w=FreeImage_GetWidth(pdata);
	unsigned int h = FreeImage_GetHeight(pdata);
	if (w != dstW || h != dstH)
	{
		presdata = FreeImage_Rescale(pdata, dstW, dstH);
	}

	unsigned int nPixSize = FreeImage_GetBPP(presdata) / 8;
	unsigned int lPitch = dstW*nPixSize;
	unsigned int bufferSize = lPitch*dstH;
	m_pData = new unsigned char[bufferSize];
	FreeImage_ConvertToRawBits(m_pData, presdata, lPitch, FreeImage_GetBPP(presdata), FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, TRUE);

	FreeImage_Unload(pdata);
	FreeImage_Unload(presdata);

}
float TerrainHeightField::GetHeight(int x, int y)
{
	return m_pData[y*m_width + x]/gHeightMapScale;
	return 0;
}


//////////////////////////////////////////////////////////////////////////


TerrainPatch::TerrainPatch(TerrainRenderable* parent,float x,float y,float z,float range)
{
	m_Pos=D3DXVECTOR3(x,y,z);
	m_r=range;
	m_parent=parent;
	m_pIB = 0;
	LoadFx();

}
TerrainPatch::~TerrainPatch()
{
	if (m_pIB)
		m_pIB->Release();
	m_pIB = 0;

}

void TerrainPatch::ChangePos(float x,float y,float z)
{
	//只有第一次更新 或者位置变化了才更新
	if (!m_pIB || m_Pos.x != x || m_Pos.y != y || m_Pos.z != z)
	{
		m_Pos = D3DXVECTOR3(x, y, z);
		CalcIndexBuffer();
	}

}

bool IsTriangleInCircle2D(const D3DXVECTOR3* pos,float r,TerrainVertex* v0, TerrainVertex* v1, TerrainVertex* v2)
{
	D3DXVECTOR2 center = D3DXVECTOR2(pos->x, pos->z);
	D3DXVECTOR2 v00 = center - D3DXVECTOR2(v0->m_pos.x, v0->m_pos.z);
	D3DXVECTOR2 v01 = center - D3DXVECTOR2(v1->m_pos.x, v1->m_pos.z);
	D3DXVECTOR2 v02 = center - D3DXVECTOR2(v2->m_pos.x, v2->m_pos.z);
	//未来可能放宽一点
	return 
		(D3DXVec2LengthSq(&v00) <= r*r) ||
		(D3DXVec2LengthSq(&v01) <= r*r) ||
		(D3DXVec2LengthSq(&v02) <= r*r);

}

void TerrainPatch::CalcIndexBuffer()
{
	std::vector<WORD> tmp;
	//遍历地形中的索引，找到在圆圈内的三角形
	auto itr = m_parent->m_IndexData.begin();
	while (itr != m_parent->m_IndexData.end())
	{
		WORD idx0 = *itr;
		TerrainVertex* v0 = &m_parent->m_VertexData.at(idx0);
		++itr;

		WORD idx1 = *itr;
		TerrainVertex* v1 = &m_parent->m_VertexData.at(idx1);
		++itr;

		WORD idx2 = *itr;
		TerrainVertex* v2 = &m_parent->m_VertexData.at(idx2);
		++itr;

		if (IsTriangleInCircle2D(&m_Pos, m_r, v0, v1, v2))
		{
			tmp.push_back(idx0);
			tmp.push_back(idx1);
			tmp.push_back(idx2);
		}
	}

	if (tmp.size() > m_IndexData.size())
	{
		//删除硬件buffer
		if (m_pIB)
			m_pIB->Release();
		m_pIB = 0;
	}
	m_IndexData.swap(tmp);
	tmp.clear();

	//创建硬件buffer
	unsigned int indexsize = m_IndexData.size() * sizeof(WORD);
	if (!m_pIB)
	{
		auto device=Globals::GetDevice()->GetDeviceD3D9();
		HRESULT res = device->CreateIndexBuffer(indexsize, D3DUSAGE_DYNAMIC, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &m_pIB, 0);
		if (res != S_OK)
			ReportErr("Terrain Patch CreateIB Failed");
	}
	//fill buffer
	WORD* g_Indices = 0;
	HRESULT res = m_pIB->Lock(0, 0, (void**)&g_Indices, D3DLOCK_DISCARD);
	WORD* first = &(m_IndexData[0]);
	memcpy_s(g_Indices, indexsize, first, indexsize);
	res = m_pIB->Unlock();
}

void TerrainPatch::LoadFx()
{
	m_fxhandle = Globals::GetFxManager()->RequireEffectFormFile("../media/fx/terrain_patch.fx");
	m_fxhandle->SetTechnique("RenderSceneWithTexture1Light");

}


TerrainRenderablePlane::TerrainRenderablePlane(GameEntity* parent):
TerrainRenderable(parent)
{
	D3DXVECTOR3 org000(0,0,0);
	D3DXVECTOR3 nrm(0,1,0);
	D3DXPlaneFromPointNormal(&m_phy_shape,&org000,&nrm);
}
TerrainRenderablePlane::~TerrainRenderablePlane()
{

}
void TerrainRenderablePlane::Render(HippoD3d9Device* pdevice, unsigned int escapeTime)
{
	auto d3d9device = pdevice->GetDeviceD3D9();

	//d3d9device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	//world matrix
	D3DXMATRIX tmpMatrix;
	D3DXMatrixMultiply(&tmpMatrix, &m_local_matrix, m_parent->GetWorldTransform());
	HRESULT v = m_fxhandle->SetMatrix("g_mWorld", &tmpMatrix);

	//wvp matrix
	auto proj = Globals::GetRender()->GetProjMatrix();
	auto view = Globals::GetRender()->GetViewMatrix();
	D3DXMATRIX vp = (*view)*(*proj);
	v = m_fxhandle->SetMatrix("g_mViewProjection", &vp);

	//camera
	const D3DXVECTOR3* pos = Globals::GetCurrentCamera()->GetPos();
	v = m_fxhandle->SetValue("g_camera_pos", pos, sizeof(D3DXVECTOR3));

	//material
	D3DXCOLOR vWhite = D3DXCOLOR(0.5, 0.3, 0.2, 1);
	v = m_fxhandle->SetValue("g_MaterialDiffuseColor", &vWhite, sizeof(D3DXCOLOR));

	//texture
	v = m_fxhandle->SetTexture("g_MeshTexture", m_pTextrue[0]);

	//tower info
	auto alltowers = Globals::GetWorld()->GetAllTower();
	auto itr = alltowers.begin();
	D3DXVECTOR4 tmp[2];
	int idx = 0;
	while (itr != alltowers.end())
	{
		TowerEntity* pEntity = (TowerEntity*)((*itr).get());
		D3DXVECTOR3* pos = pEntity->GetPos();
		float r = pEntity->GetRange();
		tmp[idx] = D3DXVECTOR4(pos->x, pos->y, pos->z, r);
		++itr;
		++idx;
	}
	v = m_fxhandle->SetVectorArray("towerInfo", tmp, 2);

	//robot info
	auto player = Globals::GetWorld()->GetPlayer();
	pos = player->GetPos();
	D3DXVECTOR4 robotpos = D3DXVECTOR4(pos->x, pos->z, 12.f, 1.0f);
	v = m_fxhandle->SetVector("robotInfo", &robotpos);


	d3d9device->SetStreamSource(0, m_pVB, 0, sizeof(TerrainVertex));
	d3d9device->SetVertexDeclaration(m_pVertexDecl);
	d3d9device->SetIndices(m_pIB);	UINT iPass, totalPasses;
	m_fxhandle->Begin(&totalPasses, 0);

	for (iPass = 0; iPass < totalPasses; iPass++)
	{
		m_fxhandle->BeginPass(iPass);

		d3d9device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
			0,   // 将要绘制的索引缓冲区的起始地址
			0, //　索引数组中最小的索引值
			GetVertexNum(),   // 要绘制的索引数组中的顶点数量
			0, // 从索引数组中的第几个元素开始绘制图元
			GetTriangleNum()); // 绘制的图元数量

		m_fxhandle->EndPass();
	}

	m_fxhandle->End();

	m_border_renderable->Render(pdevice, escapeTime);
}

bool TerrainRenderablePlane::RayHit(HippoRay* ray,D3DXVECTOR3* insertPoint)
{
	auto end=ray->GetEndPoint(100000.f);
	auto* insertpoint=D3DXPlaneIntersectLine(insertPoint,&m_phy_shape,&ray->m_OrgPos,&end);

	return insertpoint!=0;
}
void TerrainRenderablePlane::MakeVertexBuffer()
{
	auto device = Globals::GetDevice()->GetDeviceD3D9();
	int n = GetVertexNum();
	HRESULT res = device->CreateVertexBuffer(n * sizeof(TerrainVertex), D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED, &m_pVB, 0);
	if (res != S_OK)
		ReportErr("Terrain CreateVB Failed");
	TerrainVertex* v = 0;
	int idx = 0;
	m_pVB->Lock(0, 0, (void**)&v, 0);

	m_VertexData.push_back(TerrainVertex(0 - w / 2, 0 , 0 - h / 2, 0, 0));
	m_VertexData.push_back(TerrainVertex(0 - w / 2, 0 , h - h / 2, 0, 1));
	m_VertexData.push_back(TerrainVertex(w - w / 2, 0 , h - h / 2, 1, 1));
	m_VertexData.push_back(TerrainVertex(w - w / 2, 0 , 0 - h / 2, 1, 0));

	TerrainVertex* first = &(m_VertexData[0]);
	memcpy_s(v, n*sizeof(TerrainVertex), first, n*sizeof(TerrainVertex));
	m_pVB->Unlock();
}
void TerrainRenderablePlane::MakeIndexBuffer()
{
	auto device = Globals::GetDevice()->GetDeviceD3D9();

	HRESULT res = device->CreateIndexBuffer(GetIndexNum() * sizeof(WORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB, 0);
	if (res != S_OK)
		ReportErr("Terrain CreateIB Failed");

	WORD* g_Indices = 0;
	res = m_pIB->Lock(0, 0, (void**)&g_Indices, 0);

	m_IndexData.push_back(0);
	m_IndexData.push_back(1);
	m_IndexData.push_back(2);

	m_IndexData.push_back(0);
	m_IndexData.push_back(2);
	m_IndexData.push_back(3);

	WORD* first = &(m_IndexData[0]);
	memcpy_s(g_Indices, GetIndexNum() * sizeof(WORD), first, GetIndexNum() * sizeof(WORD));
	res = m_pIB->Unlock();
}
void TerrainRenderablePlane::LoadTexture()
{
	auto device = Globals::GetDevice()->GetDeviceD3D9();
	HRESULT v = D3DXCreateTextureFromFileA(device, "../media/grid.dds", &m_pTextrue[0]);

	
}
void TerrainRenderablePlane::LoadFx()
{
	m_fxhandle = Globals::GetFxManager()->RequireEffectFormFile("../media/fx/terrain_plane.fx");
	m_fxhandle->SetTechnique("RenderSceneWithTexture1Light");

}


void TerrainRenderablePlane::LoadFromFile()
{
	TerrainRenderable::LoadFromFile();
	m_border_renderable.reset(new TerrainBorderRenderable());
	m_border_renderable->Init(-w*0.5f, h*0.5f, w*0.5f, -h*0.5f);
}
int TerrainRenderablePlane::GetVertexNum()
{
	return 4;
}

int TerrainRenderablePlane::GetTriangleNum()
{
	return 2;
}
int TerrainRenderablePlane::GetIndexNum()
{
	return 3 * GetTriangleNum();
}

TerrainBorderRenderable::TerrainBorderRenderable()
{

}
TerrainBorderRenderable::~TerrainBorderRenderable()
{

}

//参数是把地形看作一个2维矩形，该矩形的四个顶点
void TerrainBorderRenderable::Init(float left, float top, float right, float bottom)
{
	LoadTexture();
	MakeVertexBuffer(left, top, right, bottom);
	MakeIndexBuffer();
	MakeVertexDeclaration();
	LoadFx();
}
void TerrainBorderRenderable::Render(HippoD3d9Device* pdevice, unsigned int escapeTime)
{
	auto device = Globals::GetDevice()->GetDeviceD3D9();

	RS_HELP_OBJ(device, D3DRS_ALPHABLENDENABLE, TRUE);
	RS_HELP_OBJ(device, D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	RS_HELP_OBJ(device, D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	//RS_HELP_OBJ(device, D3DRS_ZWRITEENABLE, false);
	RS_HELP_OBJ(device, D3DRS_CULLMODE, D3DCULL_CCW);

	D3DXMATRIX matrix;
	D3DXMatrixIdentity(&matrix);
	HRESULT v = m_fxhandle->SetMatrix("g_mWorld", &matrix);

	//wvp matrix
	auto proj = Globals::GetRender()->GetProjMatrix();
	auto view = Globals::GetRender()->GetViewMatrix();
	D3DXMATRIX vp = (*view)*(*proj);
	v = m_fxhandle->SetMatrix("g_mViewProjection", &vp);

	//time
	float time = GetTickCount()*0.001f;
	v = m_fxhandle->SetValue("g_fTime", &time, sizeof(float));

	//viewmatrix
	const D3DXVECTOR3* camPos = Globals::GetCurrentCamera()->GetPos();
	v = m_fxhandle->SetMatrix("ViewMatrix", view);
	v = m_fxhandle->SetValue("g_camerapos", camPos, sizeof(D3DXVECTOR3));

	//texture
	v = m_fxhandle->SetTexture("g_MeshTexture", m_pTextrue.get());

	//threshold
	D3DXVECTOR4 threshold = D3DXVECTOR4(800.f, 0.99f, 0, 0);
	v = m_fxhandle->SetValue("threshold", &threshold, sizeof(D3DXVECTOR4));

	device->SetStreamSource(0, m_pVB.get(), 0, sizeof(TerrainVertex));
	device->SetVertexDeclaration(m_pVertexDecl.get());
	device->SetIndices(m_pIB.get());

	UINT iPass, totalPasses;
	m_fxhandle->Begin(&totalPasses, 0);
	m_fxhandle->BeginPass(0);

	device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
		0,   // 将要绘制的索引缓冲区的起始地址
		0, //　索引数组中最小的索引值
		16,   // 要绘制的索引数组中的顶点数量
		0, // 从索引数组中的第几个元素开始绘制图元
		8); // 绘制的图元数量

	m_fxhandle->EndPass();
	m_fxhandle->End();
}

void TerrainBorderRenderable::MakeVertexBuffer(float left, float top, float right, float bottom)
{
	auto device = Globals::GetDevice()->GetDeviceD3D9();
	IDirect3DVertexBuffer9* pVb = 0;
	HRESULT res = device->CreateVertexBuffer(16 * sizeof(TerrainVertex), D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED, &pVb, 0);
	if (res != S_OK)
		ReportErr("SkyBox CreateVB Failed");
	m_pVB.reset(pVb, [&](IDirect3DVertexBuffer9* p){p->Release(); });

	TerrainVertex* v = 0;
	float scale = 500.0f;
	m_pVB->Lock(0, 0, (void**)&v, 0);
	float height = (top - bottom)*0.5f;
	float width = (right - left)*0.5f;

	// positive x
	v[0] = TerrainVertex(1.0f*right, -1.0f*height, 1.0f*width, 0.0f, 1.0f);
	v[1] = TerrainVertex(1.0f*right, 1.0f*height, 1.0f*width, 0.0f, 0.0f);
	v[2] = TerrainVertex(1.0f*right, 1.0f*height, -1.0f*width, 1.0f, 0.0f);
	v[3] = TerrainVertex(1.0f*right, -1.0f*height, -1.0f*width, 1.0f, 1.0f);
	D3DXPlaneFromPoints(&m_border_planes[POSITIVE_X], &v[0].m_pos, &v[1].m_pos, &v[2].m_pos);

	// negative x
	v[4] = TerrainVertex(1.0f*left, -1.0f*height, -1.0f*width, 0.0f, 1.0f);
	v[5] = TerrainVertex(1.0f*left, 1.0f*height, -1.0f*width, 0.0f, 0.0f);
	v[6] = TerrainVertex(1.0f*left, 1.0f*height, 1.0f*width, 1.0f, 0.0f);
	v[7] = TerrainVertex(1.0f*left, -1.0f*height, 1.0f*width, 1.0f, 1.0f);
	D3DXPlaneFromPoints(&m_border_planes[NEGATIVE_X], &v[4].m_pos, &v[5].m_pos, &v[6].m_pos);

	// positive z
	v[8] = TerrainVertex(-1.0f*width, -1.0f*height, 1.0f*top, 0.0f, 1.0f);
	v[9] = TerrainVertex(-1.0f*width, 1.0f*height, 1.0f*top, 0.0f, 0.0f);
	v[10] = TerrainVertex(1.0f*width, 1.0f*height, 1.0f*top, 1.0f, 0.0f);
	v[11] = TerrainVertex(1.0f*width, -1.0f*height, 1.0f*top, 1.0f, 1.0f);
	D3DXPlaneFromPoints(&m_border_planes[POSITIVE_Y], &v[8].m_pos, &v[9].m_pos, &v[10].m_pos);

	// negative z
	v[12] = TerrainVertex(1.0f*width, -1.0f*height, 1.0f*bottom, 0.0f, 1.0f);
	v[13] = TerrainVertex(1.0f*width, 1.0f*height, 1.0f*bottom, 0.0f, 0.0f);
	v[14] = TerrainVertex(-1.0f*width, 1.0f*height, 1.0f*bottom, 1.0f, 0.0f);
	v[15] = TerrainVertex(-1.0f*width, -1.0f*height, 1.0f*bottom, 1.0f, 1.0f);
	D3DXPlaneFromPoints(&m_border_planes[NEGATIVE_Y], &v[12].m_pos, &v[13].m_pos, &v[14].m_pos);

	m_pVB->Unlock();
}
void TerrainBorderRenderable::MakeIndexBuffer()
{
	auto device = Globals::GetDevice()->GetDeviceD3D9();
	IDirect3DIndexBuffer9* pIb = 0;
	HRESULT res = device->CreateIndexBuffer(24 * sizeof(WORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &pIb, 0);
	if (res != S_OK)
		ReportErr("TerrainBorder CreateIB Failed");
	m_pIB.reset(pIb, [&](IDirect3DIndexBuffer9* p){p->Release(); });
	WORD* g_Indices = 0;
	m_pIB->Lock(0, 0, (void**)&g_Indices, 0);
	// positive x
	g_Indices[0] = 0; g_Indices[1] = 1; g_Indices[2] = 2;
	g_Indices[3] = 0; g_Indices[4] = 2; g_Indices[5] = 3;
	// negative x
	g_Indices[6] = 4; g_Indices[7] = 5; g_Indices[8] = 6;
	g_Indices[9] = 4; g_Indices[10] = 6; g_Indices[11] = 7;
	// positive z
	g_Indices[12] = 8; g_Indices[13] = 9; g_Indices[14] = 10;
	g_Indices[15] = 8; g_Indices[16] = 10; g_Indices[17] = 11;
	// negative z
	g_Indices[18] = 12; g_Indices[19] = 13; g_Indices[20] = 14;
	g_Indices[21] = 12; g_Indices[22] = 14; g_Indices[23] = 15;

	m_pIB->Unlock();
}
void TerrainBorderRenderable::MakeVertexDeclaration()
{
	auto device = Globals::GetDevice()->GetDeviceD3D9();
	D3DVERTEXELEMENT9 vbDecl[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },

		{ 0xFF, 0, D3DDECLTYPE_UNUSED, 0, 0, 0 }
	};
	IDirect3DVertexDeclaration9* pVd = 0;
	HRESULT res = device->CreateVertexDeclaration(vbDecl, &pVd);
	m_pVertexDecl.reset(pVd, [&](IDirect3DVertexDeclaration9* p){p->Release(); });

}
void TerrainBorderRenderable::LoadTexture()
{
	auto device = Globals::GetDevice()->GetDeviceD3D9();
	IDirect3DTexture9* ptex = 0;
	HRESULT v = D3DXCreateTextureFromFileA(device, "../media/mesh2.tga", &ptex);
	m_pTextrue.reset(ptex, [&](IDirect3DTexture9* p){p->Release(); });

}

void TerrainBorderRenderable::LoadFx()
{
	m_fxhandle = Globals::GetFxManager()->RequireEffectFormFile("../media/fx/terrain_border.fx");
	m_fxhandle->SetTechnique("RenderSceneWithTexture1Light");

}

