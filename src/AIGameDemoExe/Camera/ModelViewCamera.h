/********************************************************************
	created:	2014/06/24
	created:	24:6:2014   15:30
	filename: 	E:\AIGameDemo\src\AIGameDemoExe\Camera\ModelViewCamera.h
	file path:	E:\AIGameDemo\src\AIGameDemoExe\Camera
	file base:	ModelViewCamera
	file ext:	h
	author:		sssa2000
	
	purpose:	
*********************************************************************/
#pragma once

#include "CameraBase.h"
#include "Win32MsgUtil.h"
#include <d3dx9math.h>

class ModelViewCamera:public CameraBase
{
public:
	ModelViewCamera();
	virtual ~ModelViewCamera();

	void SetR(float r){m_r=r;}
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

	D3DXVECTOR3 m_ViewAtPoint;
	float m_r;		//半径
	float m_xz_delta; //xz平面的角度
	float m_y_delta;	//y平面的角度

	//!更新相机的坐标系即up、right、lookat
	void UpdateCameraCoord();
	int AnalysisKeyInput(float fElapsedTime);
	int AnalysisMouseInput(float fElapsedTime);

	void SetPosDelta(const D3DXVECTOR3& posdelta);
	void SetPosDelta(float dir);

	void CalcMouseMove(unsigned int dElapsedTime);
	//void Roate(float degreeInRadXZ,float degreeInRadY);

	bool m_bRightHasDown;
	int m_nLastMouseX;
	int m_nLastMouseY;


};