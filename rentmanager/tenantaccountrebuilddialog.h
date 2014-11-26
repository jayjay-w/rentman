#ifndef TENANTACCOUNTREBUILDDIALOG_H
#define TENANTACCOUNTREBUILDDIALOG_H

#include <QDialog>
#include <QThread>
#include <QtSql>

namespace Ui {
class TenantAccountRebuildDialog;
}

class TenantAccountRebuildDialog : public QDialog
{
	Q_OBJECT

public:
	explicit TenantAccountRebuildDialog(QWidget *parent = 0, QString tenantID = "");
	~TenantAccountRebuildDialog();
private slots:
	void threadFinished();
private:
	Ui::TenantAccountRebuildDialog *ui;
	QString m_tenantID;
};

class AccountRebuildThread : public QThread {
	Q_OBJECT
public:
	explicit AccountRebuildThread(QObject *parent = 0, QString tenantID = "");
	void run();
signals:
	void progress(int);
private:
	QString m_tenantID;

};

#endif // TENANTACCOUNTREBUILDDIALOG_H
