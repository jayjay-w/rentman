#ifndef SMARTPAYMENTDIALOG_H
#define SMARTPAYMENTDIALOG_H

#include <QDialog>

class QTreeWidgetItem;

namespace Ui {
class SmartPaymentDialog;
}

class SmartPaymentDialog : public QDialog
{
	Q_OBJECT

public:
	explicit SmartPaymentDialog(QWidget *parent = 0);
	~SmartPaymentDialog();
	void startNew();
private slots:
	void on_cboUnit_currentTextChanged(const QString &arg1);

	void on_cmdAutoAllocate_clicked();
	void updateTotals();
	void updateChecks();
	void on_trvPaymentAllocation_itemClicked(QTreeWidgetItem *item, int column);

	void on_trvPaymentAllocation_itemChanged(QTreeWidgetItem *item, int column);

	void on_cmdSave_clicked();

private:
	Ui::SmartPaymentDialog *ui;
	QString unitID, tenantID, tenantName, tenantTel, tenantEmail, companyID, companyName, propertyID, propertyName;
};

#endif // SMARTPAYMENTDIALOG_H
