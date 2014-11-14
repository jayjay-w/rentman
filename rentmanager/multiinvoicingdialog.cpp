#include "multiinvoicingdialog.h"
#include "ui_multiinvoicingdialog.h"

MultiInvoicingDialog::MultiInvoicingDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::MultiInvoicingDialog)
{
	ui->setupUi(this);
}

MultiInvoicingDialog::~MultiInvoicingDialog()
{
	delete ui;
}
