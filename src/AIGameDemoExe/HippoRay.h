/********************************************************************
	created:	2014/07/09
	created:	9:7:2014   11:03
	filename: 	E:\AIGameDemo\src\AIGameDemoExe\HippoRay.h
	file path:	E:\AIGameDemo\src\AIGameDemoExe
	file base:	HippoRay
	file ext:	h
	author:		sssa2000
	
	purpose:	
*********************************************************************/
#pragma once
#include <d3dx9.h>

class HippoRay
{
public:
	HippoRay(D3DXVECTOR3& orgPos,D3DXVECTOR3& dir){m_OrgPos=orgPos;m_Dir=dir;}

	~HippoRay(){}
	D3DXVECTOR3 GetEndPoint(float length);
	D3DXVECTOR3 m_OrgPos;
	D3DXVECTOR3 m_Dir;
};