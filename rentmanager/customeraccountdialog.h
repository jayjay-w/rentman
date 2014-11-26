#ifndef CUSTOMERACCOUNTDIALOG_H
#define CUSTOMERACCOUNTDIALOG_H

#include <QDialog>

namespace Ui {
class CustomerAccountDialog;
}

class CustomerAccountDialog : public QDialog
{
	Q_OBJECT

public:
	explicit CustomerAccountDialog(QWidget *parent = 0);
	~CustomerAccountDialog();

private:
	Ui::CustomerAccountDialog *ui;
};

#endif // CUSTOMERACCOUNTDIALOG_H
