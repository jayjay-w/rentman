#include "tenantaccountrebuilddialog.h"
#include "ui_tenantaccountrebuilddialog.h"

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

}

void AccountRebuildThread::run()
{
	QSqlDatabase db = QSqlDatabase::database();
	//Fetch invoices
	QSqlQuery qu = db.exec("SELECT * FROM invoice_master WHERE TenantID = '" + m_tenantID + "'");
}
