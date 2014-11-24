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
#include "myprinter.h"
#include "createinvoicedialog.h"
#include "createmultiinvoice.h"
#include "smartpaymentdialog.h"
#include "renderobjects.h"
#include "orprerender.h"
#include "previewdialog.h"
#include "orprintrender.h"
#include "parameter.h"
#include <QDomDocument>
#include <QPrinter>
RentManagerMainWindow *RentManagerMainWindow::m_instance = NULL;

RentManagerMainWindow::RentManagerMainWindow(QWidget *parent) :
	QMainWindow(),
	actionsToDisable(0),
	m_companiesDialog(0),
	m_propertiesDialog(0),
	m_tenants(0),
	m_assign(0),
	m_payDiag(0),
	m_printer(0),
	m_singleInvoice(0),
	m_multiInvoice(0),
	m_smartPayment(0),
	ui(new Ui::RentManagerMainWindow)
{
	Q_ASSERT_X(m_instance == NULL, "MainWindow", "MainWindow recreated!");
	m_instance = this;
	Q_UNUSED(parent);
	ui->setupUi(this);

	//ActionGroup
	if (!actionsToDisable)
		actionsToDisable = new QActionGroup(this);

	m_printer = new MyPrinter(this);

	actionsToDisable->addAction(ui->actionAssign_Unit_To_Tenant);
	actionsToDisable->addAction(ui->actionCompanies);
	actionsToDisable->addAction(ui->actionCreate_Invoice);
	actionsToDisable->addAction(ui->actionMulti_Invoicing);
	actionsToDisable->addAction(ui->actionProperties);
	actionsToDisable->addAction(ui->actionReceive_Payments);
	actionsToDisable->addAction(ui->actionTenants);
	actionsToDisable->addAction(ui->actionUnites);
	actionsToDisable->addAction(ui->actionView_Tenan_Accounts);
	actionsToDisable->addAction(ui->action_Save);
	actionsToDisable->addAction(ui->actionSave_As);
	actionsToDisable->addAction(ui->actionReceive_Payments);
	actionsToDisable->addAction(ui->actionView_Payments);
	actionsToDisable->addAction(ui->actionContact_List);

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
	//ui restore
	QSettings sett(qApp->organizationName(), qApp->applicationName());
	qDebug() << sett.value("MainWindowSplitter").toString();
	ui->splitter->restoreState(sett.value("MainWindowSplitter",
					      ui->splitter_2->saveState()).toByteArray());

	printer = new QPrinter(QPrinter::HighResolution);
	connect (ui->printPreviewWidget, SIGNAL(paintRequested(QPrinter*)), SLOT(previewRequested(QPrinter*)));
	//ui->printPreviewWidget = new QPrintPreviewWidget(printer);
}

RentManagerMainWindow::~RentManagerMainWindow()
{
	//ui save state
	QSettings sett(qApp->organizationName(), qApp->applicationName());
	sett.setValue("MainWindowSplitter", ui->splitter_2->saveState());
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
	ui->txtCalendar->setText("");

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
		ui->txtCalendar->setHtml(html);
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
	m_printer->qtPreview("units", QStringList("param_where"), QStringList(""));
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
	QSqlQuery unitQu = db.exec("SELECT * FROM unit WHERE PropertyID = '" + currentProperty + "'");
	QString propertyName = Publics::getDbValue("SELECT * FROM property WHERE PropertyID = '" + currentProperty + "'", "PropertyName").toString();
	html += "<h2>Payment calendar for " + propertyName + "</h2><br/>";

	html += "<table border=1 width=100%>";
	html += "<tr><td></td>"
		"<td><b>Jan</b></td>"
		"<td><b>Feb</b></td>"
		"<td><b>Mar</b></td>"
		"<td><b>Apr</b></td>"
		"<td><b>May</b></td>"
		"<td><b>Jun</b></td>"
		"<td><b>Jul</b></td>"
		"<td><b>Aug</b></td>"
		"<td><b>Sep</b></td>"
		"<td><b>Oct</b></td>"
		"<td><b>Nov</b></td>"
		"<td><b>Dec</b></td></tr>";

	while (unitQu.next()) {
		QString unitNo = unitQu.record().value("UnitNo").toString();
		QString unitID = unitQu.record().value("UnitID").toString();
		html += "<tr>"; //start or unit row
		//first col is the unit name
		html += "<td><b>" + unitNo + "</b></td>";
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
			colText += "<br/>Paid: " + QString::number(paid);
			colText += "<br/>Balance: " + QString::number(balance);

			html += "<td bgcolor=" + bgColor + ">";
			html += colText;
			html += "</td>";
		}
		html += "</tr>"; //end of unit row
	}

	html += "</table>";

	ui->txtCalendar->setHtml(html);
}

void RentManagerMainWindow::on_cboYear_currentIndexChanged(int index)
{
	Q_UNUSED(index);
	reloadCalendar();
}

void RentManagerMainWindow::on_actionContact_List_triggered()
{
	QSqlDatabase::database().exec("DELETE FROM report_contact_list");
	QSqlQuery tenantQu = QSqlDatabase::database().exec("SELECT * FROM tenant");
	while (tenantQu.next()) {
		QString tenantName, tenantTel, tenantEmail;
		tenantName = tenantQu.record().value("Name").toString();
		tenantTel = tenantQu.record().value("Tel").toString();
		tenantEmail = tenantQu.record().value("Email").toString();

		QSqlQuery leasesQu = QSqlDatabase::database().exec("SELECT * FROM leases WHERE IsOngoing = 'Yes' AND TenantID = '" +
								   tenantQu.record().value("TenantID").toString() + "'"
								   );
		int leaseCount = 0;

		while (leasesQu.next()) {
			leaseCount++;
			QString unitID = leasesQu.record().value("UnitID").toString();
			QString rent = leasesQu.record().value("MonthlyRent").toString();
			QString balance = "0";
			QString asOfDate = QDate::currentDate().toString("dd-MMM-yyyy");
			QString unitNo, propertyID, companyID, propertyCode, companyCode;
			unitNo = Publics::getDbValue("SELECT * FROM unit WHERE UnitID = '" + unitID + "'", "UnitNo").toString();
			propertyID = Publics::getDbValue("SELECT * FROM unit WHERE UnitID = '" + unitID + "'", "PropertyID").toString();
			companyID = Publics::getDbValue("SELECT * FROM property WHERE PropertyID = '" + propertyID + "'", "CompanyID").toString();
			propertyCode = Publics::getDbValue("SELECT * FROM property WHERE PropertyID = '" + propertyID + "'", "PropertyCode").toString();
			companyCode = Publics::getDbValue("SELECT * FROM company WHERE CompanyID = '" + companyID + "'", "Code").toString();

			if (leaseCount > 1) {
				tenantName = "";
				tenantEmail = "";
				tenantTel = "";
			}

			QSqlDatabase::database().exec("INSERT INTO report_contact_list (Name, Tel, Email, Company, Property, Unit, Rent)"
						      " VALUES ('"
						      + tenantName + "', '"
						      + tenantTel + "', '"
						      + tenantEmail + "', '"
						      + companyCode + "', '"
						      + propertyCode + "', '"
						      + unitNo + "', '"
						      + rent + "')");
		}

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
	m_printer->qtPreview("contact_list", QStringList("param_where"), QStringList(""));
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
	showReportPreview(item->text());
}

void RentManagerMainWindow::showReportPreview(QString reportName)
{
	if (reportName == "Contact List")
		reportName = "contact_list";

	printer = new QPrinter(QPrinter::HighResolution);
	m_reportName = reportName;
	QFile fl(":/reports/reports/" + reportName + ".xml");
	if (!fl.open(QIODevice::ReadOnly)) {
		Publics::showError("Could not open report source.\n" + fl.errorString());
		return;
	}
	//File open
	QString xml = fl.readAll();
	m_xml = xml;
	m_doc.setContent(m_xml);
	fl.close();

	ui->printPreviewWidget->updatePreview();
}

void RentManagerMainWindow::previewRequested(QPrinter *p)
{
	qDebug() << "HERE";
	ORPreRender pre;
	pre.setDatabase(QSqlDatabase::database());
	pre.setDom(m_doc);
	ORODocument *oDoc = pre.generate();

	if (oDoc) {
		ORPrintRender render;
		render.setPrinter(p);
		render.render(oDoc);
		render.setupPrinter(oDoc, p);
	} else {
		if (currentFilePath.length() > 0)
			Publics::showError("Printer Error");
	}
}
