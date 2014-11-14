#include "myprinter.h"
#include "publics.h"
//#include "renderobjects.h"
//#include "orprerender.h"
//#include "previewdialog.h"
//#include "orprintrender.h"
//#include "parameter.h"
#include <QPrintPreviewDialog>
#include <QDomDocument>

MyPrinter::MyPrinter(QObject *mzazi) :
	QObject(mzazi)
{
}

void MyPrinter::showReport(QString reportName, QString stringToReplace, QString replaceValue)
{
	QFile fl(":/reports/reports/" + reportName + ".xml");
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
//	ORPreRender pre;
//	pre.setDatabase(QSqlDatabase::database());
//	pre.setDom(doc);
//	ORODocument *oDoc = pre.generate();
//	PreviewDialog *diag = new PreviewDialog(oDoc, new QPrinter(QPrinter::HighResolution), 0);
//	diag->exec();
}

void MyPrinter::showReport(QString reportName, QStringList stringsToReplace, QStringList stringsToUse)
{
	QFile fl(":/reports/reports/" + reportName + ".xml");
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
//	ORPreRender pre;
//	pre.setDatabase(QSqlDatabase::database());
//	pre.setDom(doc);
//	ORODocument *oDoc = pre.generate();
//	PreviewDialog *diag = new PreviewDialog(oDoc, new QPrinter(QPrinter::HighResolution), 0);
//	diag->exec();
}

void MyPrinter::qtPreview(QString reportName, QStringList params, QStringList replacements, bool pdf, bool openPdf)
{
	printer = new QPrinter(QPrinter::HighResolution);
	m_reportName = reportName;
	m_paramsToReplace = params;
	m_paramReplacements = replacements;
	m_makePdf = pdf;
	m_openPdf = openPdf;

	qDebug() << params << replacements;
	QFile fl(":/reports/reports/" + reportName + ".xml");
	if (!fl.open(QIODevice::ReadOnly)) {
		Publics::showError("Could not open report source.\n" + fl.errorString());
		return;
	}
	//File open
	QString xml = fl.readAll();
	//	for (int i = 0; i < params.count(); i++) {
	//		xml.replace(params.at(i), replacements.at(i));
	//	}
	m_xml = xml;
	m_doc.setContent(m_xml);
	fl.close();

	QPrintPreviewDialog *pvr = new QPrintPreviewDialog(printer, 0);
	connect (pvr, SIGNAL(paintRequested(QPrinter*)), SLOT(printRequested(QPrinter*)));
	pvr->exec();
}

void MyPrinter::printRequested(QPrinter */*prnt*/)
{
//	ORPreRender pre;
//	pre.setDatabase(QSqlDatabase::database());
//	pre.setDom(m_doc);
//	ParameterList params = ParameterList();
//	for (int i = 0; i < m_paramsToReplace.count(); i++) {
//		Parameter param(m_paramsToReplace.at(i), m_paramReplacements.at(i));
//		params.append(param);
//	}
//	pre.setParamList(params);
//	ORODocument *oDoc = pre.generate();

//	if (oDoc) {
//		ORPrintRender render;
//		render.setPrinter(printer);
//		render.render(oDoc);
//		render.setupPrinter(oDoc, printer);
//	} else {
//		Publics::showError("Printer Error");
//	}
}
