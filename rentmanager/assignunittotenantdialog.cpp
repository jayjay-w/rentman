#include "assignunittotenantdialog.h"
#include "ui_assignunittotenantdialog.h"

#include "publics.h"
#include <QDebug>

AssignUnitToTenantDialog::AssignUnitToTenantDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::AssignUnitToTenantDialog)
{
	ui->setupUi(this);
	ui->dtpStartDate->setDate(QDate::currentDate());

	connect (ui->txtMonthlyRent, SIGNAL(textChanged(QString)), SLOT(calculateVAT()));
}

AssignUnitToTenantDialog::~AssignUnitToTenantDialog()
{
	delete ui;
}

void AssignUnitToTenantDialog::reloadLists()
{
	Publics::loadQueryToCombo("SELECT * FROM tenant", "Name", ui->cboTenant);
	Publics::loadQueryToCombo("SELECT * FROM unit WHERE Occupied = 'No'", "UnitNo", ui->cboUnit);
}

void AssignUnitToTenantDialog::on_cmdSave_clicked()
{
	QString tenantID = Publics::getDbValue("SELECT * FROM tenant WHERE Name = '" + ui->cboTenant->currentText() + "'", "TenantID")
			.toString();
	QString unitID = Publics::getDbValue("SELECT * FROM unit WHERE UnitNo = '" + ui->cboUnit->currentText() + "'", "UnitID")
			.toString();


	QString sql =
			"INSERT INTO leases (UnitID, TenantID, DateOfOccupation"
			",MonthlyRent,Deposit) VALUES ('"
			+ unitID + "', '"
			+ tenantID + "', '"
			+ ui->dtpStartDate->date().toString("dd-MMM-yyyy") + "', '"
			+ ui->txtMonthlyRent->text() + "', '"
			+ ui->txtDepositPaid->text() + "')";

	QSqlQuery qu = QSqlDatabase::database().exec(sql);
	if (qu.lastError().isValid()) {
		Publics::showError(qu.lastError().text());
		return;
	}

	QString sql2 = "UPDATE unit SET Occupied = 'Yes', CurrentTenantID = '" + tenantID + "' WHERE UnitID = '" + unitID + "'";
	qDebug() << sql;
	qDebug() << sql2;
	qu = QSqlDatabase::database().exec(sql2);

	if (qu.lastError().isValid()) {
		Publics::showError(qu.lastError().text());
		return;
	}

	this->accept();
}

void AssignUnitToTenantDialog::on_chkVAT_clicked()
{
	calculateVAT();
}

void AssignUnitToTenantDialog::calculateVAT()
{
	if (ui->chkVAT->isChecked()) {
		quint64 total = ui->txtMonthlyRent->text().toDouble();
		quint64 vat = 0;
		quint64 rent = 0;
		vat = 0.137931034483 * total;
		rent = total - vat;
		qDebug() << total << vat << rent;
		ui->txtRent->setText(QString::number(rent));
		ui->txtVAT->setText(QString::number(vat));
	} else {
		ui->txtRent->setText("0");
		ui->txtVAT->setText("0");
	}
}

void AssignUnitToTenantDialog::on_chkVAT_toggled(bool checked)
{
    calculateVAT();
}
