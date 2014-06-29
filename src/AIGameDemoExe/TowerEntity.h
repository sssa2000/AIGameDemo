/*!
 * \file TowerEntity.h
 * \date 2014/05/25 17:04
 *
 * \author BooSTLoki
 * Contact: user@company.com
 *
 * \brief sssa2000
 *
 * TODO: long description
 *
 * \note
 */
#pragma once
#include "GameEntity.h"
#include "TowerProxyImpl.h"
#include <memory>
#include "EntityFactory.h"

class ITowerProxy;
class TowerRenderable;

typedef std::vector<TowerRenderable*> TowerRenderableCon;

class TowerEntity :public GameEntity
{
public:
	friend class EntityFactory;
	friend class TowerProxyImpl;

	TowerEntity();
	~TowerEntity();
	virtual void  Update(unsigned int escapeTime);
	float GetRange(){ return m_range; }
	ITowerProxy* GetProxy(){ return (ITowerProxy*)m_pLogic.get(); }
	TowerRenderableCon* GetRenderable(){ return &m_pRenderables; }
	void Render(unsigned int escapeTime);
protected:
	void InitRenderable(TowerCreateInfo& towerInfo);
	bool IsPointInRange(const D3DXVECTOR3* point);
	std::tr1::shared_ptr<TowerProxyImpl> m_pLogic;
	float m_range;
	TowerRenderableCon m_pRenderables;
private:
};