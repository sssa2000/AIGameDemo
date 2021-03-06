#include "TankGameWorldImpl.h"
#include "GameEntity.h"
#include "Globals.h"
#include "EntityFactory.h"
#include "RobotEntity.h"
#include <functional>
#include <algorithm>
#include "TowerEntity.h"
#include "HippoD3DxModel.h"
#include "HippoD3d9Device.h"
#include "SkyEntity.h"
#include "CameraBase.h"
#include "FollowCamera.h"
#include "TerrainEntity.h"
#include "tixml/tinyxml.h"
#include "StartPoint.h"
#include "TargetPoint.h"
#include "ErrReport.h"
#include "D3dxMathExtension.h"
void CreateWorldTmp()
{
	
}

TankGameWorldImpl::TankGameWorldImpl()
{


}


TankGameWorldImpl::~TankGameWorldImpl()
{

}
void TankGameWorldImpl::SetObjIntoTerrain(float x,float z,GameEntity* obj)
{
	float y=m_pWorld->GetHeight(x,z);
	obj->SetPostion(x, y, z);

}

void TankGameWorldImpl::Update(unsigned int escapeTime)
{
	GetPlayer()->Update(escapeTime);

	std::for_each(
		m_towers.begin(),
		m_towers.end(),
		[&](GameEntityPtr& m_towers){m_towers->Update(escapeTime); }
	);


	std::for_each(
		m_scene_objs.begin(),
		m_scene_objs.end(),
		[&](GameEntityPtr& obj){obj->Update(escapeTime); }
	);
	m_pSky->Update(escapeTime);
	m_pWorld->Update(escapeTime);
	m_start->Update(escapeTime);
	m_end->Update(escapeTime);

}


IRobotProxy* TankGameWorldImpl::GetPlayerRobot()
{
	if (!m_pPlayer)
		return 0;

	RobotEntity* e = (RobotEntity*)m_pPlayer.get();
	return e->GetProxy();
}

void TankGameWorldImpl::NotifyGameOver()
{
	::MessageBoxA(0, "你被炮塔发现，游戏结束!", 0, 0);
}

//得到出生点的位置
D3DXVECTOR2 TankGameWorldImpl::GetBornPoint()
{
	return D3DXVECTOR2(m_start->GetPos()->x, m_start->GetPos()->z);
}

//得到目标点(即终点)的位置
D3DXVECTOR2 TankGameWorldImpl::GetTargetPoint()
{
	return D3DXVECTOR2(m_end->GetPos()->x, m_end->GetPos()->z);
}

//得到tower的数量
unsigned int TankGameWorldImpl::GetTowerNumber()
{
	return m_towers.size();
}

//通过idx检索得到tower的实例，传入的idx不能大于等于GetTowerNumber返回的值
ITowerProxy* TankGameWorldImpl::GetTowerByIdx(unsigned int idx)
{
	if (idx < 0 || idx >= m_towers.size())
		return 0;

	TowerEntity* p = (TowerEntity*)(m_towers.at(idx).get());
	return p->GetProxy();
}

void TankGameWorldImpl::LoadFromFile(const char* filename)
{
	TiXmlDocument doc;
	bool b = doc.LoadFile(filename);
	if (!b)
	{
		ReportErr("Can not read file: %s", filename);
		return;
	}
	ResetScene();

	m_pWorld.reset(Globals::GetEntityFactory()->CreateFloor());
	m_pSky.reset(new SkyDoomEntity);
	m_pPlayer.reset(Globals::GetEntityFactory()->CreateTank());

	auto rootelem = doc.RootElement();
	auto elem = rootelem->FirstChildElement();
	while (elem)
	{
		std::string name=elem->Value();
		if (name == "Tower")
		{
			ParseTower(elem);
		}
		else if (name=="StartPoint")
		{
			ParseStartPoint(elem);
			float height=m_pPlayer->GetPos()->y;
			m_pPlayer->SetPostion(m_start->GetPos()->x, height, m_start->GetPos()->z);
		}
		else if (name == "EndPoint")
		{
			ParseEndPoint(elem);
		}
		else if (name=="SceneObj")
		{
			ParseSceneObj(elem);
		}
		else
		{
			ReportErr("UnKnown obj Type");
		}				
		elem=elem->NextSiblingElement();
	}


}

void TankGameWorldImpl::ResetScene()
{
	m_pWorld.reset();
	m_pSky.reset();

	m_pPlayer.reset();

	{
		GameEntityPtrCon tmp2;
		m_towers.swap(tmp2);
	}
	{
		GameEntityPtrCon tmp3;
		m_scene_objs.swap(tmp3);

	}
	
}

void TankGameWorldImpl::ParseTower(TiXmlElement* e)
{
	TowerCreateInfo sinfo;
	sinfo.range = (float)atof(e->FirstChildElement("range")->Attribute("r"));

	TiXmlElement* model = e->FirstChildElement("model");
	if (model)
		ParseModelElem(&sinfo.sobj, model);

	TiXmlElement* matrixelem = e->FirstChildElement("transform");
	if (matrixelem)
		ParseMatrixElem(&sinfo.worldTransform, matrixelem);

	TowerEntity* tower = Globals::GetEntityFactory()->CreateTower(sinfo);
	m_towers.push_back(GameEntityPtr(tower));

}
void TankGameWorldImpl::ParseStartPoint(TiXmlElement* e)
{
	TiXmlElement* poselem = e->FirstChildElement("Pos");
	float x=(float)atof(poselem->Attribute("x"));
	float y=(float)atof(poselem->Attribute("z"));
	auto* p = Globals::GetEntityFactory()->CreateStartPoint(x,y);
	m_start.reset(p);
	
}
void TankGameWorldImpl::ParseEndPoint(TiXmlElement* e)
{
	TiXmlElement* poselem = e->FirstChildElement("Pos");
	float x=(float)atof(poselem->Attribute("x"));
	float y=(float)atof(poselem->Attribute("z"));
	auto* p = Globals::GetEntityFactory()->CreateTargetPoint(x,y);
	m_end.reset(p);
}
void TankGameWorldImpl::ParseSceneObj(TiXmlElement* e)
{
	SceneObjCreateInfo sinfo;
	TiXmlElement* model = e->FirstChildElement("model");
	if (model)
		ParseModelElem(&sinfo.sobj, model);

	TiXmlElement* matrixelem = e->FirstChildElement("transform");
	if (matrixelem)
		ParseMatrixElem(&sinfo.worldTransform, matrixelem);

	GameEntity* entity = Globals::GetEntityFactory()->CreateSceneObj(sinfo);
	m_scene_objs.push_back(GameEntityPtr(entity));
}

void TankGameWorldImpl::ParseModelElem(ModelCreateInfo* out, TiXmlElement* modelelem)
{
	auto mesh_elem=modelelem->FirstChildElement("mesh");
	out->meshFilename = mesh_elem->Attribute("filename");

	auto fx_elem = modelelem->FirstChildElement("fx");
	out->fxFilename = fx_elem->Attribute("filename");

	auto texture_elem = modelelem->FirstChildElement("texture");
	while (texture_elem)
	{
		out->textureFilename.push_back(std::string(texture_elem->Attribute("filename")));
		texture_elem = texture_elem->NextSiblingElement("texture");
	}
	
}
void TankGameWorldImpl::ParseMatrixElem(D3DXMATRIX* out, TiXmlElement* e)
{
	TiXmlElement* pos = e->FirstChildElement("pos");
	TiXmlElement* rot = e->FirstChildElement("rotate");
	TiXmlElement* scl = e->FirstChildElement("scale");
	D3DXMATRIX rotmatrix;
	D3DXMatrixRotationYawPitchRoll(&rotmatrix, 
		(float)atof(rot->Attribute("yaw")), 
		(float)atof(rot->Attribute("pitch")), 
		(float)atof(rot->Attribute("roll")));
	
	D3DXMATRIX sclmatrix;
	D3DXMatrixScaling(&sclmatrix, 
		(float)atof(scl->Attribute("x")), 
		(float)atof(scl->Attribute("y")), 
		(float)atof(scl->Attribute("z")));
	
	D3DXMatrixMultiply(out, &rotmatrix, &sclmatrix);
	out->_41 = (float)atof(pos->Attribute("x"));
	out->_42 = (float)atof(pos->Attribute("y"));
	out->_43 = (float)atof(pos->Attribute("z"));
}

//target:本次运动的目标点,即到了就停止运动
//返回值=true表示碰到了边界
bool TankGameWorldImpl::OnPlayerMove(RobotEntity* p,const D3DXVECTOR3& target,float v,DWORD timeInMs)
{
	D3DXVECTOR3 current_pos=*(p->GetPos());
	D3DXVECTOR3 dir = target - current_pos;
	D3DXVECTOR3 delta=dir * v * (float)(timeInMs * 0.001f);
	D3DXVECTOR3 next_pos = current_pos + delta;
	//判断是否超出边界

	for (int i = 0; i<BD_END; ++i)
	{
		const D3DXPLANE &border_plane = *m_pWorld->GetBorderPlane((BORDER_DEFINE)i);
		std::pair<bool, float> result = RayPlaneIntersection(current_pos, delta, D3DXVECTOR3(border_plane.a, border_plane.b, border_plane.c), border_plane.d); //这里用delta而不用dir，因为这样表示线段
		if (result.first && result.second>0 && result.second < 1)
		{
			//在运动过程中撞到了地形边界
			next_pos = current_pos - delta * result.second; //退回来一点，这里用减
			p->SetPostion(next_pos.x, next_pos.y, next_pos.z);
			return true;
		}
	}


	//如果离目标点比较近了 那么认为就已经到达了
	p->SetPostion(next_pos.x, next_pos.y, next_pos.z);
	D3DXVECTOR3 dis = (next_pos - target);
	return (D3DXVec3Length(&dis)<=5.f);
	
}