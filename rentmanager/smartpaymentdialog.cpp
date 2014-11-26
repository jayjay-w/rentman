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

	connect (ui->trvTenants, SIGNAL(itemClicked(QTreeWidgetItem*,int)), SLOT(handleItemClick(QTreeWidgetItem*,int)));
	connect (ui->trvUnits, SIGNAL(itemClicked(QTreeWidgetItem*,int)), SLOT(handleItemClick(QTreeWidgetItem*,int)));
}

SmartPaymentDialog::~SmartPaymentDialog()
{
	delete ui;
}

void SmartPaymentDialog::startNew()
{
	ui->spAmountReceived->setValue(0);
	ui->trvPaymentAllocation->invisibleRootItem()->takeChildren();

	ui->trvTenants->invisibleRootItem()->takeChildren();
	ui->trvUnits->invisibleRootItem()->takeChildren();
	ui->txtNotes->setText("");
	ui->spAmountReceived->setEnabled(true);
	ui->cboPaidVia->setCurrentIndex(0);
	ui->cboPaidVia->setEnabled(true);
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

		it->setText(99, s_unitID);
		it->setText(100, "Unit");
		it->setText(1, companyCode);
		it->setText(2, propertyCode);

		//tenant name, rent
		it->setText(3, "-");
		//it->setText(4, "-");
		QSqlQuery unitQu = QSqlDatabase::database().exec("SELECT * FROM leases WHERE UnitID = '" + it->text(99) + "'");
		while (unitQu.next()) {
			QString tenantID = unitQu.record().value("TenantID").toString();
			QString tenantName = Publics::getDbValue("SELECT * FROM tenant WHERE TenantID = '" + tenantID + "'", "Name").toString();
			it->setText(3, tenantName);
			//it->setText(4, unitQu.record().value("MonthlyRent").toString());
		}
	}

	qu = QSqlDatabase::database().exec("SELECT * FROM tenant");
	while (qu.next()) {
		QString tenantID = qu.record().value("TenantID").toString();
		QString tenantName = qu.record().value("Name").toString();
		QTreeWidgetItem *tenant = new QTreeWidgetItem(ui->trvTenants->invisibleRootItem());
		tenant->setText(0, tenantName);
		tenant->setText(99, tenantID);
		tenant->setText(100, "Tenant");
		QSqlQuery leaseQu = QSqlDatabase::database().exec("SELECT * FROM leases WHERE TenantID = '" + tenantID + "'");
		while (leaseQu.next()) {
			QString lease_unitID = leaseQu.record().value("UnitID").toString();
			QString unitNO, companyID, propertyID, companyName, propertyName;
			unitNO = Publics::getDbValue("SELECT * FROM unit WHERE UnitID = '" + lease_unitID + "'", "UnitNo").toString();
			propertyID = Publics::getDbValue("SELECT * FROM unit WHERE UnitID = '" + lease_unitID + "'", "PropertyID").toString();
			companyID = Publics::getDbValue("SELECT * FROM property WHERE PropertyID = '" + propertyID + "'", "CompanyID").toString();
			propertyName = Publics::getDbValue("SELECT * FROM property WHERE PropertyID = '" + propertyID + "'", "PropertyName").toString();
			companyName = Publics::getDbValue("SELECT * FROM company WHERE CompanyID = '" + propertyID + "'", "Code").toString();

			QTreeWidgetItem *unitItem = new QTreeWidgetItem(tenant);
			unitItem->setText(1, unitNO);
			unitItem->setText(2, companyName);
			unitItem->setText(3, propertyName);
			unitItem->setText(99, lease_unitID);
			unitItem->setText(100, "Unit");
		}
	}

	ui->trvTenants->resizeColumnToContents(-1);
	ui->trvUnits->resizeColumnToContents(-1);
}

void SmartPaymentDialog::startNewWithPayment(QString unitID, QString paymentAmount, QString narration, bool deposit)
{
	ui->spAmountReceived->setValue(0);
	ui->trvPaymentAllocation->invisibleRootItem()->takeChildren();

	ui->trvTenants->invisibleRootItem()->takeChildren();
	ui->trvUnits->invisibleRootItem()->takeChildren();
	ui->spAmountReceived->setEnabled(true);
	ui->cboPaidVia->setCurrentIndex(0);
	ui->cboPaidVia->setEnabled(true);

	ui->tabWidget->setVisible(false);
	ui->spAmountReceived->setValue(paymentAmount.toDouble());
	ui->spAmountReceived->setEnabled(false);

	showInvoices("Unit", unitID);
	ui->txtNotes->setText("");
	if (deposit) {
		Publics::setComboBoxText(ui->cboPaidVia, "Rent Deposit");
		ui->cboPaidVia->setEnabled(false);
	}

	ui->txtNotes->setText(narration);
}

void SmartPaymentDialog::on_cmdAutoAllocate_clicked()
{
	double _orig = ui->spAmountReceived->value();
	double _bal = _orig;
	for (int i = 0; i < ui->trvPaymentAllocation->invisibleRootItem()->childCount(); i++) {
		QTreeWidgetItem *it = ui->trvPaymentAllocation->invisibleRootItem()->child(i);
		double item_balance = it->text(6).toDouble();
		double assigned = 0;
		if (_bal  > 0) {
			if (_bal >= item_balance) {
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
		it->setText(7, QString::number(assigned));
		it->setText(8, QString::number(new_bal));
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
		assigned = assigned + it->text(7).toDouble();
		if (it->text(7).toDouble() < 1) {
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
		double balance_before = item->text(6).toDouble();
		double assigned = item->text(7).toDouble();
		double new_assign = 0;
		if (item->checkState(0) == Qt::Unchecked) {
			item->setText(7, "0");
			item->setText(8, item->text(6));
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

				item->setText(7, QString::number(new_assign));
				item->setText(8, QString::number(balance_before - new_assign) );
			}
		}
		updateTotals();
	}
}

void SmartPaymentDialog::on_trvPaymentAllocation_itemChanged(QTreeWidgetItem *item, int column)
{
	if (column == 7) {
		double balance_before = item->text(6).toDouble();
		double assiged = item->text(7).toDouble();
		if (assiged > balance_before)
			assiged = balance_before;
		double new_balance = balance_before - assiged;
		item->setText(8, QString::number(new_balance));
		item->setText(7, QString::number(assiged));
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

	if (ui->cboPaidVia->currentText() == "Rent Deposit")
		ui->spAmountReceived->setValue(assigned);

	received = assigned;
	balance = 0;
	amt_paid = assigned;

	//All ok
	QString unique = QUuid::createUuid().toString();
	unique  = unique.left(unique.length() - 1);
	unique = unique.right(unique.length() - 1);
	QString unitNo = Publics::getDbValue("SELECT * FROM unit WHERE UnitID = '" + unitID + "'", "UnitNo").toString();
	QString master_sql = "INSERT INTO payments (UniqueID, "
			     "UnitID,  TenantID, CompanyID, CompanyName, ReceiptNo, DateReceived, UnitName, TenantName, "
			     "AmountReceived, PaymentFor, PayMode, ChequeNo, Narration, Allocated, Balance) "
			     "VALUES ('"
			+ unique + "', '"
			+ unitID + "', '"
			+ tenantID + "', '"
			+ companyID + "', '"
			+ companyName + "', '"
			+ ui->txtReceiptNo->text() + "', '"
			+ ui->dtpReceiptDate->date().toString("dd-MMM-yyyy") + "', '"
			+ unitNo + "', '"
			+ tenantName + "', '"
			+ QString::number(received) + "', '"
						      "Rent', '"
			+ ui->cboPaidVia->currentText() + "', '"
			+ ui->txtChequeNo->text() + "', '"
			+ ui->txtNotes->text() + "', '"
			+ QString::number(assigned) + "', '"
			+ QString::number(balance) + "')"
			+ "";

	QSqlDatabase::database().exec(master_sql);

	for (int i = 0; i < ui->trvPaymentAllocation->invisibleRootItem()->childCount(); i++) {
		QTreeWidgetItem *it = ui->trvPaymentAllocation->invisibleRootItem()->child(i);
		if (it->text(7).toDouble() > 0) {
			QString invoiceNo = it->text(0);
			QString allocated = it->text(7);
			QString itemSql = "INSERT INTO payment_allocation (UniqueID, InvoiceID, Amount) VALUES ('"
					+ unique + "', '"
					+ invoiceNo + "', '"
					+ allocated + "')";

			QSqlDatabase::database().exec(itemSql);

		}
	}

	this->accept();
}

void SmartPaymentDialog::on_spAmountReceived_valueChanged(double arg1)
{
	Q_UNUSED(arg1);
	updateTotals();
}

void SmartPaymentDialog::handleItemClick(QTreeWidgetItem *item, int column)
{
	Q_UNUSED(column);
	showInvoices(item->text(100), item->text(99));
}

void SmartPaymentDialog::showInvoices(QString queryBy, QString queryValue)
{
	if (queryBy == "Unit") {
		unitID = queryValue;
		tenantID = Publics::getDbValue("SELECT * FROM leases WHERE UnitID = '" + unitID + "'", "TenantID").toString();
	} else if (queryBy == "Tenant") {
		tenantID = queryValue;
		unitID = Publics::getDbValue("SELECT * FROM leases WHERE TenantID = '" + tenantID + "'", "UnitID").toString();
	}
	ui->trvPaymentAllocation->invisibleRootItem()->takeChildren();
	//QString unitNo = Publics::getDbValue("SELECT * FROM unit WHERE UnitID = '" + unitID + "'", "UnitNo").toString();
	propertyID = Publics::getDbValue("SELECT * FROM unit WHERE UnitID = '" + unitID + "'", "PropertyID").toString();
	companyID = Publics::getDbValue("SELECT * FROM property WHERE PropertyID = '" + propertyID + "'", "CompanyID").toString();
	companyName = Publics::getDbValue("SELECT * FROM company WHERE CompanyID = '" + companyID + "'", "CompanyName").toString();
	//QString leaseID = Publics::getDbValue("SELECT * FROM leases WHERE UnitID = '" + unitID + "'", "EntryID").toString();

	tenantName = Publics::getDbValue("SELECT * FROM tenant WHERE TenantID = '" + tenantID + "'", "Name").toString();
	tenantTel = Publics::getDbValue("SELECT * FROM tenant WHERE TenantID = '" + tenantID + "'", "Tel").toString();
	tenantEmail = Publics::getDbValue("SELECT * FROM tenant WHERE TenantID = '" + tenantID + "'", "Email").toString();

	ui->txtTenantDetails->setPlainText(tenantName + "\n" + tenantTel + "\n" + tenantEmail);

	QSqlQuery qu;
	if (queryBy == "Unit") {
		qu = QSqlDatabase::database().exec("SELECT * FROM invoice_master WHERE UnitID = '" + unitID + "'");
	} else {
		qu = QSqlDatabase::database().exec("SELECT * FROM invoice_master WHERE TenantID = '" + tenantID + "'");
	}
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
			//unit, company, property
			invItem->setText(1, "-");
			invItem->setText(2, "-");
			invItem->setText(3, "-");
			QString lease_unitID = qu.record().value("UnitID").toString();
			QString unitNO, companyID, propertyID, companyName, propertyName;
			unitNO = Publics::getDbValue("SELECT * FROM unit WHERE UnitID = '" + lease_unitID + "'", "UnitNo").toString();
			propertyID = Publics::getDbValue("SELECT * FROM unit WHERE UnitID = '" + lease_unitID + "'", "PropertyID").toString();
			companyID = Publics::getDbValue("SELECT * FROM property WHERE PropertyID = '" + propertyID + "'", "CompanyID").toString();
			propertyName = Publics::getDbValue("SELECT * FROM property WHERE PropertyID = '" + propertyID + "'", "PropertyName").toString();
			companyName = Publics::getDbValue("SELECT * FROM company WHERE CompanyID = '" + propertyID + "'", "Code").toString();
			invItem->setText(1, unitNO);
			invItem->setText(2, companyName);
			invItem->setText(3, propertyName);
			//amt, paid, bal, alloc, newbal
			invItem->setText(4, QString::number(due));
			invItem->setText(5, QString::number(paid));
			invItem->setText(6, QString::number(balance));
			invItem->setText(7, "0");
			invItem->setText(8, QString::number(balance));
		}

	}

	updateTotals();
}
