#include "customeraccountdialog.h"
#include "ui_customeraccountdialog.h"

#include <QtSql>
#include "publics.h"
#include "tenantaccountrebuilddialog.h"

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

	QSqlQuery qu = QSqlDatabase::database().exec("SELECT * FROM tenant");
	while (qu.next()) {
		QTreeWidgetItem *it = new QTreeWidgetItem(ui->trvTenants->invisibleRootItem());
		it->setText(0, qu.record().value("Name").toString());
		it->setText(1, qu.record().value("Tel").toString());
		it->setText(99, qu.record().value("TenantID").toString());
	}

	ui->trvTenants->resizeColumnToContents(0);
	ui->trvTenants->resizeColumnToContents(1);
}

void CustomerAccountDialog::on_trvTenants_itemClicked(QTreeWidgetItem *item, int column)
{
	Q_UNUSED(column);
	QString tenantID = item->text(99);
	QSqlDatabase db = QSqlDatabase::database();
	db.exec("DELETE FROM tenant_account WHERE TenantID = '" + tenantID + "'");

	TenantAccountRebuildDialog *rebuild = new TenantAccountRebuildDialog(this, tenantID);
	if (rebuild->exec() == QDialog::Accepted) {
		//show the new account
		QSqlQuery qu = db.exec("SELECT * FROM tenant_account WHERE TenantID = '" + tenantID + "'");
		QSqlQueryModel *model = new QSqlQueryModel(this);
		model->setQuery(qu);
		ui->trvTransactions->setModel(model);
		ui->trvTransactions->setColumnHidden(0, true);
		ui->trvTransactions->setColumnHidden(1, true);
	}
}
