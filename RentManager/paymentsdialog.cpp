#include "paymentsdialog.h"
#include "ui_paymentsdialog.h"

#include "publics.h"
#include <QtSql>
#include <QSqlQueryModel>

PaymentsDialog::PaymentsDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::PaymentsDialog)
{
	ui->setupUi(this);
}

PaymentsDialog::~PaymentsDialog()
{
	delete ui;
}

void PaymentsDialog::reloadPayments()
{
	QSqlQueryModel *model = new QSqlQueryModel(this);
	QSqlQuery qu = QSqlDatabase::database().exec("SELECT EntryID, ReceiptNo as 'Receipt No', "
						     "DateReceived as 'Date Received', "
						     "UnitName as 'Unit', TennantName as 'Tenant',"
						     "AmountReceived as 'Amount', PaymentFor as 'Payment For', "
						     "PayMode as 'Payment Mode', ChequeNo as 'Cheque No' "
						     " FROM payments");
	if (qu.lastError().isValid()) {
		Publics::showError(qu.lastError().text());
		this->close();
		return;
	}
	model->setQuery(qu);

	ui->trvPayments->setModel(model);
	ui->trvPayments->setColumnHidden(0, true);
}
