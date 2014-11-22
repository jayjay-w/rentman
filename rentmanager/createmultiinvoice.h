#ifndef CREATEMULTIINVOICE_H
#define CREATEMULTIINVOICE_H

#include <QDialog>
#include <QList>

class QTimer;
class QTreeWidgetItem;

namespace Ui {
class CreateMultiInvoice;
}

class CreateMultiInvoice : public QDialog
{
	Q_OBJECT

public:
	explicit CreateMultiInvoice(QWidget *parent = 0);
	~CreateMultiInvoice();
	void startNew();
private slots:
	void currentUnitChanged(const QString &arg1);
	void onTimer();
	void on_spinMonths_valueChanged(int arg1);

	void on_cmdSave_clicked();

	void on_trvUnits_itemClicked(QTreeWidgetItem *item, int column);

private:
	Ui::CreateMultiInvoice *ui;
	QTimer *tmr;

	QString unitID;
	QString tenantID;
	QString tenantName, tenantTel, tenantEmail;
};



#endif // CREATEMULTIINVOICE_H
