#include "Timer.h"
#include <stdio.h>
Timer::Timer()
{
	m_BeginTime.QuadPart = 0;
	m_CurrentTime.QuadPart = 0;
	m_Frequency.QuadPart = 1000;
	m_fInterval = 0.0f;
	m_fElapsedTime = 0.0f;
}

void Timer::Init()
{
	QueryPerformanceFrequency(&m_Frequency);
	QueryPerformanceCounter(&m_BeginTime);
	QueryPerformanceCounter(&m_CurrentTime);
}

void Timer::Update()
{
	QueryPerformanceCounter(&m_CurrentTime);
	float newElapsedTime = float(m_CurrentTime.QuadPart - m_BeginTime.QuadPart) / m_Frequency.QuadPart;
	m_fInterval = newElapsedTime - m_fElapsedTime;
	m_fElapsedTime = newElapsedTime;
}
