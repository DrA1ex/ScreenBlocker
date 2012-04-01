#include "widget.h"
#include <QtGui/QApplication>

#include <qt_windows.h>

#ifdef Q_WS_WIN
HANDLE hMutexOneInstance;

bool isExclusive() 
{
	const QString GUID_str("BLOCKER-088FA840-B10D-11D3-BC36-006067709674");
	QString tmp;
	const size_t NAMELENGTH = 64;
	wchar_t userName[NAMELENGTH];
	DWORD userNameLength = NAMELENGTH;
	wchar_t domainName[NAMELENGTH];
	DWORD domainNameLength = NAMELENGTH;

	if(GetUserName(userName, &userNameLength))
	{
		domainNameLength = ExpandEnvironmentStrings(L"%USERDOMAIN%",
			domainName,
			NAMELENGTH);

		tmp.append(QString::fromWCharArray(domainName));
		tmp.append("-");
		tmp.append(QString::fromWCharArray(userName));
	} 

	tmp.append("-");
	tmp.append(GUID_str);

	WCHAR mutexName[NAMELENGTH*3];
	tmp.toWCharArray(mutexName);
	mutexName[tmp.length()]=L'\0';
	hMutexOneInstance = ::CreateMutex( NULL, TRUE,  mutexName);
	bool AlreadyRunning = (GetLastError() == ERROR_ALREADY_EXISTS);
	if (hMutexOneInstance != NULL) 
	{
		::ReleaseMutex(hMutexOneInstance);
	}

	return !AlreadyRunning;
}
#endif // Q_WS_WIN

int main(int argc, char *argv[])
{
#ifdef Q_WS_WIN
	HDESK oldDesktop = NULL, desktop = NULL;
#endif

	int result = -1;

	try
	{
#ifdef Q_WS_WIN
		if(!isExclusive())
			return -1;

		oldDesktop = OpenDesktop(L"default", 0, TRUE, DESKTOP_SWITCHDESKTOP|  
			DESKTOP_WRITEOBJECTS| 
			DESKTOP_READOBJECTS| 
			DESKTOP_ENUMERATE| 
			DESKTOP_CREATEWINDOW| 
			DESKTOP_CREATEMENU); 
		desktop = CreateDesktop(L"SecureDesktop-QBlocker", NULL, NULL, NULL, GENERIC_ALL, NULL);
		if(desktop != NULL && oldDesktop != NULL)
		{
			SetThreadDesktop(desktop);
			SwitchDesktop(desktop);
		}
#endif

		QApplication a(argc, argv);
		Widget w;
		w.show();
		result = a.exec();
	}
	catch(...) {}

#ifdef Q_WS_WIN
	if(desktop != NULL && oldDesktop != NULL)
	{
		SetThreadDesktop(oldDesktop);
		SwitchDesktop(oldDesktop);
		CloseDesktop(desktop);
	}
	CloseHandle(hMutexOneInstance);
#endif
	return result;
}
