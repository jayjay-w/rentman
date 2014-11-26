#include "unitsdialog.h"
#include "ui_unitsdialog.h"

#include <QtSql>
#include "publics.h"
#include "terminateoccupationdialog.h"

UnitsDialog::UnitsDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::UnitsDialog)
{
	ui->setupUi(this);
}

UnitsDialog::~UnitsDialog()
{
	delete ui;
}

void UnitsDialog::edit(QString id)
{
	m_unitID = id;
	isAdding = false;
	Publics::clearTextBoxes(this);
	QSqlQuery qu = QSqlDatabase::database().exec(
				"SELECT * FROM unit WHERE "
				" UnitID = '" + id + "'"
				);
	qu.first();

	QSqlQuery leaseQu = QSqlDatabase::database().exec("SELECT * FROM leases WHERE LeaseActive = 'Yes' AND UnitID = '" + id + "'");
	bool hasLease = false;
	ui->txtOccupied->setText("No");
	QString tenantID;
	while (leaseQu.next()) {
		qDebug() << "Has lease" << tenantID;
		tenantID = leaseQu.record().value("TenantID").toString();
		hasLease = true;
		ui->txtOccupied->setText("Yes");
	}

	if (ui->txtOccupied->text() != "Yes") {
		ui->cmdEndOccupation->setVisible(false);
		ui->lblTenant->setVisible(false);
		ui->txtTenantName->setVisible(false);
	} else {
		ui->cmdEndOccupation->setVisible(false);
		ui->lblTenant->setVisible(true);
		//QString rent = Publics::getDbValue("SELECT * FROM leases WHERE UnitID = '" + id + "'", "MonthlyRent").toString();
		//ui->txtMonthlyRent->setText(rent);
		ui->txtTenantName->setVisible(true);
		QString tenantName = Publics::getDbValue("SELECT * FROM tenant WHERE TenantID = '" + tenantID + "'", "Name").toString();
		if (tenantName.length() > 0) {
			ui->txtTenantName->setText(tenantName);
		}
	}

	ui->spNoOfRooms->setValue(0);
	ui->spNoOfRooms->setValue(0);



	ui->txtUnitNo->setText(qu.record().value("UnitNo").toString());
	ui->txtElecBillAcc->setText(qu.record().value("ElecBillAcc").toString());
	ui->txtWaterBillAc->setText(qu.record().value("WaterBillAc").toString());
	//ui->txtMonthlyRent->setText(qu.record().value("MonthlyRent").toString());
	ui->spNoOfRooms->setValue(qu.record().value("RoomCount").toInt());
	ui->spSqFt->setValue(qu.record().value("SQFT").toDouble());
	m_propertyID = qu.record().value("PropertyID").toString();

	ui->txtUnitNo->setFocus();
}

void UnitsDialog::addNew(QString propertyID)
{
	Publics::clearTextBoxes(this);
	ui->spNoOfRooms->setValue(0);
	ui->spNoOfRooms->setValue(0);
	m_propertyID = propertyID;
	isAdding  = true;
	ui->cmdEndOccupation->setVisible(false);
	ui->lblTenant->setVisible(false);
	ui->txtTenantName->setVisible(false);
	ui->txtOccupied->setText("No");
	ui->txtUnitNo->setFocus();
}

void UnitsDialog::on_cmdSave_clicked()
{
	QString sql = "SELECT NOW()";
	if (!isAdding) {
		sql = "UPDATE unit SET "
				"PropertyID = '" + m_propertyID + "', "
				"UnitNo = '" + ui->txtUnitNo->text() + "', "
				"RoomCount = '" + QString::number(ui->spNoOfRooms->value()) + "', "
				"SQFT = '" + QString::number(ui->spSqFt->value()) + "', "
				"WaterBillAc = '" + ui->txtWaterBillAc->text() + "', "
				"ElecBillAcc = '" + ui->txtElecBillAcc->text() + "' WHERE "
				"UnitID = '" + m_unitID + "'";
	} else {
		sql = "INSERT INTO unit (UnitNo, PropertyID, RoomCount,"
				"SQFT, WaterBillAc, CurrentTenantID, Occupied, ElecBillAcc) VALUES ('"
				+ ui->txtUnitNo->text() + "', '"
				+ m_propertyID + "', '"
				+ QString::number(ui->spNoOfRooms->value()) + "', '"
				+ QString::number(ui->spSqFt->value()) + "', '"
				+ ui->txtWaterBillAc->text() + "', '0', 'No', '"
				+ ui->txtElecBillAcc->text() + "')";
	}

	qDebug() << sql;

	QSqlQuery qu = QSqlDatabase::database().exec(sql);
	if (qu.lastError().isValid()) {
		Publics::showError(qu.lastError().text());
		return;
	}
	this->accept();
}

void UnitsDialog::on_cmdEndOccupation_clicked()
{
	TerminateOccupationDialog *term = new TerminateOccupationDialog(this);
	if (term->exec() == QDialog::Accepted) {
		edit(m_unitID);
	}
}
