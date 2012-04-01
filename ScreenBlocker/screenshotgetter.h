#ifndef SCREENSHOTGETTER_H
#define SCREENSHOTGETTER_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include "screenshotgetterprivate.h"

static const int updateInterval = 1500; //in msec

class ScreenshotGetter : public QThread
{
	Q_OBJECT

public:
explicit ScreenshotGetter(QObject *parent);
	~ScreenshotGetter();

	QPixmap image();

protected:
	void run();

signals:
	void imageUpdated();

private:
	ScreenshotGetterPrivate *d;
	mutable QMutex mutex;
	QPixmap img;
};

#endif // SCREENSHOTGETTER_H
