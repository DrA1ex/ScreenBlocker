#ifndef PASSCHECKER_H
#define PASSCHECKER_H

#include <QWidget>
#include "ui_passchecker.h"

class PassChecker : public QWidget
{
	Q_OBJECT

public:
	PassChecker(QWidget *parent = 0);
	~PassChecker();

protected:
	void closeEvent ( QCloseEvent * event );

private slots:
	void on_bChangePass_clicked();

private:
	Ui::PassChecker ui;
	QString pass;
};

#endif // PASSCHECKER_H
