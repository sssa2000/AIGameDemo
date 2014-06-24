#include "TowerProxyImpl.h"
#include "TowerEntity.h"

TowerProxyImpl::TowerProxyImpl(TowerEntity* parent)
{
	m_Parent = parent;
}
TowerProxyImpl::~TowerProxyImpl()
{

}

float TowerProxyImpl::GetRange()
{
	return m_Parent->GetRange();
}

D3DXVECTOR2 TowerProxyImpl::GetPos()
{
	auto pos3d = m_Parent->GetPos();
	return D3DXVECTOR2(pos3d->x, pos3d->z);
	
}