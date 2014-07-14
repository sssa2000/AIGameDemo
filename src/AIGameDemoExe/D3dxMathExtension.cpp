#include "D3dxMathExtension.h"


void ComposeMatrix(D3DXMATRIX* m, const D3DXVECTOR3& pos, const D3DXQUATERNION& rot, const D3DXVECTOR3& scl)
{
	D3DXMatrixRotationQuaternion(m,&rot);

	D3DXMATRIX sclmat;
	D3DXMatrixScaling(&sclmat,scl.x,scl.y,scl.z);
	D3DXMatrixMultiply(m,m,&sclmat);


	m->_41=pos.x;
	m->_42=pos.y;
	m->_43=pos.z;
}


D3DXQUATERNION GetRotationTo(const D3DXVECTOR3& From,const D3DXVECTOR3& To)
{
	D3DXQUATERNION res;

	D3DXVECTOR3 tempFrom = From;
	D3DXVECTOR3 tempTo = To;
	D3DXVec3Normalize(&tempFrom,&tempFrom);
	D3DXVec3Normalize(&tempTo,&tempTo);

	D3DXVECTOR3 Cross;
	D3DXVec3Cross(&Cross,&tempFrom,&tempTo);

	float d = D3DXVec3Dot(&tempFrom,&tempTo);
	if (d>=1.0f)
	{
		D3DXQuaternionIdentity(&res);
	}
	else if (d<=-0.999f)//when From = -To
	{
		if (fabs(From.x)>=0.01f)
		{
			Cross=D3DXVECTOR3((From.y+From.z)/From.x, -1.0f, -1.0f);//choose an arbitrary axis perpendicular to From
			D3DXVec3Normalize(&Cross,&Cross);
		}
		else if (fabs(From.y)>=0.01f)
		{
			Cross=D3DXVECTOR3(-1.0f, (From.x+From.z)/From.y, -1.0f);//choose an arbitrary axis perpendicular to From
			D3DXVec3Normalize(&Cross,&Cross);
		}
		else
		{
			Cross=D3DXVECTOR3(-1.0f, -1.0f, (From.x+From.y)/From.z);//choose an arbitrary axis perpendicular to From
			D3DXVec3Normalize(&Cross,&Cross);
		}

		float Half = D3DXToRadian(90);//we rotate 180 degrees
		res.w = cosf(Half);
		float HalfSin = sinf(Half);
		res.x = HalfSin*Cross.x;
		res.y = HalfSin*Cross.y;
		res.z = HalfSin*Cross.z;
	}
	else
	{
		float s = sqrtf( (1.0f+d)*2.0f );
		float invs = 1.0f/s;
		res.x = Cross.x * invs;
		res.y = Cross.y * invs;
		res.z = Cross.z * invs;
		res.w = s*0.5f;
	}
	return res;
}

//射线和平面相交，返回值first表示是否相交，second表示如果相交的话 交点在哪
std::pair<bool,float> RayPlaneIntersection(const D3DXVECTOR3& start,const D3DXVECTOR3& dir,const D3DXVECTOR3& planeNormal,float d)
{
	std::pair<bool,float> res(false,0);
	float d1 = D3DXVec3Dot(&planeNormal,&start)+d;
	float d2 = D3DXVec3Dot(&planeNormal,&dir);
	if ( d2 == 0.0f ) 
		return res;

	res.first=true;
	res.second = -( d1 / d2 );
	return res;
}