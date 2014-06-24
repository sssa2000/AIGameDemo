/********************************************************************
	created:	2014/05/21
	created:	21:5:2014   16:56
	filename: 	E:\TankGameExe\TankGameExe\Globals.h
	file path:	E:\TankGameExe\TankGameExe
	file base:	Globals
	file ext:	h
	author:		sssa2000
	
	purpose:	
*********************************************************************/
#pragma once

class EntityFactory;
class HippoD3d9Device;
class Timer;
class IGameControl;
class IGameWordContex;
class GameKeyBoardControl;
class CameraBase;
class FPSCamera;
class ModelViewCamera;
class FollowCamera;
class FxEffectManager;
class TankGameRender;
class HippoUI;

typedef IGameControl* (*CreateAIControl_FunPtr)();
typedef void (*DeleteAIControl_FunPtr)();


class Globals
{
public:
	static void Init(HippoD3d9Device* device, const char* dllname,int w,int h);
	static void CleanUp();
	static HippoD3d9Device* GetDevice();
	static EntityFactory* GetEntityFactory();
	static Timer* GetTimer();

	static FPSCamera* GetFpsCamera();
	static FollowCamera* GetFollowCamera();
	static ModelViewCamera* GetRTSCamera();
	static CameraBase* GetCurrentCamera();


	static IGameControl* GetCurrentControl();
	static GameKeyBoardControl* GetKeyBoardControl();
	static IGameWordContex* GetWorld();
	static void SwitchCameraAndControl(bool aimode);

	static FxEffectManager* GetFxManager();
	static TankGameRender* GetRender();

	static HippoUI* GetUI();
};


