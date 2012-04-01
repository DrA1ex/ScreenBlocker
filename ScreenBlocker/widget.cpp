#include "widget.h"

#include <QPainter>
#include <QPaintEvent>
#include <QPixmap>
#include <QDesktopWidget>


const qreal blurRadius = 4;
const auto foregroundColor = 0x66000000;


Widget::Widget(QWidget *parent, Qt::WFlags flags)
	: QWidget(parent, flags), getter(this)
{
	this->setWindowFlags(Qt::FramelessWindowHint 
		| Qt::CustomizeWindowHint);
	setFocusPolicy(Qt::NoFocus);
 	this->setGeometry(0,0,QApplication::desktop()->width(),QApplication::desktop()->height());		

	connect(&getter, SIGNAL(imageUpdated()),SLOT(imageUpdated()));

	foreground = new QImage(width(), height(), QImage::Format_ARGB32);
	foreground->fill(foregroundColor);

	auto effect = new QGraphicsBlurEffect;
	effect->setBlurRadius(4);
	setGraphicsEffect(effect);


	checker.move(this->geometry().center()-checker.rect().center());
	checker.show();
}

Widget::~Widget()
{
	delete foreground;
}

void Widget::paintEvent( QPaintEvent *e )
{
	QPainter painter(this);
	painter.drawPixmap(0, 0, getter.image());
	painter.drawImage(0, 0, *foreground);
}

void Widget::imageUpdated()
{
	repaint();
}

void Widget::closeEvent( QCloseEvent *e )
{
	e->ignore();
}
