#include "receivepaymentdialog.h"
#include "ui_receivepaymentdialog.h"

#include <QtSql>
#include "publics.h"

ReceivePaymentDialog::ReceivePaymentDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::ReceivePaymentDialog)
{
	ui->setupUi(this);

	Publics::loadQueryToCombo("SELECT * FROM tenant", "Name", ui->cboTenant);
	Publics::loadQueryToCombo("SELECT * FROM unit", "UnitNo", ui->cboUnit);
}

ReceivePaymentDialog::~ReceivePaymentDialog()
{
	delete ui;
}
