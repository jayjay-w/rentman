#ifndef ASSIGNUNITTOTENANTDIALOG_H
#define ASSIGNUNITTOTENANTDIALOG_H

#include <QDialog>

namespace Ui {
class AssignUnitToTenantDialog;
}

class AssignUnitToTenantDialog : public QDialog
{
	Q_OBJECT

public:
	explicit AssignUnitToTenantDialog(QWidget *parent = 0);
	~AssignUnitToTenantDialog();
	void reloadLists();
private slots:
	void on_cmdSave_clicked();

	void on_chkVAT_clicked();

	void calculateVAT();

	void on_chkVAT_toggled(bool checked);

private:
	Ui::AssignUnitToTenantDialog *ui;
};

#endif // ASSIGNUNITTOTENANTDIALOG_H
