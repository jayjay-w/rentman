#ifndef MYPRINTER_H
#define MYPRINTER_H

#include <QObject>
#include <QtSql>
#include <QPrinter>
#include <QWidget>
#include <QtXml>

class MyPrinter: public QObject
{
	Q_OBJECT
public:
	MyPrinter(QObject *mzazi);
	static void showReport(QString reportName = "", QString stringToReplace = "param_id",
			       QString replaceValue = "new_param"
			       );
	static void showReport(QString reportName = "", QStringList stringsToReplace = QStringList(),
			       QStringList stringsToUse = QStringList()
			       );
};

#endif // MYPRINTER_H
