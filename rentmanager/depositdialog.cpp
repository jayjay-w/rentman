#include "depositdialog.h"
#include "ui_depositdialog.h"
#include <QtSql>
#include <QTreeWidgetItem>
#include "publics.h"
#include "smartpaymentdialog.h"
#include "rentmanagermainwindow.h"

DepositDialog::DepositDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::DepositDialog)
{
	ui->setupUi(this);

	ui->cmdRefundDeposit->setEnabled(false);
}

DepositDialog::~DepositDialog()
{
	delete ui;
}

void DepositDialog::startNew()
{
	ui->trvUnits->invisibleRootItem()->takeChildren();
	unitID = "";
	deposit_available = "0";
	QSqlQuery qu = QSqlDatabase::database().exec("SELECT * FROM unit WHERE Occupied = 'Yes'");
	while (qu.next()) {
		QTreeWidgetItem *it = new QTreeWidgetItem(ui->trvUnits->invisibleRootItem());
		it->setText(0, qu.record().value("UnitNo").toString());
		QString s_unitID, propertyID, companyID, propertyCode, companyCode;
		s_unitID = Publics::getDbValue("SELECT * FROM unit WHERE UnitNo = '" + qu.record().value("UnitNo").toString() + "'", "UnitID").toString();
		propertyID = Publics::getDbValue("SELECT * FROM unit WHERE UnitID = '" + s_unitID + "'", "PropertyID").toString();
		companyID = Publics::getDbValue("SELECT * FROM property WHERE PropertyID = '" + propertyID + "'", "CompanyID").toString();
		propertyCode = Publics::getDbValue("SELECT * FROM property WHERE PropertyID = '" + propertyID + "'", "PropertyCode").toString();
		companyCode = Publics::getDbValue("SELECT * FROM company WHERE CompanyID = '" + companyID + "'", "Code").toString();
		QString currentTenantID = Publics::getDbValue("SELECT * FROM unit WHERE UnitID = '" + s_unitID + "'", "CurrentTenantID").toString();
		QString tenantName = Publics::getDbValue("SELECT * FROM tenant WHERE TenantID = '" + currentTenantID + "'", "Name").toString();
		QString leaseID = Publics::getDbValue("SELECT * FROM leases WHERE LeaseActive = 'Yes' AND TenantID = '" + currentTenantID + "' AND UnitID = '" + s_unitID + "'", "EntryID").toString();
		QString deposit, deposit_used, deposit_balance;
		deposit = Publics::getDbValue("SELECT * FROM leases WHERE EntryID = '" + leaseID + "'", "Deposit").toString();
		deposit_used = Publics::getDbValue("SELECT * FROM leases WHERE EntryID = '" + leaseID + "'", "DepositUsed").toString();
		deposit_balance = Publics::getDbValue("SELECT * FROM leases WHERE EntryID = '" + leaseID + "'", "DepositBalance").toString();
		it->setText(1, companyCode);
		it->setText(2, propertyCode);
		it->setText(3, tenantName);
		it->setText(4, deposit);
		it->setText(5, deposit_used);
		deposit_balance = QString::number(deposit.toDouble() - deposit_used.toDouble());
		it->setText(6, deposit_balance);

		it->setText(99, s_unitID);
		it->setText(100, leaseID);
	}

	ui->trvUnits->resizeColumnToContents(0);
	ui->trvUnits->resizeColumnToContents(1);
	ui->trvUnits->resizeColumnToContents(2);
	ui->trvUnits->resizeColumnToContents(3);
	ui->trvUnits->resizeColumnToContents(4);
	ui->trvUnits->resizeColumnToContents(5);
}

void DepositDialog::on_cmdUseDepositToPayInvoice_clicked()
{
	if (unitID.length() < 1) {
		Publics::showError("Select a unit to proceed");
		return;
	}
	if (!RentManagerMainWindow::instance()->m_smartPayment)
		RentManagerMainWindow::instance()->m_smartPayment = new SmartPaymentDialog(RentManagerMainWindow::instance());

	RentManagerMainWindow::instance()->m_smartPayment->startNewWithPayment(unitID, deposit_available, "Payment transferred from deposit.", true);
	if (RentManagerMainWindow::instance()->m_smartPayment->exec() == QDialog::Accepted) {
		double usedAmount = RentManagerMainWindow::instance()->m_smartPayment->amt_paid;
		QSqlDatabase::database().exec("UPDATE leases SET DepositUsed = DepositUsed + '" + QString::number(usedAmount) + "' WHERE EntryID = '" + m_leaseID + "'");
		startNew();
	}
}

void DepositDialog::on_trvUnits_itemClicked(QTreeWidgetItem *item, int column)
{
	Q_UNUSED(column);
	unitID = item->text(99);
	m_leaseID = item->text(100);
	deposit_available = item->text(6);
}
