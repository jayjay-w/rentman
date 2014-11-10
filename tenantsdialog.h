#ifndef TENANTSDIALOG_H
#define TENANTSDIALOG_H

#include <QDialog>


class QTreeWidgetItem;

namespace Ui {
class TenantsDialog;
}

class TenantsDialog : public QDialog
{
	Q_OBJECT

public:
	explicit TenantsDialog(QWidget *parent = 0);
	~TenantsDialog();
	QString m_tenantID;
public slots:
	void reloadTenants();
	void editTenant();
private slots:
	void newTenant();
	void saveChanges();
	void deleteTenant();
	void tenantChanged(QTreeWidgetItem *item, int col);
private:
	Ui::TenantsDialog *ui;
};

#endif // TENANTSDIALOG_H
