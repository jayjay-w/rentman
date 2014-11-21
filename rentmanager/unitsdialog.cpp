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
	ui->txtMonthlyRent->setReadOnly(true);
	ui->txtMonthlyRent->setText("0");
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

	ui->txtOccupied->setText(qu.record().value("Occupied").toString());

	if (ui->txtOccupied->text() != "Yes") {
		ui->cmdEndOccupation->setVisible(false);
		ui->lblTenant->setVisible(false);
		ui->txtTenantName->setVisible(false);
	} else {
		ui->cmdEndOccupation->setVisible(true);
		ui->lblTenant->setVisible(true);
		QString rent = Publics::getDbValue("SELECT * FROM leases WHERE UnitID = '" + id + "'", "MonthlyRent").toString();
		ui->txtMonthlyRent->setText(rent);
		ui->txtTenantName->setVisible(true);
	}

	ui->spNoOfRooms->setValue(0);
	ui->spNoOfRooms->setValue(0);

	QString tenantName = Publics::getDbValue("SELECT * FROM tenant WHERE TenantID = '" + qu.record().value("CurrentTenantID").toString() + "'", "Name").toString();
	if (tenantName.length() > 0) {
		ui->txtTenantName->setText(tenantName);
	}

	ui->txtUnitNo->setText(qu.record().value("UnitNo").toString());
	ui->txtElecBillAcc->setText(qu.record().value("ElecBillAcc").toString());
	ui->txtWaterBillAc->setText(qu.record().value("WaterBillAc").toString());
	//ui->txtMonthlyRent->setText(qu.record().value("MonthlyRent").toString());
	ui->spNoOfRooms->setValue(qu.record().value("RoomCount").toInt());
	ui->spSqFt->setValue(qu.record().value("SQFT").toDouble());
	m_propertyID = qu.record().value("PropertyID").toString();

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
}

void UnitsDialog::on_cmdSave_clicked()
{
	QString sql = "SELECT NOW()";
	if (!isAdding) {
		sql = "UPDATE unit SET "
				"PropertyID = '" + m_propertyID + "', "
				"UnitNo = '" + ui->txtUnitNo->text() + "', "
				"RoomCount = '" + QString::number(ui->spNoOfRooms->value()) + "', "
				"MonthlyRent = '" + ui->txtMonthlyRent->text() + "', "
				"SQFT = '" + QString::number(ui->spSqFt->value()) + "', "
				"WaterBillAc = '" + ui->txtWaterBillAc->text() + "', "
				"ElecBillAcc = '" + ui->txtElecBillAcc->text() + "' WHERE "
				"UnitID = '" + m_unitID + "'";
	} else {
		sql = "INSERT INTO unit (UnitNo, PropertyID, RoomCount, MonthlyRent, "
				"SQFT, WaterBillAc, CurrentTenantID, Occupied, ElecBillAcc) VALUES ('"
				+ ui->txtUnitNo->text() + "', '"
				+ m_propertyID + "', '"
				+ QString::number(ui->spNoOfRooms->value()) + "', '"
				+ ui->txtMonthlyRent->text() + "', '"
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
