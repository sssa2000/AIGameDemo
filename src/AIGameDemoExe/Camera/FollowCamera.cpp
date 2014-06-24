#include "FollowCamera.h"
#include "GameEntity.h"
#include "GameRenderable.h"


FollowCamera::FollowCamera():
FPSCamera()
{

}
FollowCamera::~FollowCamera()
{

}
void FollowCamera::SetFollowTarget(GameEntity* p)
{
	m_pTarget = p;
	CalcCameraPosRotation();
}
int FollowCamera::FrameUpdate(unsigned int dElapsedTime)
{
	CalcCameraPosRotation();
	return 1;
}

void FollowCamera::CalcCameraPosRotation()
{
	//�õ���Χ�� ���������λ��
	float r = m_pTarget->GetBoundSphere()->_radius;
	//��Target��local����ϵ��ԭ�ؽ�����������ϵ
	//
	m_Pos.x = 0;
	m_Pos.y = 3 * r*asin(D3DXToRadian(45));
	m_Pos.z = -10*r;

	//�任������ռ�
	D3DXVECTOR3* tpos = m_pTarget->GetPos();
	m_Pos += *(tpos);
	m_ViewAtDir = (*tpos) - m_Pos;
	D3DXMATRIX* wm=m_pTarget->GetWorldTransform();
	//D3DXVec3TransformCoord(&m_ViewAtDir, &m_ViewAtDir, wm);
	//D3DXVec3TransformCoord(&m_RightDir, &m_RightDir, wm);
	//D3DXVec3TransformCoord(&m_UpDir, &m_UpDir, wm);
	//D3DXVec3Cross(&m_UpDir, &m_ViewAtDir, &m_RightDir);
	UpdateCameraCoord();
}

void FollowCamera::SetLookAtPos(float x, float y, float z)
{

}