#include "companiesdialog.h"
#include "ui_companiesdialog.h"
#include <QtSql>
#include <QInputDialog>
#include <QTreeWidgetItem>
#include "publics.h"

#include "rentmanagermainwindow.h"
#include "propertiesdialog.h"

CompaniesDialog::CompaniesDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::CompaniesDialog)
{
	ui->setupUi(this);
}

CompaniesDialog::~CompaniesDialog()
{
	delete ui;
}

void CompaniesDialog::reloadCompanies()
{
	ui->trvCompanies->invisibleRootItem()->takeChildren();
	QSqlQuery qu = QSqlDatabase::database().exec("SELECT * FROM company");
	QString id;
	while (qu.next()) {
		QTreeWidgetItem *it = new QTreeWidgetItem(ui->trvCompanies);
		it->setText(0, qu.record().value("CompanyName").toString());
		it->setText(99, qu.record().value("CompanyID").toString());
		id = it->text(99);
	}

	if (id.length() > 0)
	{
		m_currentCompany = id;
		editCompany();
	}
}

void CompaniesDialog::editCompany()
{
	Publics::clearTextBoxes(this);
	QSqlQuery qu = QSqlDatabase::database().exec("SELECT * FROM company WHERE CompanyID = '" + m_currentCompany + "'");
	qu.first();
	ui->txtCode->setText(qu.record().value("Code").toString());
	ui->txtCompanyName->setText(qu.record().value("CompanyName").toString());
	ui->txtPhysicalAddress->setText(qu.record().value("PhysicalAddress").toString());
	ui->txtPostalAddress->setText(qu.record().value("PostalAddress").toString());
	ui->txtContactPerson->setText(qu.record().value("ContactPerson").toString());
	ui->txtMobileNo->setText(qu.record().value("Mobile").toString());
	ui->txtEmail->setText(qu.record().value("Email").toString());
	ui->txtTelNo->setText(qu.record().value("Tel1").toString());
	ui->trvProperties->invisibleRootItem()->takeChildren();
	qu = QSqlDatabase::database().exec("SELECT * FROM property WHERE CompanyID = '" + m_currentCompany + "'");
	while (qu.next()) {
		QTreeWidgetItem *it = new QTreeWidgetItem(ui->trvProperties);
		it->setText(99, qu.record().value("PropertyID").toString());
		it->setText(0, qu.record().value("PropertyName").toString());
		it->setText(1, qu.record().value("PropertyCode").toString());
		it->setText(2, qu.record().value("Location").toString());
		it->setText(3, qu.record().value("UnitCount").toString());
	}
}

void CompaniesDialog::on_cmdNewCompany_clicked()
{
	QString newCompanyName = QInputDialog::getText(this, "New Company", "Enter new company name");
	if (newCompanyName.length() > 0) {
		QSqlQuery qu2 = QSqlDatabase::database().exec("INSERT INTO company (CompanyName) VALUES ('" + newCompanyName + "')");
		m_currentCompany = qu2.lastInsertId().toString();
		reloadCompanies();
		editCompany();
	}
}

void CompaniesDialog::on_trvCompanies_itemClicked(QTreeWidgetItem *item, int column)
{
	Q_UNUSED(column);
	if (m_currentCompany.length() > 0)
		on_cmdSaveChanges_clicked();

	QString c_id = item->text(99);
	m_currentCompany = c_id;
	editCompany();
}

void CompaniesDialog::on_cmdSaveChanges_clicked()
{
	QString sql = "UPDATE company SET "
		      "Code = '" + ui->txtCode->text() + "', "
							 "PhysicalAddress = '" + ui->txtPhysicalAddress->text() + "', "
														  "PostalAddress = '" + ui->txtPostalAddress->text() + "', "
																				       "ContactPerson = '" + ui->txtContactPerson->text() + "', "
																											    "Mobile = '" + ui->txtMobileNo->text() + "', "
																																     "Email = '" + ui->txtEmail->text() + "', "
																																					  "CompanyName = '" + ui->txtCompanyName->text() + "', "
																																											   "Tel1 = '" + ui->txtTelNo->text() + "' WHERE "
																																															       "CompanyID = '" + m_currentCompany + "'";

	QSqlDatabase::database().exec(sql);
	reloadCompanies();
}

void CompaniesDialog::on_cmdDeleteCompany_clicked()
{
	if (QMessageBox::question(this, "Confirm Delete", "Are you sure you want to delete this company?", QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes) {
		QSqlDatabase::database().exec("DELETE FROM company WHERE CompanyID = '" + m_currentCompany + "'");
		m_currentCompany = "";
		Publics::clearTextBoxes(this);
		reloadCompanies();
	}
}

void CompaniesDialog::on_trvProperties_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
	PropertiesDialog *prg = new PropertiesDialog(this);
	prg->publicEdit(item->text(99));
	prg->exec();
	editCompany();
}
