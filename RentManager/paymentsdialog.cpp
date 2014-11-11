#include "paymentsdialog.h"
#include "ui_paymentsdialog.h"

#include "myprinter.h"
#include "publics.h"
#include <QtSql>
#include <QSqlQueryModel>

PaymentsDialog::PaymentsDialog(QWidget *parent) :
	QDialog(parent),
	model(0),
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
	if (!model)
		model = new QSqlQueryModel(this);

	QSqlQuery qu = QSqlDatabase::database().exec("SELECT EntryID, ReceiptNo as 'Receipt No', "
						     "DateReceived as 'Date Received', "
						     "company.CompanyName as 'Company', "
						     "UnitName as 'Unit', TennantName as 'Tenant',"
						     "AmountReceived as 'Amount', PaymentFor as 'Payment For', "
						     "PayMode as 'Payment Mode', ChequeNo as 'Cheque No' "
						     " FROM payments, company WHERE payments.CompanyID = company.CompanyID");
	if (qu.lastError().isValid()) {
		Publics::showError(qu.lastError().text());
		this->close();
		return;
	}
	model->setQuery(qu);

	ui->trvPayments->setModel(model);
	ui->trvPayments->setColumnHidden(0, true);
}

void PaymentsDialog::on_trvPayments_doubleClicked(const QModelIndex &index)
{
	if (!model) {
		reloadPayments();
		return;
	}
	QModelIndex myIndex = model->index(index.row(), 0);
	QString paymentID = model->data(myIndex).toString();
	QString pSql = "SELECT * FROM payments WHERE EntryID = '" + paymentID + "'";
	QString companyID = Publics::getDbValue(pSql, "CompanyID").toString();
	QStringList stringsToReplace;
	QStringList stringsToUse;
	stringsToReplace << "param_entry_id" << "param_company_id";
	stringsToUse << paymentID << companyID;
	qDebug() << paymentID << companyID;
	MyPrinter::showReport("receipt", stringsToReplace, stringsToUse);
}
