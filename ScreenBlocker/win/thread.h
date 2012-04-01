#pragma once

#include <windows.h>

class Thread
{
public:

	Thread(): m_thread(0), m_running(false)
		, m_finshed(false), m_id(0)
		, m_retCode(0), m_errorHappend(false){}

	virtual ~Thread(){ terminate(-1); }

	void start();
	void terminate(DWORD retCode = 0);
	BOOL wait(unsigned waitTime = -1);
	
	inline BOOL isFinshed() const {return m_finshed;}
	inline BOOL isRunning() const {return m_running;}
	inline BOOL errorHappend() {return m_errorHappend;}

	inline unsigned id() const {return m_id;}
	inline unsigned retCode() const {return m_retCode;}
	inline uintptr_t handle() const {return m_thread;}

	static unsigned currentThreadId();
	static uintptr_t currentThreadHandle();

protected:
	virtual unsigned run() = 0;
	virtual void finshed(){}
	virtual void started(){}

private:
	static unsigned WINAPI runThread(LPVOID param);

	uintptr_t m_thread;
	unsigned m_retCode;
	unsigned m_id;
	BOOL m_running;
	BOOL m_finshed;
	BOOL m_errorHappend;
};

