#include "screenshotgetterprivate.h"
#include "thread.h"

bool isVistaOrLater()
{
	OSVERSIONINFO osvi;
	BOOL bIsWindowsXPorLater;

	ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

	GetVersionEx(&osvi);

	return osvi.dwMajorVersion >= 6;
}

static bool isGoodOs = isVistaOrLater();

uint counter = 0;

class WorkThread :public Thread
{
public:
	WorkThread(QPixmap &pixm) :img(pixm) {}

protected:
	unsigned run()
	{

		auto oldDesktop = OpenDesktop(L"SecureDesktop-QBlocker",
			NULL, NULL, GENERIC_ALL); 

		auto desktop = OpenDesktop(L"default", 0, TRUE, DESKTOP_SWITCHDESKTOP|  
			DESKTOP_WRITEOBJECTS| 
			DESKTOP_READOBJECTS| 
			DESKTOP_ENUMERATE| 
			DESKTOP_CREATEWINDOW| 
			DESKTOP_CREATEMENU); 
		if(desktop != NULL && oldDesktop != NULL)
		{
			SetThreadDesktop(desktop);
			if(!isGoodOs)
			{
				SwitchDesktop(desktop);
				Sleep(300);
			}
		}
		else
		{
			return -1;
		}

		makeScreenshot();

		if(desktop != NULL && oldDesktop != NULL)
		{
			SetThreadDesktop(oldDesktop);
			if(!isGoodOs)
				SwitchDesktop(oldDesktop);
			CloseDesktop(desktop);
			CloseDesktop(oldDesktop);
		}

		return 0;
	}

private:
	void makeScreenshot()
	{
		RECT r;
		GetClientRect(GetDesktopWindow(), &r);

		LONG w,h,x,y;


		w = r.right - r.left;
		h = r.bottom - r.top;
		x = 0;
		y = 0;

		// Create and setup bitmap
		HDC display_dc = GetDC(0);
		HDC bitmap_dc = CreateCompatibleDC(display_dc);
		HBITMAP bitmap = CreateCompatibleBitmap(display_dc, w, h);
		HGDIOBJ null_bitmap = SelectObject(bitmap_dc, bitmap);

		// copy data
		HDC window_dc = GetDC(GetDesktopWindow());
		BitBlt(bitmap_dc, 0, 0, w, h, window_dc, x, y, SRCCOPY 
			| CAPTUREBLT
			);

		// clean up all but bitmap
		ReleaseDC(GetDesktopWindow(), window_dc);
		SelectObject(bitmap_dc, null_bitmap);
		DeleteDC(bitmap_dc);
		
		img = QPixmap();
		img = QPixmap::fromWinHBITMAP(bitmap);

		DeleteObject(bitmap);
		ReleaseDC(0, display_dc);
	}

private:
	QPixmap &img;
};

void ScreenshotGetterPrivate::updateImage()
{
	if(!isGoodOs) 
		return; // Проще ничего не перерисовывать, чем мучиться с этим дерьмом мамонта

	WorkThread thread(img);
	thread.start();
	thread.wait();
}
