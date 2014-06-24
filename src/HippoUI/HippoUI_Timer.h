/********************************************************************
	created:	2012/11/11
	created:	11:11:2012   23:44
	filename: 	F:\Git\ClassicHippoTestBox\Common\HippoUI\HippoUI_Timer.h
	file path:	F:\Git\ClassicHippoTestBox\Common\HippoUI
	file base:	HippoUI_Timer
	file ext:	h
	author:		sssa200
	
	purpose:	from dxut
*********************************************************************/
#pragma once
#include <Windows.h>
class HippoUI_Timer
{
public:
	HippoUI_Timer();

	void            Reset(); // resets the timer
	void            Start(); // starts the timer
	void            Stop();  // stop (or pause) the timer
	void            Advance(); // advance the timer by 0.1 seconds
	double          GetAbsoluteTime(); // get the absolute system time
	double          GetTime(); // get the current time
	float           GetElapsedTime(); // get the time that elapsed between Get*ElapsedTime() calls
	void            GetTimeValues( double* pfTime, double* pfAbsoluteTime, float* pfElapsedTime ); // get all time values at once
	bool            IsStopped(); // returns true if timer stopped

	// Limit the current thread to one processor (the current one). This ensures that timing code runs
	// on only one processor, and will not suffer any ill effects from power management.
	void            LimitThreadAffinityToCurrentProc();

protected:
	LARGE_INTEGER   GetAdjustedCurrentTime();

	bool m_bUsingQPF;
	bool m_bTimerStopped;
	LONGLONG m_llQPFTicksPerSec;

	LONGLONG m_llStopTime;
	LONGLONG m_llLastElapsedTime;
	LONGLONG m_llBaseTime;
};
