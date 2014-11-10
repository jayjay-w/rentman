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
				"'UnitNo' TEXT, 'RoomCount' INTEGER DEFAULT('1'), 'MonthlyRent' INTEGER DEFAULT('10000'),"
				"'PremiseType' TEXT, 'SQFT' TEXT, 'WaterBillAc' TEXT, 'ElecBillAcc' TEXT,"
				"'CurrentTenantID' INTEGER DEFAULT('0'))";
	case SQL_TENANTS:
		return "CREATE TABLE IF NOT EXISTS 'tenant' ('UnitID' INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
				"'Name' TEXT, 'Sex' TEXT Default('Male'), 'Tel' TEXT, 'Email' TEXT"
				")";

	case SQL_TENANT_UNITS:
		return "CREATE TABLE IF NOT EXISTS 'tenant_units' ('EntryID' INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
				"'UnitID' INTEGER, 'TenantID' INTEGER,"
				"'DateOfOccupation' TEXT, 'Monthlyrent' TEXT, 'Deposit' TEXT)";
	default:
		return "";
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
