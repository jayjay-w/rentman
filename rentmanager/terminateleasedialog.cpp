#include "terminateleasedialog.h"
#include "ui_terminateleasedialog.h"

#include <QtSql>
#include "publics.h"

TerminateLeaseDialog::TerminateLeaseDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::TerminateLeaseDialog)
{
	ui->setupUi(this);
}

TerminateLeaseDialog::~TerminateLeaseDialog()
{
	delete ui;
}

void TerminateLeaseDialog::startNew()
{
	ui->dtpEndOfLease->setDate(QDate::currentDate());
	ui->trvLeases->invisibleRootItem()->takeChildren();
	QSqlQuery qu = QSqlDatabase::database().exec("SELECT * FROM leases WHERE LeaseActive = 'Yes'");
	while (qu.next()) {
		QString tenantID = qu.record().value("TenantId").toString();
		QString s_unitID, propertyID, companyID, propertyCode, companyCode, tenantName;
		tenantName = Publics::getDbValue("SELECT * FROM tenant WHERE TenantID = '" + tenantID + "'", "Name").toString();
		s_unitID = qu.record().value("UnitID").toString();
		QString unitNo = Publics::getDbValue("SELECT * FROM unit WHERE UnitID = '" + s_unitID + "'", "UnitNo").toString();
		propertyID = Publics::getDbValue("SELECT * FROM unit WHERE UnitID = '" + s_unitID + "'", "PropertyID").toString();
		companyID = Publics::getDbValue("SELECT * FROM property WHERE PropertyID = '" + propertyID + "'", "CompanyID").toString();
		propertyCode = Publics::getDbValue("SELECT * FROM property WHERE PropertyID = '" + propertyID + "'", "PropertyCode").toString();
		companyCode = Publics::getDbValue("SELECT * FROM company WHERE CompanyID = '" + companyID + "'", "Code").toString();

		QTreeWidgetItem *it = new QTreeWidgetItem(ui->trvLeases->invisibleRootItem());
		it->setText(99, qu.record().value("EntryID").toString());
		it->setText(0, tenantName);
		it->setText(1, unitNo);
		it->setText(2, propertyCode);
		it->setText(3, companyCode);
		QString bal = "0";
		bal = Publics::getDbValue("SELECT Sum(Balance) as bal FROM invoice_master WHERE UnitID = '" + s_unitID + "' AND TenantID = '" + tenantID + "'", "bal").toString();
		it->setText(4, bal);
	}

	ui->trvLeases->resizeColumnToContents(2);
	ui->trvLeases->resizeColumnToContents(3);
}

void TerminateLeaseDialog::on_cmdTerminateLease_clicked()
{
	if (m_leaseID.length() < 1) {
		Publics::showError("Please select  a lease to terminate.");
		return;
	}

	QSqlQuery qu = QSqlDatabase::database().exec("UPDATE leases SET LeaseActive = 'No', ExitDate = '" + ui->dtpEndOfLease->date()
						     .toString("dd-MMM-yyyy") + "' WHERE EntryID = '" + m_leaseID + "'");
	if (qu.lastError().isValid()) {
		Publics::showError(qu.lastError().text());
		return;
	} else {
		this->accept();
	}

}

void TerminateLeaseDialog::on_trvLeases_itemClicked(QTreeWidgetItem *item, int column)
{
	Q_UNUSED(column);
	m_leaseID = item->text(99);
}
