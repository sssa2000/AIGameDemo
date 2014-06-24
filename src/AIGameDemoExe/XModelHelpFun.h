/*!
 * \file XModelHelpFun.h
 * \date 2014/05/31 23:35
 *
 * \author BooSTLoki
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#pragma once
#include <d3dx9.h>
#include <vector>
struct IDirect3DDevice9;
class GameRenderable;
class sMaterial;
class sBoundingBox;
class sBoundingSphere;
void FillRenderableMeshWithXFile(const char* fn, IDirect3DDevice9* device, ID3DXMesh** pMesh, std::vector<sMaterial*>* pMats);
bool ComputeBoundingBox(ID3DXMesh* mesh, sBoundingBox* box);
bool ComputeBoundingSphere(ID3DXMesh* mesh, sBoundingSphere* sphere);
