#include "propertiesdialog.h"
#include "ui_propertiesdialog.h"

#include "publics.h"
#include <QTreeWidgetItem>
#include <QtSql>
#include <QInputDialog>

PropertiesDialog::PropertiesDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::PropertiesDialog)
{
	ui->setupUi(this);

	connect (ui->cmdDeleteProperty, SIGNAL(clicked()), SLOT(deleteProperty()));
	connect (ui->trvProperties, SIGNAL(itemClicked(QTreeWidgetItem*,int)),
		 SLOT(itemChanged(QTreeWidgetItem*,int)));
	connect (ui->cmdNewProperty, SIGNAL(clicked()), SLOT(newProperty()));
	connect (ui->cmdSaveChanges, SIGNAL(clicked()), SLOT(saveChanges()));
}

PropertiesDialog::~PropertiesDialog()
{
	delete ui;
}

void PropertiesDialog::reloadProperties()
{
	Publics::loadQueryToCombo("SELECT * FROM company", "CompanyName", ui->cboCompany);
	ui->trvProperties->invisibleRootItem()->takeChildren();
	QSqlQuery qu = QSqlDatabase::database().exec("SELECT * FROM property");
	QString id;
	while (qu.next()) {
		QTreeWidgetItem *it = new QTreeWidgetItem(ui->trvProperties);
		it->setText(0, qu.record().value("PropertyName").toString());
		it->setText(99, qu.record().value("PropertyID").toString());
		id = it->text(99);
	}

	if (id.length() > 0)
	{
		m_currentProperty = id;
		editProperty();
	}
}

void PropertiesDialog::editProperty()
{
	Publics::clearTextBoxes(this);
	QSqlQuery qu = QSqlDatabase::database().exec("SELECT * FROM property  WHERE PropertyID = '"
						     + m_currentProperty + "'");
	qu.first();

	ui->txtCode->setText(qu.record().value("PropertyCode").toString());
	ui->txtPropertyName->setText(qu.record().value("PropertyName").toString());
	ui->txtLocation->setText(qu.record().value("Location").toString());
	ui->txtLRNo->setText(qu.record().value("LRNumber").toString());
	ui->txtStreet->setText(qu.record().value("Street").toString());
	ui->txtNoOfUnits->setText(qu.record().value("UnitCount").toString());
	QString companyID = qu.record().value("CompanyID").toString();
	QString companyName = Publics::getDbValue("SELECT * FROM company WHERE CompanyID = '" + companyID + "'", "CompanyName").toString();
	Publics::setComboBoxText(ui->cboCompany, companyName);
}

void PropertiesDialog::newProperty()
{
	QString newPropertyName = QInputDialog::getText(this, "New Property", "Enter new property name");
	if (newPropertyName.length() > 0) {
		QSqlQuery qu2 = QSqlDatabase::database().exec("INSERT INTO property (PropertyName) VALUES ('" + newPropertyName + "')");
		m_currentProperty = qu2.lastInsertId().toString();
		reloadProperties();
		editProperty();
	}
}

void PropertiesDialog::saveChanges()
{
	QString companyID = Publics::getDbValue("SELECT * FROM company WHERE CompanyName = '" + ui->cboCompany->currentText() + "'", "CompanyID").toString();
	QString sql = "UPDATE property SET "
			"PropertyCode = '" + ui->txtCode->text() + "', "
			"PropertyName = '" + ui->txtPropertyName->text() + "', "
			"Location = '" + ui->txtLocation->text() + "', "
			"LRNumber = '" + ui->txtLRNo->text() + "', "
			"Street = '" + ui->txtStreet->text() + "', "
			"CompanyID = '" + companyID + "', "
			"UnitCount = '" + ui->txtNoOfUnits->text() + "' WHERE "
			"PropertyID = '" + m_currentProperty + "'";

	QSqlDatabase::database().exec(sql);
	reloadProperties();
}

void PropertiesDialog::deleteProperty()
{
	if (QMessageBox::question(this, "Confirm Delete", "Are you sure you want to delete this property?", QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes) {
		QSqlDatabase::database().exec("DELETE FROM property WHERE PropertyID = '" + m_currentProperty + "'");
		m_currentProperty = "";
		Publics::clearTextBoxes(this);
		reloadProperties();
	}
}

void PropertiesDialog::itemChanged(QTreeWidgetItem *item, int column)
{
	Q_UNUSED(column);
	if (m_currentProperty.length() > 0)
		saveChanges();

	QString c_id = item->text(99);
	m_currentProperty = c_id;
	editProperty();
}
