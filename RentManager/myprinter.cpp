#include "myprinter.h"
#include "publics.h"
#include "renderobjects.h"
#include "orprerender.h"
#include "previewdialog.h"

#include <QDomDocument>

MyPrinter::MyPrinter(QObject *mzazi) :
	QObject(mzazi)
{
}

void MyPrinter::showReport(QString reportName, QString stringToReplace, QString replaceValue)
{
	QFile fl(qApp->applicationDirPath() + QDir::separator() + reportName);
	if (!fl.open(QIODevice::ReadOnly)) {
		Publics::showError("Could not open report source.\n" + fl.errorString());
		return;
	}
	//File open
	QString xml = fl.readAll();
	xml.replace(stringToReplace, replaceValue);
	QDomDocument doc;
	doc.setContent(xml);
	fl.close();
	qDebug() << xml;
	ORPreRender pre;
	pre.setDatabase(QSqlDatabase::database());
	pre.setDom(doc);
	ORODocument *oDoc = pre.generate();
	PreviewDialog *diag = new PreviewDialog(oDoc, new QPrinter(QPrinter::HighResolution), 0);
	diag->exec();
}

void MyPrinter::showReport(QString reportName, QStringList stringsToReplace, QStringList stringsToUse)
{
	QFile fl(qApp->applicationDirPath() + QDir::separator() + reportName);
	if (!fl.open(QIODevice::ReadOnly)) {
		Publics::showError("Could not open report source.\n" + fl.errorString());
		return;
	}
	//File open
	QString xml = fl.readAll();
	for (int i = 0; i < stringsToReplace.count(); i++) {
		xml.replace(stringsToReplace.at(i), stringsToUse.at(i));
	}
	QDomDocument doc;
	doc.setContent(xml);
	fl.close();
	qDebug() << xml;
	ORPreRender pre;
	pre.setDatabase(QSqlDatabase::database());
	pre.setDom(doc);
	ORODocument *oDoc = pre.generate();
	PreviewDialog *diag = new PreviewDialog(oDoc, new QPrinter(QPrinter::HighResolution), 0);
	diag->exec();
}
