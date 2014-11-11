#ifndef PAYMENTSDIALOG_H
#define PAYMENTSDIALOG_H

#include <QDialog>

namespace Ui {
class PaymentsDialog;
}

class PaymentsDialog : public QDialog
{
	Q_OBJECT

public:
	explicit PaymentsDialog(QWidget *parent = 0);
	~PaymentsDialog();
	void reloadPayments();
private:
	Ui::PaymentsDialog *ui;
};

#endif // PAYMENTSDIALOG_H
