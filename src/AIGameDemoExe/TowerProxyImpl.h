/*!
 * \file TowerProxyImpl.h
 * \date 2014/05/25 17:09
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
#include "ITowerProxy.h"

class TowerEntity;
class TowerProxyImpl:public ITowerProxy
{
public:
	TowerProxyImpl(TowerEntity* parent);
	~TowerProxyImpl();

	float GetRange();
	D3DXVECTOR2 GetPos();
protected:
	TowerEntity* m_Parent;
private:
};