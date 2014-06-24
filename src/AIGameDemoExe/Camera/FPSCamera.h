/********************************************************************
created:	2011/12/26
created:	26:12:2011   22:45
filename: 	f:\TestHippo\TestHippo\HIPPO_FrameWork\camera\FPSCamera.h
file path:	f:\TestHippo\TestHippo\HIPPO_FrameWork\camera
file base:	FPSCamera
file ext:	h
author:		sssa2000

purpose:	
*********************************************************************/
#pragma once

#include "Win32MsgUtil.h"
#include <d3dx9math.h>
#include "CameraBase.h"


class FPSCamera:public CameraBase
{
public:
	FPSCamera();
	virtual ~FPSCamera();


	void SetPos(float x,float y,float z);
	virtual void SetLookAtPos(float x,float y,float z);


	int FrameUpdate(unsigned int dElapsedTime);
	void LookAt(IDirect3DDevice9* p);

	const D3DXVECTOR3* GetPos(){ return &m_Pos; }
	const D3DXVECTOR3* GetDir(){ return &m_ViewAtDir; }
	const D3DXVECTOR3* GetUp(){ return &m_UpDir; }

	MouseKeyCallback GetMouseRightDownCallback();
	MouseKeyCallback GetMouseMoveCallback();
	MouseKeyCallback GetMouseRightUpCallback();
	MouseWheelCallback GetMouseWheelCallback();
	KeyCallback GetKeyDownCallback();


protected:


	int OnMouseRightDown(HippoMouseEvent& e);
	int OnMouseRightUp(HippoMouseEvent& e);
	int OnMouseMove(HippoMouseEvent& e);
	int OnMouseWheel(HippoWheelEvent& w);
	int OnKeyDown(unsigned int);


	//D3DXQUATERNION m_quat;
	D3DXMATRIX m_rotation;
	D3DXVECTOR3  m_Pos;
	D3DXVECTOR3  m_ViewAtDir;
	D3DXVECTOR3  m_UpDir;
	D3DXVECTOR3  m_RightDir;

	//!运动的方向，计算加速度会用到
	D3DXVECTOR3  m_vDirection;
	D3DXVECTOR3  m_vVelocity;

	//!更新相机的坐标系即up、right、lookat
	void UpdateCameraCoord();
	int AnalysisKeyInput(float fElapsedTime);
	int AnalysisMouseInput(float fElapsedTime);

	void SetPosDelta(const D3DXVECTOR3& posdelta);
	void SetPosDelta(float dir);

	void RoateWithYAix(float degreeInRad);
	void RoateWithXAix(float degreeInRad);

	bool m_bRightHasDown;
	int m_nLastMouseX;
	int m_nLastMouseY;
};

