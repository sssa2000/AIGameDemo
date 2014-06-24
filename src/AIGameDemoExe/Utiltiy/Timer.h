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

//��ʱ��
class Timer
{
public:
	void Release(){delete this;}
	Timer();
	//!��ʼ��
	void Init();
	//!�õ��ӳ�ʼ����ʼ�����һ��update��ȥ��ʱ��(��)
	float GetElapsedTime()const{ return m_fElapsedTime; }
	//!�õ�������γ�ʼ����ļ��(��)
	float GetIntverval()const{ return m_fInterval; }
	//!�õ�������γ�ʼ����ļ��(ms)
	float GetIntvervalMS()const{ return m_fInterval*1000.0f; }
	//!����
	void Update();
	

protected:
	//!��ʼ��ʱ��tick
	LARGE_INTEGER m_BeginTime;
	//!���һ��update��tick
	LARGE_INTEGER m_CurrentTime;
	//!tick/��
	LARGE_INTEGER m_Frequency;
	//!�ӳ�ʼ����ʼ�����һ��update��ȥ��ʱ��(��)
	float                         m_fElapsedTime;
	//!������γ�ʼ����ļ��(��)
	float                         m_fInterval;

};