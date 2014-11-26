#include "customeraccountdialog.h"
#include "ui_customeraccountdialog.h"

#include <QtSql>
#include "publics.h"

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

void CustomerAccountDialog::startNew()
{
	ui->trvTenants->invisibleRootItem()->takeChildren();
	ui->trvTransactions->invisibleRootItem()->takeChildren();
}
