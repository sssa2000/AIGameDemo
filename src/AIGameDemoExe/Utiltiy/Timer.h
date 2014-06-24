#pragma once

#include <windows.h>
struct LocalTime
{

	LocalTime()
	{
		year = month = day = hour = minute = second = 0;
	}

	short year;
	short month;
	short day;
	short hour;
	short minute;
	short second;
};

//计时器
class Timer
{
public:
	void Release(){delete this;}
	Timer();
	//!初始化
	void Init();
	//!得到从初始化开始到最近一次update过去的时间(秒)
	float GetElapsedTime()const{ return m_fElapsedTime; }
	//!得到最近两次初始化间的间隔(秒)
	float GetIntverval()const{ return m_fInterval; }
	//!得到最近两次初始化间的间隔(ms)
	float GetIntvervalMS()const{ return m_fInterval*1000.0f; }
	//!更新
	void Update();
	

protected:
	//!初始化时的tick
	LARGE_INTEGER m_BeginTime;
	//!最近一次update的tick
	LARGE_INTEGER m_CurrentTime;
	//!tick/秒
	LARGE_INTEGER m_Frequency;
	//!从初始化开始到最近一次update过去的时间(秒)
	float                         m_fElapsedTime;
	//!最近两次初始化间的间隔(秒)
	float                         m_fInterval;

};