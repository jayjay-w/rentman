#include "receivepaymentdialog.h"
#include "ui_receivepaymentdialog.h"

#include <QtSql>
#include "publics.h"

ReceivePaymentDialog::ReceivePaymentDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::ReceivePaymentDialog)
{
	ui->setupUi(this);

	Publics::loadQueryToCombo("SELECT * FROM tenant", "Name", ui->cboTenant);
	Publics::loadQueryToCombo("SELECT * FROM unit", "UnitNo", ui->cboUnit);
	Publics::loadQueryToCombo("SELECT * FROM company", "CompanyName", ui->cboCompany);

	ui->dtpDate->setDate(QDate::currentDate());
	Publics::setComboBoxText(ui->cboMonth, QDate::currentDate().toString("MMMM"));
	ui->txtYear->setText(QDate::currentDate().toString("yyyy"));
}

ReceivePaymentDialog::~ReceivePaymentDialog()
{
	delete ui;
}

void ReceivePaymentDialog::on_cmdSave_clicked()
{
	QString unitID, tenantID, companyID;
	unitID = Publics::getDbValue("SELECT * FROM unit WHERE UnitNO = '" + ui->cboUnit->currentText() + "'", "UnitID").toString();
	tenantID = Publics::getDbValue("SELECT * FROM tenant WHERE Name = '" + ui->cboTenant->currentText() + "'", "TenantID").toString();
	companyID = Publics::getDbValue("SELECT * FROM company WHERE CompanyName = '" + ui->cboCompany->currentText() + "'", "CompanyID").toString();

	QString sql = "INSERT INTO payments "
			"(UnitID, TenantID, CompanyID, CompanyName, ReceiptNo, DateReceived, UnitName, TennantName, "
			"AmountReceived, PaymentFor, PayMode, Month, MonthNo, Year, ChequeNo) "
			"VALUES ('"
			+ unitID + "', '" +
			tenantID  + "', '" +
			companyID  + "', '" +
			ui->cboCompany->currentText()  + "', '" +
			ui->txtReceiptNo->text()  + "', '" +
			ui->dtpDate->date().toString("dd-MMM-yyyy") + "', '" +
			ui->cboUnit->currentText()  + "', '" +
			ui->cboTenant->currentText()  + "', '" +
			QString::number(ui->spinAmount->value()) + "', '" +
			ui->txtDescription->toPlainText()  + "', '" +
			ui->cboPaymentMode->currentText() + "', '" +
			ui->cboMonth->currentText() + "', '" +
			QString::number(ui->cboMonth->currentIndex()) + "', '" +
			ui->txtYear->text() + "', '" +
			ui->txtChequeNo->text() + "')";


	QSqlQuery qu = QSqlDatabase::database().exec(sql);
	if (qu.lastError().isValid()) {
		Publics::showError(qu.lastError().text());
	} else {
		this->accept();
	}
}
