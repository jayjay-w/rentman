#ifndef COMPANIESDIALOG_H
#define COMPANIESDIALOG_H

#include <QDialog>

class QTreeWidgetItem;

namespace Ui {
class CompaniesDialog;
}

class CompaniesDialog : public QDialog
{
	Q_OBJECT

public:
	explicit CompaniesDialog(QWidget *parent = 0);
	~CompaniesDialog();
public slots:
	void reloadCompanies();
	void editCompany();
private slots:
	void on_cmdNewCompany_clicked();

	void on_trvCompanies_itemClicked(QTreeWidgetItem *item, int column);

	void on_cmdSaveChanges_clicked();

	void on_cmdDeleteCompany_clicked();

	void on_trvProperties_itemDoubleClicked(QTreeWidgetItem *item, int column);

private:
	Ui::CompaniesDialog *ui;
	QString m_currentCompany;
};

#endif // COMPANIESDIALOG_H
