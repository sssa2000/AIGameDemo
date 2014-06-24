#pragma once
#include <d3dx9math.h>

class IRobotProxy;
class ITowerProxy;


class IGameWorld
{
public:
	virtual ~IGameWorld() = 0{};

	//得到出生点的位置
	virtual D3DXVECTOR2 GetBornPoint() = 0; 

	//得到目标点(即终点)的位置
	virtual D3DXVECTOR2 GetTargetPoint() = 0; 

	//得到操作的机器人的实例
	virtual IRobotProxy* GetPlayerRobot() = 0;

	//得到tower的数量
	virtual unsigned int GetTowerNumber() = 0;

	//通过idx检索得到tower的实例，传入的idx不能大于等于GetTowerNumber返回的值
	virtual ITowerProxy* GetTowerByIdx(unsigned int idx) = 0;
};
