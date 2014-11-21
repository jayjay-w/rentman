#include "publics.h"
#include <QSettings>

#include <QLineEdit>
#include <QDebug>

Publics::Publics(QObject *parent) :
	QObject(parent)
{
}

QString Publics::lastPath()
{
	return getSetting("lastPath", QDir::homePath()).toString();
}

QString Publics::getOpenFile(QWidget *parent, QString filter)
{
	QFileDialog dg;
	QString ret = dg.getOpenFileName(parent, "New File Name", lastPath(), filter);
	if (ret.length() > 0) {
		saveSetting("lastPath", ret);
	}
	return ret;
}

QString Publics::getSaveFile(QWidget *parent, QString filter)
{
	QFileDialog dg;
	QString ret = dg.getSaveFileName(parent, "New File Name", lastPath(), filter);
	if (ret.length() > 0) {
		saveSetting("lastPath", ret);
	}
	return ret;
}

void Publics::saveSetting(QString settingName, QVariant value)
{
#if QT_VERSION > 0x50000
	QSettings sett(qApp->organizationName(), qApp->applicationDisplayName());
#endif
#if QT_VERSION < 0x50000
	QSettings sett(qApp->organizationName(), qApp->applicationName());
#endif
	sett.setValue(settingName, value);
}

QVariant Publics::getSetting(QString settingName, QVariant defaultValue)
{
#if QT_VERSION > 0x50000
	QSettings sett(qApp->organizationName(), qApp->applicationDisplayName());
#endif
#if QT_VERSION < 0x50000
	QSettings sett(qApp->organizationName(), qApp->applicationName());
#endif
	return sett.value(settingName, defaultValue);
}

QString Publics::getSql(SQL_STRING sqlString)
{
	switch (sqlString) {
	case SQL_COMPANY:
		return "CREATE TABLE IF NOT EXISTS 'company' ('CompanyID' INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, 'CompanyName' Text,"
		       "'Code' TEXT, 'PhysicalAddress' Text, 'PostalAddress' Text, 'ContactPerson' Text,"
		       "'Mobile' Text, 'Tel1' Text,'Tel2' Text,"
		       "'Fax' Text, 'Email' Text, 'appName' Text DEFAULT('rentman'),"
		       "'CurrentMonth' Integer, 'dbVersion' TEXT DEFAULT('1.0'))";
	case SQL_PROPERTIES:
		return "CREATE TABLE IF NOT EXISTS 'property' ('PropertyID' INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
		       "'CompanyID' INTEGER,"
		       "'PropertyCode' TEXT, 'PropertyName' TEXT, 'Location' TEXT, 'Street' TEXT, 'LRNumber' TEXT, 'DateBought' TEXT,"
		       "'UnitCount' INTEGER DEFAULT('0'))";
	case SQL_UNITS:
		return "CREATE TABLE IF NOT EXISTS 'unit' ('UnitID' INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
		       "'PropertyID' INTEGER, 'CompanyID' INTEGER,"
		       "'UnitNo' TEXT, 'RoomCount' INTEGER DEFAULT('1'), 'MonthlyRent' REAL DEFAULT('10000'),"
		       "'PremiseType' TEXT, 'SQFT' TEXT, 'WaterBillAc' TEXT, 'ElecBillAcc' TEXT,"
		       "'CurrentTenantID' INTEGER DEFAULT('0'), Occupied TEXT DEFAULT('No'))";
	case SQL_TENANTS:
		return "CREATE TABLE IF NOT EXISTS 'tenant' ('TenantID' INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
		       "'Name' TEXT, 'Sex' TEXT Default('Male'), 'Tel' TEXT, 'Email' TEXT, 'IsActive' TEXT Default('No'), "
		       "'StartDate' TEXT, 'EndDate' TEXT"
		       ")";

	case SQL_TENANT_UNITS:
		return "CREATE TABLE IF NOT EXISTS 'leases' ('EntryID' INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
		       "'UnitID' INTEGER, 'TenantID' INTEGER, 'IsOngoing' TEXT DEFAULT('Yes'),"
		       "'DateOfOccupation' TEXT, 'ExitDate' TEXT, 'Monthlyrent' REAL DEFAULT('0'), 'Deposit' REAL DEFAULT('0'))";

	case SQL_PAYMENTS:
		return "CREATE TABLE IF NOT EXISTS 'payments' ('EntryID' INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
		       "'CompanyID' TEXT, 'CompanyName' TEXT, 'UniqueID' TEXT, "
		       "'UnitID' TEXT, 'TenantID' TEXT, 'ReceiptNo' TEXT, 'DateReceived' TEXT, 'UnitName' TEXT,"
		       "'TenantName' TEXT, 'AmountReceived' TEXT, 'PaymentFor' TEXT, 'PayMode' TEXT,"
		       "'Month' TEXT, 'InvoiceID' TEXT, 'MonthNo' INTEGER, 'Year' TEXT, 'ChequeNo' TEXT,"
				"'Allocated' TEXT DEFAULT('0'), 'Balance' TEXT DEFAULT('0')"
		       ")";
	case SQL_INVOICE_ITEMS:
		return "CREATE TABLE IF NOT EXISTS 'invoice_item' ('EntryID' INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
		       "'ItemName' TEXT, 'Description' TEXT)";

	case SQL_INVOICE_MASTER:
		return "CREATE TABLE IF NOT EXISTS 'invoice_master' ('EntryID' INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
				"'InvoiceMonthYear' TEXT, 'InvoiceNo' TEXT, 'PaidAmount' TEXT DEFAULT('0'), 'Balance' TEXT DEFAULT('0'),"
		       "'UniqueID' TEXT, 'HasVAT' TEXT Default('No'), 'VATPerc' TEXT Default('0'),"
		       "'VATAmount' TEXT Default('0'), 'GrandTotal' TEXT Default('0'),"
		       "'TenantID' INTEGER, 'UnitID' INTEGER, 'PropertyID' INTEGER, 'CompanyID' INTEGER, "
		       "'InvoiceDate' TEXT, 'TenantName' TEXT, 'TenantAddress' TEXT, "
		       "'InvoiceTotal' REAL, "
		       "'AmountInWords' TEXT)";

	case SQL_INVOICE_DETAIL:
		return "CREATE TABLE IF NOT EXISTS 'invoice_detail' ('EntryID' INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
		       "'UniqueID' TEXT, 'ItemID' INTEGER, 'ItemName' TEXT, 'Amount' REAL)";

	case SQL_PAYMENT_ALLOCATION:
		return "CREATE TABLE IF NOT EXISTS 'payment_allocation' ('EntryID' INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
				"'UniqueID' TEXT,'PaymentID' TEXT, 'InvoiceID' TEXT, 'Amount' TEXT DEFAULT('0'))";


	default:
		return "SELECT NOW()";
	}
}

QVariant Publics::getDbValue(QString query, QString returnCol)
{
	QSqlDatabase db = QSqlDatabase::database();
	QSqlQuery qu = db.exec(query);
	qu.first();
	return qu.record().value(returnCol);
}

void Publics::loadQueryToCombo(QString query, QString col, QComboBox *cbo, QVariant userData)
{
	cbo->clear();
	QSqlDatabase db = QSqlDatabase::database();
	QSqlQuery qu = db.exec(query);
	while (qu.next()) {
		cbo->addItem(qu.record().value(col).toString(), userData);
	}
}

///
/// \brief Publics::setComboBoxText
/// useful for pre Qt5 versions that dont have QComboBox::setCurrentText()
/// \param cbo
/// \param text
///
void Publics::setComboBoxText(QComboBox *cbo, QString text)
{
#if QT_VERSION < 0x50000
	for (int i = 0; i < cbo->count(); i++) {
		if (cbo->itemText(i) == text) {
			cbo->setCurrentIndex(i);
			return;
		}
	}
#endif
#if QT_VERSION > 0x50000
	cbo->setCurrentText(text);
#endif
}

void Publics::clearTextBoxes(QWidget *parent)
{
	for (int i = 0; i < parent->children().count(); i++) {
		if (qobject_cast<QLineEdit *>(parent->children().at(i))) {
			QLineEdit *txt = qobject_cast<QLineEdit *>(parent->children().at(i));
			txt->setText("");
		}
		if (parent->children().at(i)->children().count() > 0) {
			if (qobject_cast<QWidget *>(parent->children().at(i))) {
				QWidget *wid = qobject_cast<QWidget *>(parent->children().at(i));
				if (wid->children().count() > 0) {
					clearTextBoxes(wid);
				}
			}
		} else {
			if (qobject_cast<QLineEdit *>(parent->children().at(i))) {
				QLineEdit *txt = qobject_cast<QLineEdit *>(parent->children().at(i));
				txt->setText("");
			}
		}
	}
}

void Publics::showError(QString message, QString title)
{
	QMessageBox::critical(0, title, message);
}
