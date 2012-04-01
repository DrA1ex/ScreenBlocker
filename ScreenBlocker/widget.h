#ifndef WIDGET_H
#define WIDGET_H

#include <QtGui/QWidget>
#include <QImage>
#include <QGraphicsBlurEffect>
#include "screenshotgetter.h"
#include "passchecker.h"

class Widget : public QWidget
{
	Q_OBJECT

public:
	Widget(QWidget *parent = 0, Qt::WFlags flags = 0);
	~Widget();

protected:
	void paintEvent(QPaintEvent *e);
	void closeEvent(QCloseEvent *e);

private:
	ScreenshotGetter getter;
	PassChecker checker;
	QImage *foreground;

private slots:
	void imageUpdated();		
};

#endif // WIDGET_H
