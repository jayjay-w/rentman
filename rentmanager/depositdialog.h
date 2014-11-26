#ifndef DEPOSITDIALOG_H
#define DEPOSITDIALOG_H

#include <QDialog>

class QTreeWidgetItem;

namespace Ui {
class DepositDialog;
}

class DepositDialog : public QDialog
{
	Q_OBJECT

public:
	explicit DepositDialog(QWidget *parent = 0);
	~DepositDialog();

	void startNew();
private slots:
	void on_cmdUseDepositToPayInvoice_clicked();

	void on_trvUnits_itemClicked(QTreeWidgetItem *item, int column);

private:
	Ui::DepositDialog *ui;
	QString unitID;
	QString deposit_available;
	QString m_leaseID;
};

#endif // DEPOSITDIALOG_H
