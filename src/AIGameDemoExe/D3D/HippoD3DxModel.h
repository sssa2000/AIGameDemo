/*!
 * \file HippoD3DxModel.h
 * \date 2014/03/11 23:09
 *
 * \author BooSTLoki
 * Contact: user@company.com
 *
 * \brief d3dx只提供了mesh的功能，需要对mesh进行简单的模型封装
 *
 * TODO: long description
 *
 * \note
*/

#pragma once
#include <d3dx9mesh.h>
#include <d3dx9math.h>
#include <vector>



class HippoD3DxModel;

//负责创建、组装Model
class HippoD3DxModelFactory
{
public:
	static HippoD3DxModel* CreateTeapotModel(IDirect3DDevice9* device);
	static HippoD3DxModel* CreateFromXFile(const char* path,IDirect3DDevice9* device);
};

class XMaterial
{
public:
	XMaterial(D3DXMATERIAL& pmat);
	~XMaterial();
	D3DMATERIAL9* m_d3d_mat;
	IDirect3DTexture9* m_d3d_texture;

};
class HippoD3DxModel
{
	friend class HippoD3DxModelFactory;

public:
	void Release();
	void SetPos(float x, float y, float z);
	virtual void Update(unsigned int timeMs);
	virtual void Render(IDirect3DDevice9* device);

protected:
	HippoD3DxModel();
	virtual ~HippoD3DxModel();
	D3DXMATRIX m_matWorld;
	ID3DXMesh* m_pD3dxMesh;
	std::vector<XMaterial*> m_mats;
	

private:
};

