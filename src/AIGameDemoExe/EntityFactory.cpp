#include "RobotEntity.h"
#include "EntityFactory.h"
#include "HippoD3d9Device.h"
#include "GameRenderable.h"
#include "GameEntity.h"
#include <d3dx9.h>
#include "HippoFilePath.h"
#include "RobotEntity.h"
#include "RobotRenderable.h"
#include "TowerEntity.h"
#include "TowerRenderable.h"
#include "XModelHelpFun.h"
#include "TerrainEntity.h"

EntityFactory::EntityFactory(HippoD3d9Device* pDevice)
{
	m_pDevice=pDevice;
}

GameEntity* EntityFactory::CreateTank()
{
	RobotEntity* probot=new RobotEntity;
	return probot;
}

TowerEntity* EntityFactory::CreateTower(float range)
{

	TowerEntity* p1 = new TowerEntity(range);

	//
	//D3DXMATRIX rotmatrix;
	//HRESULT v=D3DXCreateCylinder(m_pDevice->GetDeviceD3D9(), 3, 3, 10, 20, 20, &m1->m_pD3dxMesh,0);
	//m1->CreateDefaultMat(); 
	//D3DXMatrixRotationX(&rotmatrix, D3DXToRadian(90));
	//p1->ApplyWorldTransform(rotmatrix); 

	////用于绘制范围的圆柱体，半径=1，高度=0.1，绘制的时候通过缩放扩大
	//v = D3DXCreateCylinder(m_pDevice->GetDeviceD3D9(), 1, 1, 0.1, 20, 20, &m1->m_pRangeMesh, 0);
	return p1;


	//auto p=CreateFromXFile("../media/arrow.x");
	//D3DXMATRIX rotmatrix;
	//D3DXMatrixRotationX(&rotmatrix, D3DXToRadian(90));
	//p->ApplyWorldTransform(rotmatrix);
	//return p;
}

TerrainEntity* EntityFactory::CreateFloor()
{
	TerrainEntity* p = new TerrainEntity();
	p->InitRenderable();
	return p;
}


GameEntity* EntityFactory::CreateFromXFile(const char* fn)
{

	return 0;
}