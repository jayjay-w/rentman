#ifndef CREATEINVOICEDIALOG_H
#define CREATEINVOICEDIALOG_H

#include <QDialog>

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
private:
	Ui::CreateInvoiceDialog *ui;
};

#endif // CREATEINVOICEDIALOG_H
