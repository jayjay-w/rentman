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

RentManagerMainWindow *RentManagerMainWindow::m_instance = NULL;

RentManagerMainWindow::RentManagerMainWindow(QWidget *parent) :
	QMainWindow(),
	actionsToDisable(0),
	m_companiesDialog(0),
	m_propertiesDialog(0),
	m_tenants(0),
	m_assign(0),
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
	actionsToDisable->addAction(ui->actionView_Tenan_Accounts);
	actionsToDisable->addAction(ui->action_Save);
	actionsToDisable->addAction(ui->actionSave_As);

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

	//ui restore
	ui->splitter->restoreState(Publics::getSetting("MainSplitter",
						       ui->splitter->saveState()).toByteArray());
}

RentManagerMainWindow::~RentManagerMainWindow()
{
	//ui save state
	Publics::saveSetting("MainSplitter", ui->splitter->saveState());
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
	ui->tblUnits->clearContents();

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
	ReceivePaymentDialog *rec = new ReceivePaymentDialog(this);
	rec->exec();
}

void RentManagerMainWindow::on_trvBrowser_itemClicked(QTreeWidgetItem *item, int column)
{
	Q_UNUSE(column);
	if (item->text(98) == "property") {

	}
}
