#include "companiesdialog.h"
#include "ui_companiesdialog.h"

CompaniesDialog::CompaniesDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::CompaniesDialog)
{
	ui->setupUi(this);
}

CompaniesDialog::~CompaniesDialog()
{
	delete ui;
}
