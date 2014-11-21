#ifndef CREATEINVOICEDIALOG_H
#define CREATEINVOICEDIALOG_H

#include <QDialog>
#include <QTimer>

namespace Ui {
class CreateInvoiceDialog;
}

class CreateInvoiceDialog : public QDialog
{
	Q_OBJECT

public:
	explicit CreateInvoiceDialog(QWidget *parent = 0);
	~CreateInvoiceDialog();
	void startNew();
private slots:
    void on_cboUnits_currentIndexChanged(int index);

    void on_cmdAddItemToInvoice_clicked();

    void on_cmdSaveInvoice_clicked();
	void onTimer();
private:
	Ui::CreateInvoiceDialog *ui;
	void addItem(QString itemName, QString description, QString amount);

    QString unitID, tenantID, tenantName, tenantTel, tenantEmail;
    QTimer *tmr;
};

#endif // CREATEINVOICEDIALOG_H
