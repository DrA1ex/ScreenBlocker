#include "passchecker.h"

#include <QCloseEvent>
#include <QSettings>
#include <QCryptographicHash>
#include "passchangedialog.h"

PassChecker::PassChecker(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	setWindowModality(Qt::ApplicationModal);
	this->setWindowFlags(Qt::Dialog
		| Qt::WindowStaysOnTopHint);

	QSettings *m_settings=new QSettings("A1ex Inc","ScreenBlocker",this);
	if(m_settings->value("Settings/pass").isNull())
		m_settings->setValue("Settings/pass", PassChangeDialog::computeHash(""));
	pass=m_settings->value("Settings/pass").toString();

	delete m_settings;

	connect(ui.bOk,SIGNAL(clicked()),SLOT(close()));
	connect(ui.ePass,SIGNAL(returnPressed()),SLOT(close()));
}

PassChecker::~PassChecker()
{

}

void PassChecker::closeEvent( QCloseEvent * event )
{
	if( PassChangeDialog::computeHash(ui.ePass->text().toLocal8Bit()) == pass )
		QApplication::exit(0);
	else
	{
		ui.ePass->clear();
		event->ignore();
	}
}

void PassChecker::on_bChangePass_clicked()
{
	PassChangeDialog *dlg = new PassChangeDialog(this);
	dlg->setModal(true);
	dlg->setAttribute(Qt::WA_DeleteOnClose, true);
	dlg->show();
}