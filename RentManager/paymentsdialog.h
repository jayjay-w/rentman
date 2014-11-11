#ifndef PAYMENTSDIALOG_H
#define PAYMENTSDIALOG_H

#include <QDialog>

class QSqlQueryModel;
class QModelIndex;

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
	QSqlQueryModel *model;
private slots:
	void on_trvPayments_doubleClicked(const QModelIndex &index);

private:
	Ui::PaymentsDialog *ui;
};

#endif // PAYMENTSDIALOG_H
