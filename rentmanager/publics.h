#ifndef PUBLICS_H
#define PUBLICS_H

#include <QObject>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QFileDialog>
#include <QApplication>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QComboBox>


class Publics : public QObject
{
	Q_OBJECT
public:
	explicit Publics(QObject *parent = 0);
	enum SQL_STRING {
		SQL_COMPANY,
		SQL_PROPERTIES,
		SQL_UNITS,
		SQL_TENANTS,
		SQL_TENANT_UNITS,
		SQL_INVOICE_MASTER,
		SQL_INVOICE_DETAIL,
		SQL_PAYMENTS,
		SQL_INVOICE_ITEMS,
		SQL_LOG
	};

	static QString lastPath();
	static QString getOpenFile(QWidget *parent = 0, QString filter = "Rent Manager Files (*.rmf)");
	static QString getSaveFile(QWidget *parent = 0, QString filter = "Rent Manager Files (*.rmf)");
	static void saveSetting(QString settingName = "", QVariant value = 0);
	static QVariant getSetting(QString settingName = "", QVariant defaultValue = 0);
	static QString getSql(SQL_STRING sqlString);
	static QVariant getDbValue(QString query, QString returnCol);
	static void loadQueryToCombo(QString query = "SELECT NOW()", QString col = "NOW", QComboBox *cbo = 0,
				     QVariant userData = 0);
	static void setComboBoxText(QComboBox *cbo, QString text);
	static void clearTextBoxes(QWidget *parent);
	static void showError(QString message = "An error has occured.", QString title = "Error");

signals:

public slots:

};

#endif // PUBLICS_H
