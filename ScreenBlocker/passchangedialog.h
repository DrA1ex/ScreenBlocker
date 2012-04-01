#ifndef PASSCHANGEDIALOG_H
#define PASSCHANGEDIALOG_H

#include <QDialog>
#include "ui_passchangedialog.h"

class PassChangeDialog : public QDialog
{
	Q_OBJECT

public:
	PassChangeDialog(QWidget *parent = 0);
	~PassChangeDialog();

	static QString salt;
	static QString computeHash(const QString& str);

public slots:
	void accept();

private:
	Ui::PassChangeDialog ui;
};

#endif // PASSCHANGEDIALOG_H
