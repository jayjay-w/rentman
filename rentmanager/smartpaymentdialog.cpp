#include "smartpaymentdialog.h"
#include "ui_smartpaymentdialog.h"

#include "publics.h"
#include <QTreeWidgetItem>
#include <QDebug>
#include <QUuid>

SmartPaymentDialog::SmartPaymentDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::SmartPaymentDialog)
{
	ui->setupUi(this);

	ui->txtReceived->setReadOnly(true);
	ui->txtAllocated->setReadOnly(true);
	ui->txtBalance->setReadOnly(true);

	ui->dtpReceiptDate->setDate(QDate::currentDate());
}

SmartPaymentDialog::~SmartPaymentDialog()
{
	delete ui;
}

void SmartPaymentDialog::startNew()
{
	Publics::loadQueryToCombo("SELECT * FROM unit", "UnitNo", ui->cboUnit);
	ui->spAmountReceived->setValue(0);
	ui->trvPaymentAllocation->invisibleRootItem()->takeChildren();

	on_cboUnit_currentTextChanged(ui->cboUnit->currentText());
}

void SmartPaymentDialog::on_cboUnit_currentTextChanged(const QString &arg1)
{
	ui->trvPaymentAllocation->invisibleRootItem()->takeChildren();
	QString unitNo = arg1;
	unitID = Publics::getDbValue("SELECT * FROM unit WHERE UnitNo = '" + unitNo + "'", "UnitID").toString();
	propertyID = Publics::getDbValue("SELECT * FROM unit WHERE UnitNo = '" + unitNo + "'", "PropertyID").toString();
	companyID = Publics::getDbValue("SELECT * FROM property WHERE PropertyID = '" + propertyID + "'", "CompanyID").toString();
	companyName = Publics::getDbValue("SELECT * FROM company WHERE CompanyID = '" + companyID + "'", "CompanyName").toString();
	//QString leaseID = Publics::getDbValue("SELECT * FROM leases WHERE UnitID = '" + unitID + "'", "EntryID").toString();

	tenantID = Publics::getDbValue("SELECT * FROM leases WHERE UnitID = '" + unitID + "'", "TenantID").toString();
	tenantName = Publics::getDbValue("SELECT * FROM tenant WHERE TenantID = '" + tenantID + "'", "Name").toString();
	tenantTel = Publics::getDbValue("SELECT * FROM tenant WHERE TenantID = '" + tenantID + "'", "Tel").toString();
	tenantEmail = Publics::getDbValue("SELECT * FROM tenant WHERE TenantID = '" + tenantID + "'", "Email").toString();

	ui->txtTenantDetails->setPlainText(tenantName + "\n" + tenantTel + "\n" + tenantEmail);

	QSqlQuery qu = QSqlDatabase::database().exec("SELECT * FROM invoice_master WHERE UnitID = '" + unitID + "'");
	while (qu.next()) {
		double paid = 0;
		double due = 0;
		double balance = 0;
		QString invoiceID = qu.record().value("InvoiceNo").toString();
		QSqlQuery allocQu = QSqlDatabase::database().exec("SELECT * FROM payment_allocation WHERE InvoiceID = '" + invoiceID + "'");
		while (allocQu.next()) {
			paid = paid + allocQu.record().value("Amount").toDouble();
		}
		due = qu.record().value("InvoiceTotal").toDouble();
		balance = due - paid;

		if (balance > 0) {
			QTreeWidgetItem *invItem = new QTreeWidgetItem(ui->trvPaymentAllocation->invisibleRootItem());
			invItem->setCheckState(0, Qt::Unchecked);
			invItem->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled|Qt::ItemIsUserCheckable);
			invItem->setText(0, qu.record().value("InvoiceNo").toString());
			//amt, paid, bal, alloc, newbal
			invItem->setText(1, QString::number(due));
			invItem->setText(2, QString::number(paid));
			invItem->setText(3, QString::number(balance));
			invItem->setText(4, "0");
			invItem->setText(5, QString::number(balance));
		}

	}

	updateTotals();
}

void SmartPaymentDialog::on_cmdAutoAllocate_clicked()
{
	double _orig = ui->spAmountReceived->value();
	double _bal = _orig;
	for (int i = 0; i < ui->trvPaymentAllocation->invisibleRootItem()->childCount(); i++) {
		QTreeWidgetItem *it = ui->trvPaymentAllocation->invisibleRootItem()->child(i);
		double item_balance = it->text(3).toDouble();
		double assigned = 0;
		if (_bal  > 0) {
			if (_bal > item_balance) {
				assigned = item_balance;
			}
			if (_bal < item_balance) {
				assigned = _bal;
			}

			if (_bal < 1) {
				assigned = 0;
				_bal = 0;
			}
		}


		_bal = _bal - assigned;
		double new_bal = item_balance - assigned;
		it->setText(4, QString::number(assigned));
		it->setText(5, QString::number(new_bal));
		it->setCheckState(0, Qt::Unchecked);

		if (assigned > 0)
			it->setCheckState(0, Qt::Checked);
	}

	updateTotals();
}

void SmartPaymentDialog::updateTotals()
{
	double received = ui->spAmountReceived->value();
	double assigned = 0;
	double balance = 0;
	for (int i = 0; i < ui->trvPaymentAllocation->invisibleRootItem()->childCount(); i++) {
		QTreeWidgetItem *it = ui->trvPaymentAllocation->invisibleRootItem()->child(i);
		assigned = assigned + it->text(4).toDouble();
		if (it->text(4).toDouble() < 1) {
			it->setCheckState(0, Qt::Unchecked);
		}
	}
	balance = received - assigned;
	ui->txtReceived->setText(QString::number(received));
	ui->txtAllocated->setText(QString::number(assigned));
	ui->txtBalance->setText(QString::number(balance));
}

void SmartPaymentDialog::updateChecks()
{

	updateTotals();
}

void SmartPaymentDialog::on_trvPaymentAllocation_itemClicked(QTreeWidgetItem *item, int column)
{
	if (column == 0) {
		double receipt_balance = ui->txtBalance->text().toDouble();
		double balance_before = item->text(3).toDouble();
		double assigned = item->text(4).toDouble();
		double new_assign = 0;
		if (item->checkState(0) == Qt::Unchecked) {
			item->setText(4, "0");
			item->setText(5, item->text(3));
		} else {
			if (assigned < 1) {
				if (receipt_balance < balance_before) {
					new_assign = receipt_balance;
				}
				if (receipt_balance >= balance_before) {
					new_assign = balance_before;
				}
				if (receipt_balance < 1) {
					new_assign = 0;
				}

				item->setText(4, QString::number(new_assign));
				item->setText(5, QString::number(balance_before - new_assign) );
			}
		}
		updateTotals();
	}
}

void SmartPaymentDialog::on_trvPaymentAllocation_itemChanged(QTreeWidgetItem *item, int column)
{
	if (column == 4) {
		double balance_before = item->text(3).toDouble();
		double assiged = item->text(4).toDouble();
		if (assiged > balance_before)
			assiged = balance_before;
		double new_balance = balance_before - assiged;
		item->setText(5, QString::number(new_balance));
		item->setText(4, QString::number(assiged));
		if (assiged > 0)
			item->setCheckState(0, Qt::Checked);
		updateTotals();
	}
}

void SmartPaymentDialog::on_cmdSave_clicked()
{
	double received = ui->spAmountReceived->value();
	double assigned = ui->txtAllocated->text().toDouble();
	double balance = ui->txtBalance->text().toDouble();

	if (balance < 0) {
		//over allocation
		Publics::showError("You have allocated more funds than received. Please un-select some invoices.");
		return;
	}

	//All ok
	QString unique = QUuid::createUuid().toString();
	unique  = unique.left(unique.length() - 1);
	unique = unique.right(unique.length() - 1);
	QString master_sql = "INSERT INTO payments (UniqueID, "
			     "UnitID,  TenantID, CompanyID, CompanyName, ReceiptNo, DateReceived, UnitName, TenantName, "
			     "AmountReceived, PaymentFor, PayMode, ChequeNo, Allocated, Balance) "
			     "VALUES ('"
			+ unique + "', '"
			+ unitID + "', '"
			+ tenantID + "', '"
			+ companyID + "', '"
			+ companyName + "', '"
			+ ui->txtReceiptNo->text() + "', '"
			+ ui->dtpReceiptDate->date().toString("dd-MMM-yyyy") + "', '"
			+ ui->cboUnit->currentText() + "', '"
			+ tenantName + "', '"
			+ QString::number(received) + "', '"
			"Rent', '"
			+ ui->cboPaidVia->currentText() + "', '"
			+ ui->txtChequeNo->text() + "', '"
			+ QString::number(assigned) + "', '"
			+ QString::number(balance) + "')"
			+ "";

	QSqlDatabase::database().exec(master_sql);

	for (int i = 0; i < ui->trvPaymentAllocation->invisibleRootItem()->childCount(); i++) {
		QTreeWidgetItem *it = ui->trvPaymentAllocation->invisibleRootItem()->child(i);
		if (it->text(4).toDouble() > 0) {
			QString invoiceNo = it->text(0);
			QString allocated = it->text(4);
			QString itemSql = "INSERT INTO payment_allocation (UniqueID, InvoiceID, Amount) VALUES ('"
					+ unique + "', '"
					+ invoiceNo + "', '"
					+ allocated + "')";

			QSqlDatabase::database().exec(itemSql);

		}
	}

	this->accept();
}
