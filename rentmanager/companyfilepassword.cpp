#include "companyfilepassword.h"
#include "ui_companyfilepassword.h"
#include <QMessageBox>
#include <QtSql>
CompanyFilePassword::CompanyFilePassword(QWidget *parent, bool newP) :
	QDialog(parent),
	ui(new Ui::CompanyFilePassword)
{
	ui->setupUi(this);
	if (newP)
		ui->txtCurrentPassword->setDisabled(true);

	isNewP = newP;
}

CompanyFilePassword::~CompanyFilePassword()
{
	delete ui;
}

void CompanyFilePassword::on_cmdOK_clicked()
{
    if (ui->txtConfirmPassword->text() != ui->txtNewPassword->text()) {
	    QMessageBox::critical(this, "Check password", "Your passwords do not match. Please re-enter");
	    return;
    }

    if (!isNewP) {
	QSqlQuery qu = QSqlDatabase::database().exec("SELECT * FROM password");
	if (!qu.lastError().isValid()) {
		qu.first();
		QString oldP = qu.record().value("Password").toString();
		if (oldP != ui->txtCurrentPassword->text()) {
			QMessageBox::critical(this, "Check password", "Your current password is incorrect. Please re-enter");
			return;
		}
	}
	QSqlDatabase::database().exec("CREATE TABLE IF NOT EXISTS 'password' ('Password' TEXT)");
	QSqlDatabase::database().exec("DELETE FROM password");
	QSqlDatabase::database().exec("INSERT INTO password ('Password') VALUES ('" + ui->txtNewPassword->text() + "')");
	this->accept();
    }

    newPassword = ui->txtNewPassword->text();
    currentPassword = ui->txtCurrentPassword->text();
    this->accept();
}
