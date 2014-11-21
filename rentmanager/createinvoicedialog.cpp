#include "createinvoicedialog.h"
#include "ui_createinvoicedialog.h"


#include "publics.h"
#include "additemtoinvoicedialog.h"
#include <QDebug>
#include <QUuid>

CreateInvoiceDialog::CreateInvoiceDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::CreateInvoiceDialog)
{
	ui->setupUi(this);
	ui->dtpInvoiceDate->setDate(QDate::currentDate());
	tmr = new QTimer(this);
	connect (tmr, SIGNAL(timeout()), SLOT(onTimer()));
	tmr->start(250);
}

CreateInvoiceDialog::~CreateInvoiceDialog()
{
	delete ui;
}

void CreateInvoiceDialog::startNew()
{
	QString unitSql = "SELECT * FROM unit WHERE Occupied = 'Yes'";
	Publics::loadQueryToCombo(unitSql, "UnitNo", ui->cboUnits);
	//addItem("Rent", "November 2014 Rent", "25000");
}

void CreateInvoiceDialog::addItem(QString itemName, QString description, QString amount)
{
	ui->tblInvoiceItems->insertRow(ui->tblInvoiceItems->rowCount());
	QTableWidgetItem *nameItem = new QTableWidgetItem(itemName);
	QTableWidgetItem *descItem = new QTableWidgetItem(description);
	QTableWidgetItem *amountItem = new QTableWidgetItem(amount);

	nameItem->setText(itemName);
	descItem->setText(description);
	amountItem->setText(amount);

	ui->tblInvoiceItems->setItem(ui->tblInvoiceItems->rowCount() - 1, 0, nameItem);
	ui->tblInvoiceItems->setItem(ui->tblInvoiceItems->rowCount() - 1, 1, descItem);
	ui->tblInvoiceItems->setItem(ui->tblInvoiceItems->rowCount() - 1, 2, amountItem);
}


void CreateInvoiceDialog::on_cboUnits_currentIndexChanged(int index)
{
	Q_UNUSED(index);
	ui->tblInvoiceItems->clearContents();
	for (int i = 0; i < ui->tblInvoiceItems->rowCount(); i++) {
		ui->tblInvoiceItems->removeRow(0);
	}
	QString unitNo = ui->cboUnits->currentText();
	unitID = Publics::getDbValue("SELECT * FROM unit WHERE UnitNO = '" + unitNo + "'", "UnitID").toString();
	tenantID = Publics::getDbValue("SELECT * FROM leases WHERE UnitID = '" + unitID + "'", "TenantID").toString();
	tenantName = Publics::getDbValue("SELECT * FROM tenant WHERE TenantID = '" + tenantID + "'", "Name").toString();
	tenantTel = Publics::getDbValue("SELECT * FROM tenant WHERE TenantID = '" + tenantID + "'", "Tel").toString();
	tenantEmail = Publics::getDbValue("SELECT * FROM tenant WHERE TenantID = '" + tenantID + "'", "Email").toString();

	QString rentAmount = Publics::getDbValue("SELECT * FROM leases WHERE UnitID = '" + unitID + "'", "MonthlyRent").toString();

	ui->txtTenantDetails->setPlainText(tenantName + "\n" + tenantTel + "\n" + tenantEmail);
	addItem("Rent", "Monthly Rent", rentAmount);
}

void CreateInvoiceDialog::on_cmdAddItemToInvoice_clicked()
{
	AddItemToInvoiceDialog *adder = new AddItemToInvoiceDialog(this);

	if (adder->exec() == QDialog::Accepted) {
		addItem(adder->itemName(), adder->itemDescription(), adder->itemCost());
	}
}

void CreateInvoiceDialog::on_cmdSaveInvoice_clicked()
{
	QString total, words, propID, compID;
	QString unique = QUuid::createUuid().toString();
	double d_total = 0;

	QStringList items;

	for (int i = 0; i < ui->tblInvoiceItems->rowCount(); i++) {
		QString itemName = ui->tblInvoiceItems->item(i, 0)->text();
		QString itemCost = ui->tblInvoiceItems->item(i, 2)->text();
		double d_itemCost = itemCost.toDouble();
		items.append("INSERT INTO invoice_detail (UniqueID, ItemName, Amount) VALUES ('"
			     + unique + "', '"
			     + itemName + "', '"
			     + itemCost + "'"
					  ")");
		total = total + d_itemCost;
	}

	total = QString::number(d_total);

	QString insertMaster = "INSERT INTO invoice_master (UniqueID, TenantID, "
			       "UnitID, PropertyID, CompanyID, InvoiceDate, TenantName, TenantAddress, "
			       "InvoiceTotal, AmountInWords) VALUES ('"
			+ unique + "', '"
			+ tenantID + "', '"
			+ unitID + "', '"
			+ propID + "', '"
			+ compID + "', '"
			+ ui->dtpInvoiceDate->date().toString("dd-MMM-yyyy") + "', '"
			+ tenantName + "', '"
			+ tenantTel + "\n" + tenantEmail + "', '"
			+ total + "', '"
			+ words + "')";

	for (int i = 0; i < items.count(); i++) {
		QSqlQuery qu = QSqlDatabase::database().exec(items.at(i));
		if (qu.lastError().isValid()) {
			qDebug() << items.at(i) << qu.lastError().text();
		}
	}

	QSqlQuery qu = QSqlDatabase::database().exec(insertMaster);
	if (qu.lastError().isValid())
		Publics::showError(qu.lastError().text());
}

void CreateInvoiceDialog::onTimer()
{
	long d_total = 0;

	for (int i = 0; i < ui->tblInvoiceItems->rowCount(); i++) {
		QString itemCost = ui->tblInvoiceItems->item(i, 2)->text();
		long d_itemCost = itemCost.toDouble();
		d_total = d_total + d_itemCost;
	}
	long vat = 0;
	ui->txtSubtotal->setText(QString::number(d_total));

	if (ui->chkHasVAT->isChecked()) {
		vat = (0.16 * d_total);
	}

	ui->txtVATAmount->setText(QString::number(vat));
	ui->txtGrandTotal->setText(QString::number(vat + d_total));
}
