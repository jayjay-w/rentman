#include "createmultiinvoice.h"
#include "ui_createmultiinvoice.h"

#include "publics.h"
#include <QDate>
#include <QTimer>
#include <QDebug>
#include <QMap>
#include <QUuid>

CreateMultiInvoice::CreateMultiInvoice(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::CreateMultiInvoice)
{
	ui->setupUi(this);
	tmr = new QTimer(this);
	connect (tmr, SIGNAL(timeout()), SLOT(onTimer()));
}

CreateMultiInvoice::~CreateMultiInvoice()
{
	delete ui;
}

void CreateMultiInvoice::startNew()
{
	Publics::loadQueryToCombo("SELECT UnitNo FROM unit WHERE Occupied = 'Yes'", "UnitNo", ui->cboUnits);
}

void CreateMultiInvoice::on_cboUnits_currentIndexChanged(const QString &arg1)
{
	QString unitNo = arg1;
	unitID = Publics::getDbValue("SELECT * FROM unit WHERE UnitNo = '" + unitNo + "'", "UnitID").toString();
	QString leaseID = Publics::getDbValue("SELECT * FROM leases WHERE UnitID = '" + unitID + "'", "EntryID").toString();
	QString rent = Publics::getDbValue("SELECT * FROM leases WHERE EntryID = '" + leaseID + "'", "MonthlyRent").toString();
	tenantID = Publics::getDbValue("SELECT * FROM leases WHERE UnitID = '" + unitID + "'", "TenantID").toString();
	tenantName = Publics::getDbValue("SELECT * FROM tenant WHERE TenantID = '" + tenantID + "'", "Name").toString();
	tenantTel = Publics::getDbValue("SELECT * FROM tenant WHERE TenantID = '" + tenantID + "'", "Tel").toString();
	tenantEmail = Publics::getDbValue("SELECT * FROM tenant WHERE TenantID = '" + tenantID + "'", "Email").toString();
	ui->spinRent->setValue(rent.toDouble());
	ui->txtTenantDetails->setPlainText(tenantName + "\n" + tenantTel + "\n" + tenantEmail);
}

void CreateMultiInvoice::onTimer()
{

}

void CreateMultiInvoice::on_spinMonths_valueChanged(int arg1)
{
	QDate startDate = QDate(ui->txtYear->text().toInt(), ui->cboMonth->currentIndex()  + 1, 1);

	int rs = ui->tblPreview->rowCount();
	for (int i = 0; i < rs; i++) {
		ui->tblPreview->removeRow(0);
	}

	for (int r = 0; r < arg1; r++) {
		ui->tblPreview->insertRow(r);
		QDate thisMonthStart = startDate.addMonths(r);
		thisMonthStart = QDate(thisMonthStart.year(), thisMonthStart.month(), 1);
		QString monthYear = thisMonthStart.toString(thisMonthStart.toString("M") + "/" + ui->txtYear->text());
		QString itemName = "Rent";
		QString itemDesc = "Rent for " + thisMonthStart.toString("MMM-yyyy");
		QString amt = QString::number(ui->spinRent->value());
		QTableWidgetItem *myItem = new QTableWidgetItem(monthYear);
		QTableWidgetItem *itName = new QTableWidgetItem(itemName);
		QTableWidgetItem *itDesc = new QTableWidgetItem(itemDesc);
		QTableWidgetItem *itAmt = new QTableWidgetItem(amt);

		ui->tblPreview->setItem(r, 0, myItem);
		ui->tblPreview->setItem(r, 1, itName);
		ui->tblPreview->setItem(r, 2, itDesc);
		ui->tblPreview->setItem(r, 3, itAmt);
	}
}



void CreateMultiInvoice::on_cmdSave_clicked()
{
	int rs = ui->tblPreview->rowCount();

	if (rs < 1)
	{
		Publics::showError("No invoices chosen");
		return;
	}


	for (int i = 0; i < rs; i++) {
		QString itemName, monthYear, desc, amount;
		QString total, words, propID, compID;
		QString unique = QUuid::createUuid().toString();
		unique  = unique.left(unique.length() - 1);
		unique = unique.right(unique.length() - 1);
		double d_total = 0;
		monthYear =  ui->tblPreview->item(i, 0)->text() ;
		int month = QStringList(monthYear.split("/")).at(0).toInt();
		int year = QStringList(monthYear.split("/")).at(1).toInt();
		//itemName = ui->tblPreview->item(i, 1)->text() ;
		desc =  ui->tblPreview->item(i, 2)->text() ;
		amount =  ui->tblPreview->item(i, 3)->text() ;
		total = amount;

		QString itemSql = "INSERT INTO invoice_detail (UniqueID, ItemName, Amount) VALUES ('"
				+ unique + "', '"
				+ desc + "', '"
				+ amount + "'"
					     ")";

		QString insertMaster = "INSERT INTO invoice_master (UniqueID, TenantID, "
				"Balance, InvoiceMonthYear,"
				       "UnitID, PropertyID, CompanyID, InvoiceDate, TenantName, TenantAddress, "
				       "InvoiceTotal, AmountInWords) VALUES ('"
				+ unique + "', '"
				+ tenantID + "', '"
				+ total  + "', '"
				+ QDate(year, month, 1).toString("MMM-yy")  + "', '"
				+ unitID + "', '"
				+ propID + "', '"
				+ compID + "', '"
				+ QDate(year, month, 1).toString("dd-MMM-yyyy") + "', '"
				+ tenantName + "', '"
				+ tenantTel + "\n" + tenantEmail + "', '"
				+ total + "', '"
				+ words + "')";

		QSqlQuery qu = QSqlDatabase::database().exec(itemSql);
		if (qu.lastError().isValid()) {
			Publics::showError(qu.lastError().text());
			return;
		}

		qu = QSqlDatabase::database().exec(insertMaster);
		if (qu.lastError().isValid()) {
			Publics::showError(qu.lastError().text());
			return;
		}

		QString updateSql = "UPDATE invoice_master SET InvoiceNo = (InvoiceMonthYear || '-' || EntryID) WHERE UniqueID = '"
				+ unique + "'";
		qu = QSqlDatabase::database().exec(updateSql);
		if (qu.lastError().isValid()) {
			Publics::showError(qu.lastError().text());
			return;
		}
	}

	QMessageBox::information(this, "Saved", "Invoice saved");
	this->accept();


}
