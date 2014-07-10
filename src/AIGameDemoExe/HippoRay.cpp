#include "HippoRay.h"
D3DXVECTOR3 HippoRay::GetEndPoint(float length)
{
	//D3DXVECTOR3 res=
	return m_OrgPos + m_Dir*length;
}