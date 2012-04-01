#include "screenshotgetter.h"

ScreenshotGetter::ScreenshotGetter(QObject *parent = 0)
	: QThread(parent), d(new ScreenshotGetterPrivate)
{
	start();
}

ScreenshotGetter::~ScreenshotGetter()
{
	delete d;
}


void ScreenshotGetter::run()
{
	forever
	{
		d->updateImage();
		mutex.lock();
		img = d->img;
		mutex.unlock();
		emit imageUpdated();
		msleep(updateInterval);
	}
}

QPixmap ScreenshotGetter::image()
{
	mutex.lock();
	QPixmap result(img);
	mutex.unlock();
	return result;
}
