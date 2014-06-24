/*!
 * \file AIControlImplExample.h
 * \date 2014/05/26 0:42
 *
 * \author BooSTLoki
 * Contact: user@company.com
 *
 * \brief sssa2000
 *
 * TODO: 简单的示范如何编写一个简单的ai控制器
 *
 * \note
*/
#pragma once

#include "IGameControl.h"

class AIControlImplExample:public IGameControl
{
public:
	AIControlImplExample();
	~AIControlImplExample();

	void OnTick(IGameWorld* p, unsigned int frameTime);
protected:
private:
};