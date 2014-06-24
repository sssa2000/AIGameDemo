#include "HippoUI_Timer.h"
#include <assert.h>
//--------------------------------------------------------------------------------------
HippoUI_Timer::HippoUI_Timer()
{
	m_bTimerStopped = true;
	m_llQPFTicksPerSec = 0;

	m_llStopTime = 0;
	m_llLastElapsedTime = 0;
	m_llBaseTime = 0;

	// Use QueryPerformanceFrequency to get the frequency of the counter
	LARGE_INTEGER qwTicksPerSec = { 0 };
	QueryPerformanceFrequency( &qwTicksPerSec );
	m_llQPFTicksPerSec = qwTicksPerSec.QuadPart;
}


//--------------------------------------------------------------------------------------
void HippoUI_Timer::Reset()
{
	LARGE_INTEGER qwTime = GetAdjustedCurrentTime();

	m_llBaseTime = qwTime.QuadPart;
	m_llLastElapsedTime = qwTime.QuadPart;
	m_llStopTime = 0;
	m_bTimerStopped = FALSE;
}


//--------------------------------------------------------------------------------------
void HippoUI_Timer::Start()
{
	// Get the current time
	LARGE_INTEGER qwTime = { 0 };
	QueryPerformanceCounter( &qwTime );

	if( m_bTimerStopped )
		m_llBaseTime += qwTime.QuadPart - m_llStopTime;
	m_llStopTime = 0;
	m_llLastElapsedTime = qwTime.QuadPart;
	m_bTimerStopped = FALSE;
}


//--------------------------------------------------------------------------------------
void HippoUI_Timer::Stop()
{
	if( !m_bTimerStopped )
	{
		LARGE_INTEGER qwTime = { 0 };
		QueryPerformanceCounter( &qwTime );
		m_llStopTime = qwTime.QuadPart;
		m_llLastElapsedTime = qwTime.QuadPart;
		m_bTimerStopped = TRUE;
	}
}


//--------------------------------------------------------------------------------------
void HippoUI_Timer::Advance()
{
	m_llStopTime += m_llQPFTicksPerSec / 10;
}


//--------------------------------------------------------------------------------------
double HippoUI_Timer::GetAbsoluteTime()
{
	LARGE_INTEGER qwTime = { 0 };
	QueryPerformanceCounter( &qwTime );

	double fTime = qwTime.QuadPart / ( double )m_llQPFTicksPerSec;

	return fTime;
}


//--------------------------------------------------------------------------------------
double HippoUI_Timer::GetTime()
{
	LARGE_INTEGER qwTime = GetAdjustedCurrentTime();

	double fAppTime = ( double )( qwTime.QuadPart - m_llBaseTime ) / ( double )m_llQPFTicksPerSec;

	return fAppTime;
}


//--------------------------------------------------------------------------------------
void HippoUI_Timer::GetTimeValues( double* pfTime, double* pfAbsoluteTime, float* pfElapsedTime )
{
	assert( pfTime && pfAbsoluteTime && pfElapsedTime );

	LARGE_INTEGER qwTime = GetAdjustedCurrentTime();

	float fElapsedTime = ( float )( ( double )( qwTime.QuadPart - m_llLastElapsedTime ) / ( double )
		m_llQPFTicksPerSec );
	m_llLastElapsedTime = qwTime.QuadPart;

	// Clamp the timer to non-negative values to ensure the timer is accurate.
	// fElapsedTime can be outside this range if processor goes into a 
	// power save mode or we somehow get shuffled to another processor.  
	// However, the main thread should call SetThreadAffinityMask to ensure that 
	// we don't get shuffled to another processor.  Other worker threads should NOT call 
	// SetThreadAffinityMask, but use a shared copy of the timer data gathered from 
	// the main thread.
	if( fElapsedTime < 0.0f )
		fElapsedTime = 0.0f;

	*pfAbsoluteTime = qwTime.QuadPart / ( double )m_llQPFTicksPerSec;
	*pfTime = ( qwTime.QuadPart - m_llBaseTime ) / ( double )m_llQPFTicksPerSec;
	*pfElapsedTime = fElapsedTime;
}


//--------------------------------------------------------------------------------------
float HippoUI_Timer::GetElapsedTime()
{
	LARGE_INTEGER qwTime = GetAdjustedCurrentTime();

	double fElapsedTime = ( float )( ( double )( qwTime.QuadPart - m_llLastElapsedTime ) / ( double )
		m_llQPFTicksPerSec );
	m_llLastElapsedTime = qwTime.QuadPart;

	// See the explanation about clamping in HippoUI_Timer::GetTimeValues()
	if( fElapsedTime < 0.0f )
		fElapsedTime = 0.0f;

	return ( float )fElapsedTime;
}


//--------------------------------------------------------------------------------------
// If stopped, returns time when stopped otherwise returns current time
//--------------------------------------------------------------------------------------
LARGE_INTEGER HippoUI_Timer::GetAdjustedCurrentTime()
{
	LARGE_INTEGER qwTime;
	if( m_llStopTime != 0 )
		qwTime.QuadPart = m_llStopTime;
	else
		QueryPerformanceCounter( &qwTime );
	return qwTime;
}

//--------------------------------------------------------------------------------------
bool HippoUI_Timer::IsStopped()
{
	return m_bTimerStopped;
}

//--------------------------------------------------------------------------------------
// Limit the current thread to one processor (the current one). This ensures that timing code 
// runs on only one processor, and will not suffer any ill effects from power management.
// See "Game Timing and Multicore Processors" for more details
//--------------------------------------------------------------------------------------
void HippoUI_Timer::LimitThreadAffinityToCurrentProc()
{
	HANDLE hCurrentProcess = GetCurrentProcess();

	// Get the processor affinity mask for this process
	DWORD_PTR dwProcessAffinityMask = 0;
	DWORD_PTR dwSystemAffinityMask = 0;

	if( GetProcessAffinityMask( hCurrentProcess, &dwProcessAffinityMask, &dwSystemAffinityMask ) != 0 &&
		dwProcessAffinityMask )
	{
		// Find the lowest processor that our process is allows to run against
		DWORD_PTR dwAffinityMask = ( dwProcessAffinityMask & ( ( ~dwProcessAffinityMask ) + 1 ) );

		// Set this as the processor that our thread must always run against
		// This must be a subset of the process affinity mask
		HANDLE hCurrentThread = GetCurrentThread();
		if( INVALID_HANDLE_VALUE != hCurrentThread )
		{
			SetThreadAffinityMask( hCurrentThread, dwAffinityMask );
			CloseHandle( hCurrentThread );
		}
	}

	CloseHandle( hCurrentProcess );
}

