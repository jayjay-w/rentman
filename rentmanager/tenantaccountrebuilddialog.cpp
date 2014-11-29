#include "tenantaccountrebuilddialog.h"
#include "ui_tenantaccountrebuilddialog.h"
#include "publics.h"

TenantAccountRebuildDialog::TenantAccountRebuildDialog(QWidget *parent, QString tenantID) :
	QDialog(parent),
	ui(new Ui::TenantAccountRebuildDialog)
{
	ui->setupUi(this);
	m_tenantID = tenantID;
	AccountRebuildThread *th = new AccountRebuildThread(this, m_tenantID);
	connect (th, SIGNAL(progress(int)),
		 ui->prgProgress, SLOT(setValue(int)));
	connect (th, SIGNAL(terminated()), SLOT(threadFinished()));
	connect (th, SIGNAL(finished()), SLOT(threadFinished()));

	th->start();
}

TenantAccountRebuildDialog::~TenantAccountRebuildDialog()
{
	delete ui;
}

void TenantAccountRebuildDialog::threadFinished()
{
	this->accept();
}


AccountRebuildThread::AccountRebuildThread(QObject *parent, QString tenantID)
	: QThread(parent)
{
	m_tenantID = tenantID;
}

void AccountRebuildThread::run()
{
	QSqlDatabase db = QSqlDatabase::database();
	//Fetch invoices
	db.exec("DELETE FROM tenant_account WHERE TenantID  = '" + m_tenantID + "'");
	QSqlQuery qu = db.exec("SELECT * FROM invoice_master WHERE TenantID = '" + m_tenantID + "'");
	while (qu.next()) {
		QString invoiceNo = qu.record().value("InvoiceNo").toString();
		QString invoiceDate = qu.record().value("InvoiceDate").toString();
		QString invoiceAmount = qu.record().value("InvoiceTotal").toString();

		QString tenantID, Name, Tel, ItemName, ItemNo, Amount, Balance;

		Name = Publics::getDbValue("SELECT * FROM tenant WHERE TenantID = '" + m_tenantID + "'", "Name").toString();
		Tel = Publics::getDbValue("SELECT * FROM tenant WHERE TenantID = '" + m_tenantID + "'", "Tel").toString();

		ItemName = "Invoice";
		ItemNo = invoiceNo;
		Amount = invoiceAmount;
		tenantID = m_tenantID;

		QString sql = "INSERT INTO tenant_account (TenantID, Name, Tel, ItemName, ItemNo, Amount, Balance) VALUES ('"
				+ tenantID + "', '" +
				Name + "', '" +
				Tel + "', '" +
				ItemName + "', '" +
				ItemNo  + "', '" +
				Amount  + "', '" +
				Balance + "')";
		db.exec(sql);
	}

}
