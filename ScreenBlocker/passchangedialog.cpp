#include "passchangedialog.h"
#include <QCryptographicHash>
#include <QSettings>
#include <QMessageBox>

PassChangeDialog::PassChangeDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
}

PassChangeDialog::~PassChangeDialog()
{

}

void PassChangeDialog::accept()
{
	QString oldPass, newPass;
	oldPass = ui.eOld->text();
	newPass = ui.eNew->text();

	QSettings m_settings("A1ex Inc","ScreenBlocker",this);
	QString passHash=m_settings.value("Settings/pass").toString();

	if( computeHash(oldPass) != passHash )
	{
		QMessageBox::critical(this, QString::fromLocal8Bit("Ошибка"),
			QString::fromLocal8Bit("Пароль введен неверно"));
		return;
	}
	else if( newPass != ui.eConfirm->text() )
	{
		QMessageBox::critical(this, QString::fromLocal8Bit("Ошибка"),
			QString::fromLocal8Bit("Введенные пароли не совпадают"));
		return;
	}else if( computeHash(newPass) == passHash )
	{
		QMessageBox::critical(this, QString::fromLocal8Bit("Ошибка"),
			QString::fromLocal8Bit("Введенный пароль совпадает со старым. Предумайте другой пароль"));
	}

	m_settings.setValue("Settings/pass", computeHash(newPass));

	QMessageBox::information(this, QString::fromLocal8Bit("Операция завершена успешно"),
		QString::fromLocal8Bit("Пароль изменен. Изменения вступят в силу со следующего запуска программы."));
	close();
}

QString PassChangeDialog::computeHash( const QString& str )
{
	return QCryptographicHash::hash( QCryptographicHash::hash(salt.toLocal8Bit(), QCryptographicHash::Md5).toHex()
		+ QCryptographicHash::hash(str.toLocal8Bit(), QCryptographicHash::Md5).toHex(),
		QCryptographicHash::Md5).toHex();
}

QString PassChangeDialog::salt = "a]f_]%W:Fr";
