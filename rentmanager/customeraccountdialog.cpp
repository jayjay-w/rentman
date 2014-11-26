#include "customeraccountdialog.h"
#include "ui_customeraccountdialog.h"

CustomerAccountDialog::CustomerAccountDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::CustomerAccountDialog)
{
	ui->setupUi(this);
}

CustomerAccountDialog::~CustomerAccountDialog()
{
	delete ui;
}
