#ifndef RECEIVEPAYMENTDIALOG_H
#define RECEIVEPAYMENTDIALOG_H

#include <QDialog>

namespace Ui {
class ReceivePaymentDialog;
}

class ReceivePaymentDialog : public QDialog
{
	Q_OBJECT

public:
	explicit ReceivePaymentDialog(QWidget *parent = 0);
	~ReceivePaymentDialog();

private slots:
	void on_cmdSave_clicked();

private:
	Ui::ReceivePaymentDialog *ui;
};

#endif // RECEIVEPAYMENTDIALOG_H
