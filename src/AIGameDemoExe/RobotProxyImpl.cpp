#include "RobotProxyImpl.h"
#include "RobotEntity.h"

RobotProxyImpl::RobotProxyImpl(RobotEntity* parent)
{
	m_Parent = parent;
}
D3DXVECTOR2 RobotProxyImpl::GetPos()
{
	return D3DXVECTOR2(m_Parent->GetPos()->x, m_Parent->GetPos()->z);
}
void RobotProxyImpl::SetVelocity(float v)
{
	m_Parent->SetVelocity(v);
}

float RobotProxyImpl::GetVelocity()
{
	return m_Parent->GetVelocity();
}

void RobotProxyImpl::SetDirection(float angle)
{
	return m_Parent->SetDirection(angle);
}
float RobotProxyImpl::GetDirection()
{
	return m_Parent->GetDirection();
}