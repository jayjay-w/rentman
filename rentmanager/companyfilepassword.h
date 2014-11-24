#ifndef COMPANYFILEPASSWORD_H
#define COMPANYFILEPASSWORD_H

#include <QDialog>

namespace Ui {
class CompanyFilePassword;
}

class CompanyFilePassword : public QDialog
{
	Q_OBJECT

public:
	explicit CompanyFilePassword(QWidget *parent = 0, bool newP  =false);
	~CompanyFilePassword();
	QString currentPassword, newPassword;
private slots:
	void on_cmdOK_clicked();

private:
	Ui::CompanyFilePassword *ui;
	bool isNewP;
};

#endif // COMPANYFILEPASSWORD_H
