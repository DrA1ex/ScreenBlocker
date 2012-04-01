#include "thread.h"
#include <winnt.h>
#include <process.h>


void Thread::start()
{	
	m_errorHappend = false;
	if(!isRunning())
		m_thread = _beginthreadex(NULL,NULL,&Thread::runThread,this,NULL,&m_id);
	if(m_thread != NULL)
		InterlockedExchange((LONG*)&m_running, TRUE);
}

void Thread::terminate( DWORD retCode /*= 0*/ )
{
	TerminateThread((HANDLE)m_thread, retCode);
	m_running=false;
}

uintptr_t WINAPI Thread::runThread( LPVOID param )
{
	Thread* self = (Thread*)param;
	try
	{
		(self->*(&Thread::started))();
		InterlockedExchange(&(self->*(&Thread::m_retCode)), (self->*(&Thread::run))());
		(self->*(&Thread::finshed))();
	}
	catch(...)
	{
		InterlockedExchange((LONG*)&(self->*(&Thread::m_running)), FALSE);
		InterlockedExchange((LONG*)&(self->*(&Thread::m_errorHappend)), TRUE);
		InterlockedExchange((LONG*)&(self->*(&Thread::m_finshed)), TRUE);
	}

	return self->*(&Thread::m_retCode);
}

BOOL Thread::wait(unsigned waitTime )
{
	if(WaitForSingleObject((HANDLE)m_thread, waitTime) == WAIT_OBJECT_0)
		return TRUE;

	return FALSE;
}

unsigned Thread::currentThreadId()
{
 	return GetCurrentThreadId();
}

uintptr_t Thread::currentThreadHandle()
{
	return (uintptr_t)GetCurrentThread();
}