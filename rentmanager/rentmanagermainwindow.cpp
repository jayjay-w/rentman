#include "rentmanagermainwindow.h"
#include "ui_rentmanagermainwindow.h"

#include "publics.h"
#include <QActionGroup>
#include "companyinitializationdialog.h"
#include "companiesdialog.h"
#include "propertiesdialog.h"
#include "tenantsdialog.h"
#include "assignunittotenantdialog.h"
#include "receivepaymentdialog.h"
#include "paymentsdialog.h"
#include <QTableWidgetItem>

#include "createinvoicedialog.h"
#include "createmultiinvoice.h"
#include "smartpaymentdialog.h"

#include <QPrinter>
#include <QPrintDialog>
#include <QInputDialog>
#include <QPageSetupDialog>
#include <QPrintPreviewDialog>
#include "companyfilepassword.h"
#include "simplecrypt.h"
#include "depositdialog.h"
#include "customeraccountdialog.h"
#include "terminateleasedialog.h"

#include "xlslib.h"

using namespace xlslib_core;
using namespace std;

RentManagerMainWindow *RentManagerMainWindow::m_instance = NULL;

RentManagerMainWindow::RentManagerMainWindow(QWidget *parent) :
	QMainWindow(),
	actionsToDisable(0),
	m_companiesDialog(0),
	m_propertiesDialog(0),
	m_tenants(0),
	m_assign(0),
	m_payDiag(0),
	m_singleInvoice(0),
	m_multiInvoice(0),
	m_smartPayment(0),
	m_deposits(0),
	m_custAccount(0),
	m_terminateLease(0),
	ui(new Ui::RentManagerMainWindow)
{
	Q_ASSERT_X(m_instance == NULL, "MainWindow", "MainWindow recreated!");
	m_instance = this;
	Q_UNUSED(parent);
	ui->setupUi(this);

	//ActionGroup
	if (!actionsToDisable)
		actionsToDisable = new QActionGroup(this);




	actionsToDisable->addAction(ui->actionAssign_Unit_To_Tenant);
	actionsToDisable->addAction(ui->actionCompanies);
	actionsToDisable->addAction(ui->actionCreate_Invoice);
	actionsToDisable->addAction(ui->actionMulti_Invoicing);
	actionsToDisable->addAction(ui->actionProperties);
	actionsToDisable->addAction(ui->actionReceive_Payments);
	actionsToDisable->addAction(ui->actionTenants);
	actionsToDisable->addAction(ui->actionUnites);
	actionsToDisable->addAction(ui->actionView_Tenant_Accounts);
	actionsToDisable->addAction(ui->action_Save);
	actionsToDisable->addAction(ui->actionSave_As);
	actionsToDisable->addAction(ui->actionReceive_Payments);
	actionsToDisable->addAction(ui->actionView_Payments);
	actionsToDisable->addAction(ui->actionContact_List);
	actionsToDisable->addAction(ui->actionChange_Password);
	actionsToDisable->addAction(ui->actionDeposit_List);
	actionsToDisable->addAction(ui->actionTerminate_Lease);

	closeFile();
	ui->actionAbout_Qt->setVisible(false);
	actionsToDisable->setDisabled(true);
	ui->menuRecent_Files->clear();
	for (int i = 0; i < MaxRecentFiles; ++i) {
		recentFileActs[i] = new QAction(this);
		recentFileActs[i]->setVisible(false);
		connect (recentFileActs[i], SIGNAL(triggered()), SLOT(openRecentFile()));
		ui->menuRecent_Files->addAction(recentFileActs[i]);
	}
	updateRecentFileActions();
	//connects
	connect (ui->action_Open, SIGNAL(triggered()), SLOT(startOpenFile()));
	connect (ui->action_New, SIGNAL(triggered()), SLOT(startNewFile()));
	connect (ui->actionAbout_Qt, SIGNAL(triggered()), SLOT(aboutQt()));
	connect (ui->actionCreate_Invoice, SIGNAL(triggered()), SLOT(singleInvoice()));
	connect (ui->actionFit_Page, SIGNAL(triggered()), SLOT(fitPage()));
	connect (ui->actionFit_Width, SIGNAL(triggered()), SLOT(fitWidth()));
	//ui restore
	QSettings sett(qApp->organizationName(), qApp->applicationName());
	qDebug() << sett.value("MainWindowSplitter").toString();
	ui->splitter->restoreState(sett.value("MainWindowSplitter",
					      ui->splitter->saveState()).toByteArray());

	printer = new QPrinter(QPrinter::HighResolution);

	QHBoxLayout *tab2Layout = new QHBoxLayout(this);
	tab2Layout->addWidget(ui->lstReports);

	QVBoxLayout *printLayout = new QVBoxLayout(this);

	QToolBar *tblQuery = new QToolBar("Report Query Toolbar", this);
	printLayout->addWidget(ui->printToolBar);
	printLayout->addWidget(tblQuery);
	printLayout->addWidget(ui->reportSplitter);
	tab2Layout->addLayout(printLayout);

	ui->tab_2->setLayout(tab2Layout);

	rpt_dtpReportQuery = new QDateEdit(this);
	rpt_dtpReportQuery->setCalendarPopup(true);
	rpt_dtpReportQuery->setDisplayFormat("dd-MMM-yyyy");
	rpt_dtpReportQuery->setDate(QDate::currentDate());
	rpt_refresh = new QPushButton("Refresh", this);
	QLabel *lbl = new QLabel(this);
	lbl->setText("Select Date");
	tblQuery->addWidget(lbl);
	tblQuery->addWidget(rpt_dtpReportQuery);
	tblQuery->addSeparator();

	rpt_cboCompany = new QComboBox(this);
	rpt_cboProperty = new QComboBox(this);
	rpt_cboTenant = new QComboBox(this);
	tblQuery->addWidget(new QLabel("Company:", this));
	tblQuery->addWidget(rpt_cboCompany);
	tblQuery->addWidget(new QLabel("Property:", this));
	tblQuery->addWidget(rpt_cboProperty);
	tblQuery->addWidget(new QLabel("Tenant:", this));
	tblQuery->addWidget(rpt_cboTenant);
	tblQuery->addSeparator();
	tblQuery->addWidget(rpt_refresh);
	connect (rpt_refresh, SIGNAL(clicked()), SLOT(showReportPreview()));
	connect (rpt_cboCompany, SIGNAL(currentIndexChanged(QString)), SLOT(reportCompanyChanged()));
}

RentManagerMainWindow::~RentManagerMainWindow()
{
	//ui save state
	QSettings sett(qApp->organizationName(), qApp->applicationName());
	sett.setValue("MainWindowSplitter", ui->splitter->saveState());
	delete ui;
}

RentManagerMainWindow *RentManagerMainWindow::instance()
{
	return m_instance;
}

void RentManagerMainWindow::showQueryError(QSqlQuery qu, QString title, QString textBefore, QString textAfter)
{
	QMessageBox::critical(this, title, QString("%1<br/><b>%2</b><br/>%3").arg(textBefore, qu.lastError().text(), textAfter));
}

void RentManagerMainWindow::startOpenFile()
{
	QString fileName = Publics::getOpenFile(this);
	if (fileName.length() > 0) {
		loadFile(fileName);
	}
}

void RentManagerMainWindow::closeFile()
{
	actionsToDisable->setEnabled(false);

	ui->trvBrowser->invisibleRootItem()->takeChildren();
    ui->calendarTextView->setText("");
	ui->tabWidget->setCurrentIndex(0);
	ui->tabWidget->setEnabled(false);

	curFile = "";
	if (db.isOpen())
		db.close();
}

void RentManagerMainWindow::reloadBrowser()
{
	ui->trvBrowser->invisibleRootItem()->takeChildren();
	QSqlQuery qu = db.exec("SELECT * FROM company");
	while (qu.next()) {
		QString companyName = qu.record().value("CompanyName").toString();
		QString companyID = qu.record().value("CompanyID").toString();

		QTreeWidgetItem *companyItem = new QTreeWidgetItem(ui->trvBrowser->invisibleRootItem());
		companyItem->setText(0, companyName);
		companyItem->setText(99, companyID);
		companyItem->setText(98, "company");
		QSqlQuery propQu = db.exec("SELECT * FROM property WHERE CompanyID = '" + companyID + "'");
		while (propQu.next()) {
			QTreeWidgetItem *propIt = new QTreeWidgetItem(companyItem);
			propIt->setText(0, propQu.record().value("PropertyName").toString());
			propIt->setText(99, propQu.record().value("PropertyID").toString());
			propIt->setText(98, "property");
		}
	}

	QString currentCompany = rpt_cboCompany->currentText();
	QString currentProperty = rpt_cboProperty->currentText();
	QString currentTenane = rpt_cboTenant->currentText();

	Publics::loadQueryToCombo("SELECT * FROM company", "Code", rpt_cboCompany);
	Publics::loadQueryToCombo("SELECT * FROM property", "PropertyCode", rpt_cboProperty);
	Publics::loadQueryToCombo("SELECT * FROM tenant", "Name", rpt_cboTenant);

	rpt_cboCompany->insertItem(0, "--ALL--");
	rpt_cboProperty->insertItem(0, "--ALL--");
	rpt_cboTenant->insertItem(0, "--ALL--");

	rpt_cboCompany->setCurrentIndex(0);
	rpt_cboProperty->setCurrentIndex(0);
	rpt_cboTenant->setCurrentIndex(0);

	if (currentCompany.length() > 0)
		Publics::setComboBoxText(rpt_cboCompany, currentCompany);

	if (currentProperty.length() > 0)
		Publics::setComboBoxText(rpt_cboProperty, currentCompany);

	if (currentTenane.length() > 0)
		Publics::setComboBoxText(rpt_cboTenant, currentCompany);

	if (m_currentCompany.length() > 0)
		reportCompanyChanged();
}

void RentManagerMainWindow::loadFile(const QString &fileName)
{
	if (fileName.isNull()) {
		startOpenFile();
		return;
	}

	closeFile();
	db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName(fileName);
	if (!db.open()) {
		QMessageBox::critical(this, tr("Error"), tr("Database Error.\n%1").arg(db.lastError().text()));
		return;
	}

	//Database open
	initializeCompanyFile();

	QSqlQuery pasQu = db.exec("SELECT * FROM password");
	QString pass;
	while (pasQu.next()) {
		pass = pasQu.record().value("Password").toString();
	}

	if (pass.length() > 1) {
		//This file has a valid password
		QString userPass = QInputDialog::getText(this, "User Login", "Enter your password to open this file:", QLineEdit::Password);
		SimpleCrypt crypt(Q_UINT64_C(0x0c2ad4a4acb9f023));
		pass = crypt.decryptToString(pass);
		if (userPass != pass) {
			Publics::showError("The password you entered is invalid. Please try again.");
			closeFile();
			return;
		}
	}

	curFile = fileName;
	ui->statusBar->showMessage(QString("Opened file: %1").arg(fileName));
	//refresh browser

#if QT_VERSION > 0x50000
	QSettings sett(qApp->organizationName(), qApp->applicationDisplayName());
#endif
#if QT_VERSION < 0x50000
	QSettings sett(qApp->organizationName(), qApp->applicationName());
#endif
	QStringList files = sett.value("recentFiles").toStringList();
	files.removeAll(fileName);
	files.prepend(fileName);

	while (files.size() > MaxRecentFiles)
		files.removeLast();

	sett.setValue("recentFiles", files);
	updateRecentFileActions();
	//Enable employee related widgets and actions
	actionsToDisable->setEnabled(true);
	ui->tabWidget->setEnabled(true);
	reloadBrowser();
}

bool RentManagerMainWindow::saveFile(const QString &fileName)
{
	Q_UNUSED(fileName);
	return true;
}

void RentManagerMainWindow::setCurrentFile(const QString &fileName)
{
	Q_UNUSED(fileName);
}

QString RentManagerMainWindow::strippedName(const QString &fullFileName)
{
	return QFileInfo(fullFileName).fileName();
}

void RentManagerMainWindow::updateRecentFileActions()
{
#if QT_VERSION > 0x50000
	QSettings sett(qApp->organizationName(), qApp->applicationDisplayName());
#endif
#if QT_VERSION < 0x50000
	QSettings sett(qApp->organizationName(), qApp->applicationName());
#endif
	QStringList files = sett.value("recentFiles").toStringList();

	int numRecentFiles = qMin(files.size(), (int)MaxRecentFiles);

	for (int i = 0; i < numRecentFiles; ++i) {
		QString fNum = QString::number(i + 1);

		if (i < 9)
			fNum.prepend(" ");

		QString text = tr("%1 %2").arg(fNum).arg(strippedName(files[i]));
		recentFileActs[i]->setText(text);
		recentFileActs[i]->setStatusTip(files[i]);
		recentFileActs[i]->setData(files[i]);
		recentFileActs[i]->setVisible(true);
	}
	for (int j = numRecentFiles; j < MaxRecentFiles; ++j)
		recentFileActs[j]->setVisible(false);

}

bool RentManagerMainWindow::initializeCompanyFile()
{
	CompanyInitializationDialog *init = new CompanyInitializationDialog(this);
	if (init->exec() == QDialog::Accepted)
		return true;

	return false;
}

void RentManagerMainWindow::openRecentFile()
{
	QAction *action = qobject_cast<QAction *>(sender());
	if (action)
		loadFile(action->data().toString());
}

void RentManagerMainWindow::startNewFile()
{
	QString fileName = Publics::getSaveFile(this);
	if (fileName.length() > 0) {
		QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
		db.setDatabaseName(fileName);
		if (!db.open()) {
			QMessageBox::critical(this, "Error", QString("The following error occurred while creating your company file:\n%1.\nPlease contact your administrator.")
					      .arg(db.lastError().text()));
			return;
		}
		QString password  = "";
		if (QMessageBox::question(this, "Confirm", "Would you like to add a password to this file?\nIf you do not add a password now, you can add it later.", QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes) {
			CompanyFilePassword *pass = new CompanyFilePassword(this, true);
			if (pass->exec() == QDialog::Accepted)
				password = pass->newPassword;
		}
		QSqlDatabase::database().exec("CREATE TABLE IF NOT EXISTS 'password' ('Password' TEXT)");
		QSqlDatabase::database().exec("DELETE FROM password");
		QSqlDatabase::database().exec("INSERT INTO password ('Password') VALUES ('" + password + "')");
		if (initializeCompanyFile()) {
			closeFile();
			loadFile(fileName);
		} else {
			QMessageBox::critical(this, "Error", "An error occurred during the file initialization. Please try again.");
			return;
		}
	} else {
		QMessageBox::critical(this, "Error", "Please specify a valid file.");
		return;
	}
}

void RentManagerMainWindow::on_actionCompanies_triggered()
{
	if (!m_companiesDialog)
		m_companiesDialog = new CompaniesDialog(this);

	m_companiesDialog->reloadCompanies();
	m_companiesDialog->exec();
	reloadBrowser();
}

void RentManagerMainWindow::on_actionProperties_triggered()
{
	if (!m_propertiesDialog)
		m_propertiesDialog = new PropertiesDialog(this);

	m_propertiesDialog->reloadProperties();
	m_propertiesDialog->exec();
	reloadBrowser();
}

void RentManagerMainWindow::on_actionTenants_triggered()
{
	if (!m_tenants)
		m_tenants = new TenantsDialog(this);

	m_tenants->reloadTenants();
	m_tenants->exec();
}

void RentManagerMainWindow::on_actionAssign_Unit_To_Tenant_triggered()
{
	if (!m_assign)
		m_assign = new AssignUnitToTenantDialog(this);

	m_assign->reloadLists();
	m_assign->exec();
}

void RentManagerMainWindow::on_actionReceive_Payments_triggered()
{
	if (!m_smartPayment)
		m_smartPayment = new SmartPaymentDialog(this);

	m_smartPayment->startNew();
	if (m_smartPayment->exec() == QDialog::Accepted)
	{
		reloadCalendar();
	}
}

void RentManagerMainWindow::on_trvBrowser_itemClicked(QTreeWidgetItem *item, int column)
{
	Q_UNUSED(column);
	QString html = "";
	if (item->text(98) == "property") {
		QString propertyID = item->text(99);
		currentProperty = propertyID;
		reloadCalendar();
	} else {
		html = "<h3>Select a property to see its' payment schedule</h3>";
        ui->calendarTextView->setText(html);
	}

}


void RentManagerMainWindow::on_actionView_Payments_triggered()
{
	if (!m_payDiag)
		m_payDiag = new PaymentsDialog(this);

	m_payDiag->reloadPayments();
	m_payDiag->exec();
}

void RentManagerMainWindow::on_actionAll_Units_triggered()
{

}

void RentManagerMainWindow::aboutQt()
{
	QMessageBox::aboutQt(this, "About QT");
}

void RentManagerMainWindow::singleInvoice()
{
	if (!m_singleInvoice)
		m_singleInvoice = new CreateInvoiceDialog(this);

	m_singleInvoice->startNew();
	m_singleInvoice->exec();
}




void RentManagerMainWindow::on_actionMulti_Invoicing_triggered()
{
	if (!m_multiInvoice)
		m_multiInvoice = new CreateMultiInvoice(this);

	m_multiInvoice->startNew();
	if (m_multiInvoice->exec() == QDialog::Accepted)
		reloadCalendar();
}

void RentManagerMainWindow::reloadCalendar()
{
	QString html = "";
	QFile fl(":/reports/reports/html_pretext");
	if (!fl.open(QIODevice::ReadOnly)) {
		Publics::showError("Could not open report source.\n" + fl.errorString());
		return;
	}
	//File open
	html += fl.readAll();
	QSqlQuery unitQu = db.exec("SELECT * FROM unit WHERE PropertyID = '" + currentProperty + "'");
	QString propertyName = Publics::getDbValue("SELECT * FROM property WHERE PropertyID = '" + currentProperty + "'", "PropertyName").toString();
	QString companyId = Publics::getDbValue("SELECT * FROM property WHERE PropertyID = '" + currentProperty + "'", "CompanyID").toString();
	QString companyName = Publics::getDbValue("SELECT * FROM company WHERE CompanyID = '" + companyId + "'", "CompanyName").toString();
	QString physical = Publics::getDbValue("SELECT * FROM company WHERE CompanyID = '" + companyId + "'", "PhysicalAddress").toString();
	QString postal = Publics::getDbValue("SELECT * FROM company WHERE CompanyID = '" + companyId + "'", "PostalAddress").toString();
	QString headerTable;


	headerTable += "<table class='reference' style='width:100%' align='center'>";
	headerTable += QString("<tr style='font-face:verdana'><td align=center><b>%1</b><br/><b>%2</b><br/>%3<br/>%4</td></tr>")
			.arg(companyName, propertyName, physical, postal);
	headerTable += "</table>";
	headerTable += "<table class='reference' border=0 style='width:100%' align='center'>";
	headerTable += QString("<tr style='font-face:verdana'><td>Payment Calendar</td><td align=right>%1</td></tr>").arg(QDate::currentDate().toString("dd-MMM-yyyy"));
	headerTable += "</table>";

	html += headerTable;
	QStringList months;
	months << "" << "Jan"
	       << "Feb"
	       << "Mar"
	       << "Apr"
	       << "May"
	       << "Jun"
	       << "Jul"
	       << "Aug"
	       << "Sep"
	       << "Oct"
	       << "Nov"
	       << "Dec";

	html += "<body><table class='reference' style='width:100%' align='center'>\n";
	html += "<tr>";

	for (int i = 0; i < months.count(); i++)
	{
		html += QString("<th><font face='verdana'>%1</font></th>").arg(months.at(i));
	}
	html += "</tr>";

	while (unitQu.next()) {
		QString unitNo = unitQu.record().value("UnitNo").toString();
		QString unitID = unitQu.record().value("UnitID").toString();
		html += "<tr>"; //start or unit row
		//first col is the unit name
		QSqlQuery leaseQu = db.exec("SELECT * FROM leases WHERE UnitID = '" + unitID + "' AND DateOfOccupation LIKE '%" + ui->cboYear->currentText() + "%'");
		QString names = "";
		while (leaseQu.next()) {
			QString tenantID = leaseQu.record().value("TenantID").toString();
			QString userName = Publics::getDbValue("SELECT * FROM tenant WHERE TenantID = '" + tenantID + "'", "Name").toString();
			names += userName;
		}
		html += "<th>" + unitNo + "<br/>" + names + "</th>";
		for (int i = 1; i < 13; i++) {
			QDate dat(ui->cboYear->currentText().toInt(), i, 1);
			QString monthYear = dat.toString("MMM-yy");
			QSqlQuery invQu = QSqlDatabase::database().exec("SELECT * FROM invoice_master "
									" WHERE InvoiceMonthYear = '" + monthYear + "' AND UnitID = '" + unitID + "'");

			//QString invoiceNos;
			double paid = 0;
			double due = 0;
			double balance = 0;

			QString colText, bgColor = "LightGreen";
			while (invQu.next()) {
				QString invoiceID = invQu.record().value("InvoiceNo").toString();
				QSqlQuery allocQu = QSqlDatabase::database().exec("SELECT * FROM payment_allocation WHERE InvoiceID = '" + invoiceID + "'");
				while (allocQu.next()) {
					paid = paid + allocQu.record().value("Amount").toDouble();
				}
				due = due + invQu.record().value("InvoiceTotal").toDouble();
				balance = due - paid;
			}
			if (balance > 0) {
				bgColor = "Salmon";
			}

			if (balance == due) {
				bgColor = "Red";
			}

			if (balance < 1) {
				bgColor = "LightGreen";
			}

			if (due == 0) {
				bgColor = "LightBlue";
			}

			if (balance )
				colText = "";


			colText += "<b>Due:</b> " + QString::number(due);
			colText += "<br/><b>Pd:</b> " + QString::number(paid);
			colText += "<br/><b>Bal:</b> " + QString::number(balance);


			html += "<td bgcolor=" + bgColor + " style='font-size:11px'><font face='verdana'>";
			html += colText;
			html += "</font></td>";
		}
		html += "</tr>"; //end of unit row
	}

	html += "</table>";

    ui->calendarTextView->setText(html);
}

void RentManagerMainWindow::on_cboYear_currentIndexChanged(int index)
{
	Q_UNUSED(index);
	reloadCalendar();
}

void RentManagerMainWindow::on_actionContact_List_triggered()
{


}

void RentManagerMainWindow::on_actionAbout_triggered()
{
	QMessageBox::about(this, "About RentManager", "<h3>Rent Manager</h3><br/><b>(c) 2014 Joseph W Joshua</b><br/>joejoshw@gmail.com");
}

void RentManagerMainWindow::on_actionQuit_triggered()
{
	closeFile();
	qApp->quit();
}

void RentManagerMainWindow::on_lstReports_itemClicked(QListWidgetItem *item)
{
	rptName = item->text();
	showReportPreview();
}

void RentManagerMainWindow::showReportPreview()
{
	m_paramReplacements.clear();
	m_paramsToReplace.clear();
	QString reportName = rptName;
	if (reportName == "Contact List") {
		reportName = "contact_list";
		QSqlDatabase::database().exec("DELETE FROM report_contact_list");
		QSqlQuery tenantQu = QSqlDatabase::database().exec("SELECT * FROM tenant");
		while (tenantQu.next()) {
			QString tenantName, tenantTel, tenantEmail;
			tenantName = tenantQu.record().value("Name").toString();
			tenantTel = tenantQu.record().value("Tel").toString();
			tenantEmail = tenantQu.record().value("Email").toString();
			QString tenantID = tenantQu.record().value("TenantID").toString();

			QSqlQuery leasesQu = QSqlDatabase::database().exec("SELECT * FROM leases WHERE IsOngoing = 'Yes' AND TenantID = '" +
									   tenantQu.record().value("TenantID").toString() + "'"
									   );
			int leaseCount = 0;

			while (leasesQu.next()) {
				bool toAdd = true;
				leaseCount++;
				QString unitID = leasesQu.record().value("UnitID").toString();
				QString rent = leasesQu.record().value("MonthlyRent").toString();
				QString balance = "0";
				QString unitNo, propertyID, companyID, propertyCode, companyCode;
				unitNo = Publics::getDbValue("SELECT * FROM unit WHERE UnitID = '" + unitID + "'", "UnitNo").toString();
				propertyID = Publics::getDbValue("SELECT * FROM unit WHERE UnitID = '" + unitID + "'", "PropertyID").toString();
				companyID = Publics::getDbValue("SELECT * FROM property WHERE PropertyID = '" + propertyID + "'", "CompanyID").toString();
				propertyCode = Publics::getDbValue("SELECT * FROM property WHERE PropertyID = '" + propertyID + "'", "PropertyCode").toString();
				companyCode = Publics::getDbValue("SELECT * FROM company WHERE CompanyID = '" + companyID + "'", "Code").toString();

				if (rpt_cboCompany->currentIndex() != 0) {
					if (companyCode != rpt_cboCompany->currentText())
						toAdd = false;
				}

				if (rpt_cboProperty->currentIndex() != 0) {
					if (propertyCode != rpt_cboProperty->currentText())
						toAdd = false;
				}

				if (rpt_cboTenant->currentIndex() != 0) {
					if (tenantName != rpt_cboTenant->currentText())
						toAdd = false;
				}

				if (leaseCount > 1) {
					tenantName = "";
					tenantEmail = "";
					tenantTel = "";
				}

				QSqlQuery inv = QSqlDatabase::database().exec("SELECT * FROM invoice_master WHERE TenantID = '" + tenantID + "' "
																	     " AND UnitID = '" + unitID + "'");

				double bal = 0;
				while (inv.next()) {
					QString invDueDate = inv.record().value("InvoiceDate").toString();
					QDate dInvoiceDate = QDate::fromString(invDueDate, "dd-MMM-yyyy");
					dInvoiceDate = QDate(dInvoiceDate.year(), dInvoiceDate.month(), 28);
					if (dInvoiceDate < rpt_dtpReportQuery->date().addDays(1)) {
						QString invID = inv.record().value("InvoiceNO").toString();
						QString paidSoFar = Publics::getDbValue("SELECT SUM(Amount) as 'Paid' FROM payment_allocation WHERE InvoiceID = '" + invID + "'", "Paid").toString();
						QString invDue = inv.record().value("InvoiceTotal").toString();
						double m_bal = invDue.toDouble() - paidSoFar.toDouble();
						bal = bal + m_bal;
					}
				}

				balance = QString::number(bal);
				if (toAdd)
					QSqlDatabase::database().exec("INSERT INTO report_contact_list (Name, Tel, Email, Company, Property, Unit, Balance, Rent)"
								      " VALUES ('"
								      + tenantName + "', '"
								      + tenantTel + "', '"
								      + tenantEmail + "', '"
								      + companyCode + "', '"
								      + propertyCode + "', '"
								      + unitNo + "', '"
								      + balance + "', '"
								      + rent + "')");
			}

			bool showBlanks = true;
			if (rpt_cboCompany->currentIndex() != 0) {
				showBlanks = false;
			}
			if (rpt_cboProperty->currentIndex() != 0) {
				showBlanks = false;
			}
			if (rpt_cboTenant->currentIndex() != 0) {
				showBlanks = false;
			}
			if (showBlanks)
				if (leaseCount == 0) {
					//this tenant has no leases
					QString dash = "-";
					QSqlDatabase::database().exec("INSERT INTO report_contact_list (Name, Tel, Email, Company, Property, Unit, Rent)"
								      " VALUES ('"
								      + tenantName + "', '"
								      + tenantTel + "', '"
								      + tenantEmail + "', '"
								      + dash + "', '"
								      + dash + "', '"
								      + dash + "', '"
								      + dash + "')");
				}

		}

	}

	if (reportName == "Vacant Units")
		reportName = "vacant_units";

	if (reportName == "Occupied Units") {
		reportName = "occupied_units";
		if (rpt_cboCompany->currentIndex() != 0) {
			m_paramsToReplace.append("company_code");
			m_paramReplacements.append(rpt_cboCompany->currentText());
		}
	}

	showHtmlReport();
}

void RentManagerMainWindow::on_actionPrint_triggered()
{
	printer = new QPrinter(QPrinter::HighResolution);
	QPrintDialog *prnt = new QPrintDialog(printer, this);
	if (prnt->exec() == QDialog::Accepted) {
        ui->reportTextView->print(printer);
	}
}

void RentManagerMainWindow::on_actionExport_To_Excel_triggered()
{
	QString sql;
	if (rptName == "Contact List") {
		sql = "SELECT * FROM report_contact_list";
	}
	if (rptName == "Vacant Units") {
		sql = "SELECT company.Code as'Company', property.propertyCode as 'Property',unit.UnitNo as 'Unit No' FROM unit, property,company WHERE unit.PropertyID = property.PropertyID AND property.CompanyID = company.CompanyID AND Unit.Occupied = 'No' ";
		if (rpt_cboCompany->currentIndex() > 0)
			sql += " AND company.Code = '" + rpt_cboCompany->currentText() + "' ";

		if (rpt_cboProperty->currentIndex() > 0)
			sql += " AND property.PropertyCode = '" + rpt_cboProperty->currentText() + "'";

	}

	if (rptName == "Occupied Units") {
		sql = 	"SELECT company.Code as 'Company',  property.PropertyCode as 'Property', unit.UnitNo as 'Unit No.' , tenant.Name  as 'Tenant Name', leases.MonthlyRent as 'Rent Per Month'"
			"	FROM leases, unit, tenant, property, company WHERE LeaseActive='Yes'"
			"	AND leases.UnitID = unit.UnitID"
			"	AND leases.TenantID = tenant.TenantID"
			"	AND unit.PropertyID = property.PropertyID"
			"	AND property.CompanyID = company.CompanyID";

		if (rpt_cboCompany->currentIndex() > 0)
			sql += " AND company.Code = '" + rpt_cboCompany->currentText() + "' ";

		if (rpt_cboProperty->currentIndex() > 0)
			sql += " AND property.PropertyCode = '" + rpt_cboProperty->currentText() + "'";

	}

	QSqlQuery qu = db.exec(sql);

	QSqlQueryModel *rptModel = new QSqlQueryModel(this);
	rptModel->setQuery(sql, QSqlDatabase::database());

	int col_nums =rptModel->columnCount();



	workbook wb;
	worksheet *sh = wb.sheet(rptName.toStdString());

	sh->label(0, 0, rptName.toStdString());
	sh->label(1, 0, QDateTime::currentDateTime().toString("hh:mm:ss dd-MMM-yyyy").toStdString());


	int rowStart = 4;
	for (int c = 0; c < col_nums; c++) {
		sh->label(rowStart, c, rptModel->headerData(c, Qt::Horizontal, Qt::DisplayRole).toString().toStdString());
	}

	int r = rowStart + 1;
	while (qu.next()) {
		for (int c = 0; c < col_nums; c++) {
			sh->label(r, c, qu.record().value(c).toString().toStdString());
		}
		r++;
	}

	QString exportFileName = QFileDialog::getSaveFileName(this, "Excel File Name", QDir::homePath(), "Excel Files (*.xls)");
	if (exportFileName.length() > 0)
		wb.Dump(exportFileName.toStdString());
}

void RentManagerMainWindow::on_actionChange_Password_triggered()
{
	CompanyFilePassword *pass = new CompanyFilePassword(this, false);
	pass->exec() ;
}


void RentManagerMainWindow::on_actionDeposit_List_triggered()
{
	if (!m_deposits)
		m_deposits = new DepositDialog(this);

	m_deposits->startNew();
	m_deposits->exec();
}

void RentManagerMainWindow::on_actionView_Tenant_Accounts_triggered()
{
	if (!m_custAccount)
		m_custAccount = new CustomerAccountDialog(this);

	m_custAccount->startNew();
	m_custAccount->exec();
}

void RentManagerMainWindow::on_actionTerminate_Lease_triggered()
{
	if (!m_terminateLease)
		m_terminateLease = new TerminateLeaseDialog(this);

	m_terminateLease->startNew();
	m_terminateLease->exec();
}

void RentManagerMainWindow::reportCompanyChanged()
{
	if (rpt_cboCompany->currentIndex() != 0) {
		m_currentCompany = rpt_cboCompany->currentText();
		QString compID = Publics::getDbValue("SELECT * FROM company WHERE Code = '" + m_currentCompany + "'", "CompanyID").toString();
		Publics::loadQueryToCombo("SELECT * FROM property WHERE CompanyID = '" + compID + "'", "PropertyCode", rpt_cboProperty);

		rpt_cboProperty->insertItem(0, "--ALL--");

		rpt_cboProperty->setCurrentIndex(0);
	} else {
		m_currentCompany = "";
		Publics::loadQueryToCombo("SELECT * FROM property", "PropertyCode", rpt_cboProperty);

		rpt_cboProperty->insertItem(0, "--ALL--");
	}
}


void RentManagerMainWindow::on_cmdPrintCalendar_clicked()
{
	QPrinter *prnt = new QPrinter(QPrinter::HighResolution);
	QPrintDialog *dlg = new QPrintDialog(prnt, this);
	if (dlg->exec() == QDialog::Accepted)
        ui->calendarTextView->print(prnt);
}

void RentManagerMainWindow::showHtmlReport()
{
	QString html;
	QString manualHtml;
	QString reportTitle;
	QFile fl(":/reports/reports/html_pretext");
	if (!fl.open(QIODevice::ReadOnly)) {
		Publics::showError("Could not open report source.\n" + fl.errorString());
		return;
	}
	//File open
	html += fl.readAll();
	QString sql;

	bool autoSql = true;

	if (rptName == "Vacant Units") {
		sql = "SELECT company.Code as'Company', property.propertyCode as 'Property',unit.UnitNo as 'Unit No' FROM unit, property,company WHERE unit.PropertyID = property.PropertyID AND property.CompanyID = company.CompanyID AND Unit.Occupied = 'No' ";
		if (rpt_cboCompany->currentIndex() > 0)
			sql += " AND company.Code = '" + rpt_cboCompany->currentText() + "' ";

		if (rpt_cboProperty->currentIndex() > 0)
			sql += " AND property.PropertyCode = '" + rpt_cboProperty->currentText() + "'";

		reportTitle = "Vacant Units";
	}

	if (rptName == "Occupied Units") {
		sql = 	"SELECT company.Code as 'Company',  property.PropertyCode as 'Property', unit.UnitNo as 'Unit No.' , tenant.Name  as 'Tenant Name', leases.MonthlyRent as 'Rent Per Month'"
			"	FROM leases, unit, tenant, property, company WHERE LeaseActive='Yes'"
			"	AND leases.UnitID = unit.UnitID"
			"	AND leases.TenantID = tenant.TenantID"
			"	AND unit.PropertyID = property.PropertyID"
			"	AND property.CompanyID = company.CompanyID";

		if (rpt_cboCompany->currentIndex() > 0)
			sql += " AND company.Code = '" + rpt_cboCompany->currentText() + "' ";

		if (rpt_cboProperty->currentIndex() > 0)
			sql += " AND property.PropertyCode = '" + rpt_cboProperty->currentText() + "'";


		reportTitle = "Occupied Units";
	}


	if (rptName == "Contact List") {
		sql = "SELECT * FROM report_contact_list";
		reportTitle  = "Contact List";
	}

	if (rptName == "Companies / Properties") {
		reportTitle = "Companies / Properties";
		autoSql = false;
		QSqlQuery companiesQu = QSqlDatabase::database().exec("SELECT * FROM company");
		while (companiesQu.next()) {
			QString companyName = companiesQu.record().value("Code").toString();
			QString companyID = companiesQu.record().value("CompanyID").toString();
			manualHtml += "<br/><table class='reference' style='width:90%' align='center'>\n";
			//Append table headers
			manualHtml += QString("<tr><th align='left'><font face='verdana'>%1</font></th></tr>\n").arg(companyName);
			QSqlQuery propQu = QSqlDatabase::database().exec("SELECT * FROM property WHERE CompanyID = '" + companyID + "'");
			while (propQu.next()) {
				QString propName = propQu.record().value("PropertyCode").toString();
				manualHtml += "<tr><td style='font-size:11px'><font face='verdana'>" + propName + "</font></td></tr>\n";

			}
			manualHtml += "</table><br/>\n";
		}
	}
	html += "<body><h1 align=center><font face='verdana'>" + reportTitle + "</font></h1>";
	if (autoSql) {
		QSqlQuery qu = db.exec(sql);
		QStringList headers;

		QSqlQueryModel *rptModel = new QSqlQueryModel(this);
		rptModel->setQuery(sql, QSqlDatabase::database());

		int col_nums =rptModel->columnCount();

		for (int c = 0; c < col_nums; c++) {
			headers.append(rptModel->headerData(c, Qt::Horizontal, Qt::DisplayRole).toString());
		}
		html += "<table class='reference' style='width:90%' align='center'>\n";
		html += QString("<tr><td align='right' style='font-size:11px'><font face='verdana'>%1</font></td></tr>\n").arg(QDate::currentDate().toString("dd-MMM-yyyy"));
		html += "</table>\n";
		html += "<table class='reference' style='width:90%' align='center'>\n";
		//Append table headers
		html += "<tr>\n";
		for (int c = 0; c < col_nums; c++) {
			html += "<th><font face='verdana'>" + headers.at(c) + "</font></th>\n";
		}
		html += "</tr>\n";
		//Add table data
		while (qu.next()) {
			html += "<tr>\n";
			for (int c = 0; c < col_nums; c++) {
				html += "<td style='font-size:11px'><font face='verdana'>" + qu.record().value(c).toString() + "</font></td>\n";
			}
			html += "</tr>\n";
		}
		//Add html footer
		html += "</table>\n";
	} else {
		html += manualHtml;
	}
	html += "</body></html>";
    ui->reportTextView->setText(html);
}

void RentManagerMainWindow::on_actionPrint_Preview_triggered()
{
	QPrintPreviewDialog *prev = new QPrintPreviewDialog(printer, this);
	connect (prev, SIGNAL(paintRequested(QPrinter*)), SLOT(printReport(QPrinter*)));
	prev->exec();
}

void RentManagerMainWindow::printReport(QPrinter *prnt)
{
    ui->reportTextView->print(prnt);
}

void RentManagerMainWindow::on_actionPage_Setup_triggered()
{
	QPageSetupDialog *pg = new QPageSetupDialog(printer, this);
	pg->exec();
}

void RentManagerMainWindow::on_cmdExportCalendar_clicked()
{
	workbook wb;

	worksheet *sheet = wb.sheet(QString("Payment Calendar").toStdString());


	QSqlQuery unitQu = db.exec("SELECT * FROM unit WHERE PropertyID = '" + currentProperty + "'");
	QString propertyName = Publics::getDbValue("SELECT * FROM property WHERE PropertyID = '" + currentProperty + "'", "PropertyName").toString();
	QString companyId = Publics::getDbValue("SELECT * FROM property WHERE PropertyID = '" + currentProperty + "'", "CompanyID").toString();
	QString companyName = Publics::getDbValue("SELECT * FROM company WHERE CompanyID = '" + companyId + "'", "CompanyName").toString();
	QString physical = Publics::getDbValue("SELECT * FROM company WHERE CompanyID = '" + companyId + "'", "PhysicalAddress").toString();
	QString postal = Publics::getDbValue("SELECT * FROM company WHERE CompanyID = '" + companyId + "'", "PostalAddress").toString();
	QString headerTable;


	headerTable += "<table class='reference' style='width:100%' align='center'>";
	headerTable += QString("<tr style='font-face:verdana'><td align=center><b>%1</b><br/><b>%2</b><br/>%3<br/>%4</td></tr>")
			.arg(companyName, propertyName, physical, postal);
	headerTable += "</table>";
	headerTable += "<table class='reference' border=0 style='width:100%' align='center'>";
	headerTable += QString("<tr style='font-face:verdana'><td>Payment Calendar</td><td align=right>%1</td></tr>").arg(QDate::currentDate().toString("dd-MMM-yyyy"));
	headerTable += "</table>";

	sheet->label(0 , 0, companyName.toStdString());
	sheet->label(1, 0, propertyName.toStdString());
	sheet->label(2, 0, physical.toStdString());
	sheet->label(3, 0, postal.toStdString());



	QStringList months;
	months << "" << "Jan"
	       << "Feb"
	       << "Mar"
	       << "Apr"
	       << "May"
	       << "Jun"
	       << "Jul"
	       << "Aug"
	       << "Sep"
	       << "Oct"
	       << "Nov"
	       << "Dec";


	for (int i = 0; i < months.count(); i++)
	{
		sheet->label(5, i, months.at(i).toStdString());
	}

	int row = 5;
	while (unitQu.next()) {
		row ++;
		QString unitNo = unitQu.record().value("UnitNo").toString();
		QString unitID = unitQu.record().value("UnitID").toString();
		//first col is the unit name
		QSqlQuery leaseQu = db.exec("SELECT * FROM leases WHERE UnitID = '" + unitID + "' AND DateOfOccupation LIKE '%" + ui->cboYear->currentText() + "%'");
		QString names = "";
		while (leaseQu.next()) {
			QString tenantID = leaseQu.record().value("TenantID").toString();
			QString userName = Publics::getDbValue("SELECT * FROM tenant WHERE TenantID = '" + tenantID + "'", "Name").toString();
			names += userName;
		}
		QString fName = unitNo + "\n" + names;
		sheet->label(row, 0, fName.toStdString());
		for (int i = 1; i < 13; i++) {
			QDate dat(ui->cboYear->currentText().toInt(), i, 1);
			QString monthYear = dat.toString("MMM-yy");
			QSqlQuery invQu = QSqlDatabase::database().exec("SELECT * FROM invoice_master "
									" WHERE InvoiceMonthYear = '" + monthYear + "' AND UnitID = '" + unitID + "'");

			//QString invoiceNos;
			double paid = 0;
			double due = 0;
			double balance = 0;

			QString colText, bgColor = "LightGreen";
			while (invQu.next()) {
				QString invoiceID = invQu.record().value("InvoiceNo").toString();
				QSqlQuery allocQu = QSqlDatabase::database().exec("SELECT * FROM payment_allocation WHERE InvoiceID = '" + invoiceID + "'");
				while (allocQu.next()) {
					paid = paid + allocQu.record().value("Amount").toDouble();
				}
				due = due + invQu.record().value("InvoiceTotal").toDouble();
				balance = due - paid;
			}
			if (balance > 0) {
				bgColor = "Salmon";
			}

			if (balance == due) {
				bgColor = "Red";
			}

			if (balance < 1) {
				bgColor = "LightGreen";
			}

			if (due == 0) {
				bgColor = "LightBlue";
			}

			if (balance )
				colText = "";


			colText += "Due: " + QString::number(due);
			colText += "\nPd: " + QString::number(paid);
			colText += "\nBal: " + QString::number(balance);

			sheet->label(row, i, colText.toStdString());
		}
		sheet->rowheight(row, 820);
		//end of unit row
	}


	QString fileName = QFileDialog::getSaveFileName(this, "Export Calendar", QDir::homePath(), "Excel FIles (*.xls)");
	if (fileName.length() > 0)
		wb.Dump(fileName.toStdString());
}
