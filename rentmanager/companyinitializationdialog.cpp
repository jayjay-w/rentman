#include "companyinitializationdialog.h"
#include "ui_companyinitializationdialog.h"
#include "publics.h"
CompanyInitializationDialog::CompanyInitializationDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::CompanyInitializationDialog)
{
	ui->setupUi(this);
	DatabaseInitThread *th = new DatabaseInitThread(this);
	connect (th, SIGNAL(progress(int))
		 , ui->prgProgress, SLOT(setValue(int)));
	connect (th, SIGNAL(status(QString))
		 , ui->lblCurrentAction, SLOT(setText(QString)));
	connect (th, SIGNAL(finished())
		 , SLOT(threadFinished()));

	th->start();
}

CompanyInitializationDialog::~CompanyInitializationDialog()
{
	delete ui;
}

void CompanyInitializationDialog::threadFinished()
{
	this->accept();
}


DatabaseInitThread::DatabaseInitThread(QObject *parent) :
	QThread(parent)
{

}

void DatabaseInitThread::run()
{
	QSqlDatabase db = QSqlDatabase::database();
	Q_UNUSED(db);
	//Create tables
	emit progress(0);
	executeInitSql(Publics::getSql(Publics::SQL_COMPANY));
	executeInitSql(Publics::getSql(Publics::SQL_FILE));
	executeInitSql(Publics::getSql(Publics::SQL_PROPERTIES));
	executeInitSql(Publics::getSql(Publics::SQL_TENANTS));
	executeInitSql(Publics::getSql(Publics::SQL_UNITS));
	executeInitSql(Publics::getSql(Publics::SQL_TENANT_UNITS));
	executeInitSql(Publics::getSql(Publics::SQL_PAYMENTS));
	executeInitSql(Publics::getSql(Publics::SQL_INVOICE_DETAIL));
	executeInitSql(Publics::getSql(Publics::SQL_INVOICE_MASTER));
	executeInitSql(Publics::getSql(Publics::SQL_INVOICE_ITEMS));
	executeInitSql(Publics::getSql(Publics::SQL_PAYMENT_ALLOCATION));
	executeInitSql(Publics::getSql(Publics::SQL_REPORT_CONTACT_LIST));
	executeInitSql(Publics::getSql(Publics::SQL_REPORT_OCCUPIED_UNITS));
	executeInitSql(Publics::getSql(Publics::SQL_REPORT_PROPERTIES));
	executeInitSql(Publics::getSql(Publics::SQL_REPORT_VACANT_UNITS));
	db.exec("DROP TABLE tenant_account");
	executeInitSql(Publics::getSql(Publics::SQL_TENANT_ACCOUNT));
	db.exec("CREATE TABLE IF NOT EXISTS 'password' ('Password' TEXT)");
	emit progress(100);


	//Insert invoice items
	QSqlQuery qu = db.exec("SELECT Count(EntryID) as 'cnt' FROM invoice_item");
	if (!qu.lastError().isValid()) {
		qu.first();
		if (qu.record().value("cnt").toInt() < 1) {
			db.exec("INSERT INTO invoice_item (ItemName, Description) VALUES ('Rent', 'Montly Rent')");
			db.exec("INSERT INTO invoice_item (ItemName, Description) VALUES ('Rent Deposit', 'Rent security deposit')");
			db.exec("INSERT INTO invoice_item (ItemName, Description) VALUES ('Service Charge', 'Misc. Services charge')");
			db.exec("INSERT INTO invoice_item (ItemName, Description) VALUES ('Security Fee', 'Property Security')");
		}
	}

	//Sustained table changes (db v1)
	db.exec("ALTER table leases ADD COLUMN 'DepositBalance' TEXT DEFAULT('0')");
	db.exec("ALTER table leases ADD COLUMN 'DepositUsed' TEXT DEFAULT('0')");
	db.exec("ALTER table leases ADD COLUMN 'LeaseActive' TEXT DEFAULT('Yes')");
	db.exec("ALTER table payments ADD COLUMN 'Narration' TEXT");
	db.exec("ALTER table tenant ADD COLUMN 'Balance' TEXT Default('0')");

	db.exec("DELETE FROM version");
	db.exec("INSERT INTO version('DbVersion') VALUES ('2')");

	//Reload unit status
	db.exec("UPDATE unit SET CurrentTenantID = '', Occupied='No'");
	QSqlQuery leaseQu = db.exec("SELECT * FROM leases WHERE LeaseActive='Yes'");
	while (leaseQu.next()) {
		QString unitID = leaseQu.record().value("UnitID").toString();
		QString tenantID = leaseQu.record().value("TenantID").toString();
		db.exec("UPDATE unit SET Occupied = 'Yes', CurrentTenantID = '" + tenantID + "' WHERE UnitID = '" + unitID + "'");
	}
}

void DatabaseInitThread::executeInitSql(QString sql)
{
	QSqlDatabase db = QSqlDatabase::database();
	//Create tables
	db.exec(sql);
	if (db.lastError().isValid())
		qDebug() << db.lastError().text();
}
