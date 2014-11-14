#ifndef MULTIINVOICINGDIALOG_H
#define MULTIINVOICINGDIALOG_H

#include <QDialog>

namespace Ui {
class MultiInvoicingDialog;
}

class MultiInvoicingDialog : public QDialog
{
	Q_OBJECT

public:
	explicit MultiInvoicingDialog(QWidget *parent = 0);
	~MultiInvoicingDialog();

private:
	Ui::MultiInvoicingDialog *ui;
};

#endif // MULTIINVOICINGDIALOG_H
