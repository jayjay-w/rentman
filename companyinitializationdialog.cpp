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
	executeInitSql(Publics::getSql(Publics::SQL_PROPERTIES));
	executeInitSql(Publics::getSql(Publics::SQL_TENANTS));
	executeInitSql(Publics::getSql(Publics::SQL_UNITS));
	executeInitSql(Publics::getSql(Publics::SQL_TENANT_UNITS));
	emit progress(100);


	//Insert jo groups
//	qu = db.exec("SELECT Count(JobGroupID) as 'cnt' FROM JobGroups");
//	if (!qu.lastError().isValid()) {
//		qu.first();
//		int cnt = qu.record().value("cnt").toInt();
//		if (cnt < 1) {
//			//No j groups
//			emit status("Creating job groups");
//			//Insert job groups
//			db.exec("INSERT INTO JobGroups (JobGroup) VALUES ('-No Job Group-')");
//			db.exec("INSERT INTO JobGroups (JobGroup) VALUES ('A')");
//			db.exec("INSERT INTO JobGroups (JobGroup) VALUES ('B')");
//			db.exec("INSERT INTO JobGroups (JobGroup) VALUES ('C')");
//			db.exec("INSERT INTO JobGroups (JobGroup) VALUES ('D')");
//		}
//	}
}

void DatabaseInitThread::executeInitSql(QString sql)
{
	QSqlDatabase db = QSqlDatabase::database();
	//Create tables
	db.exec(sql);
	if (db.lastError().isValid())
		qDebug() << db.lastError().text();
}
