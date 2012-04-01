#ifndef SCREENSHOTGETTERPRIVATE_H
#define SCREENSHOTGETTERPRIVATE_H

#include <QObject>
#include <QPixmap>

class ScreenshotGetterPrivate
{
public:

	void updateImage();
	QPixmap img;	
};

#endif // SCREENSHOTGETTERPRIVATE_H
