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
	//Publics::loadQueryToCombo("SELECT * FROM unit WHERE Occupied = 'No'", "UnitNo", ui->cboUnit);
	QSqlQuery qu = QSqlDatabase::database().exec("SELECT * FROM unit WHERE Occupied = 'No'");
	while (qu.next()) {
		QTreeWidgetItem *it = new QTreeWidgetItem(ui->trvUnits->invisibleRootItem());
		it->setText(0, qu.record().value("UnitNo").toString());
		QString s_unitID, propertyID, companyID, propertyCode, companyCode;
		s_unitID = Publics::getDbValue("SELECT * FROM unit WHERE UnitNo = '" + qu.record().value("UnitNo").toString() + "'", "UnitID").toString();
		propertyID = Publics::getDbValue("SELECT * FROM unit WHERE UnitID = '" + s_unitID + "'", "PropertyID").toString();
		companyID = Publics::getDbValue("SELECT * FROM property WHERE PropertyID = '" + propertyID + "'", "CompanyID").toString();
		propertyCode = Publics::getDbValue("SELECT * FROM property WHERE PropertyID = '" + propertyID + "'", "PropertyCode").toString();
		companyCode = Publics::getDbValue("SELECT * FROM company WHERE CompanyID = '" + companyID + "'", "Code").toString();

		it->setText(99, s_unitID);
		it->setText(1, companyCode);
		it->setText(2, propertyCode);
	}
}

void AssignUnitToTenantDialog::on_cmdSave_clicked()
{
	QString tenantID = Publics::getDbValue("SELECT * FROM tenant WHERE Name = '" + ui->cboTenant->currentText() + "'", "TenantID")
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

void AssignUnitToTenantDialog::on_trvUnits_itemClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);
	unitID = item->text(99);
}
