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

	void qtPreview(QString reportName, QStringList params, QStringList replacements, bool pdf = false, bool openPdf = true);

public slots:
	void printRequested(QPrinter *);
private:
	QPrinter *printer;
	QString m_reportName;
	QStringList m_paramsToReplace;
	QStringList m_paramReplacements;
	QString m_xml;
	bool m_makePdf;
	bool m_openPdf;
	QString m_pdfFileName;
protected:
	QDomDocument m_doc;
};

#endif // MYPRINTER_H
