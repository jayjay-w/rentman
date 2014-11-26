#ifndef CUSTOMERACCOUNTDIALOG_H
#define CUSTOMERACCOUNTDIALOG_H

#include <QDialog>

class QTreeWidgetItem;

namespace Ui {
class CustomerAccountDialog;
}

class CustomerAccountDialog : public QDialog
{
	Q_OBJECT

public:
	explicit CustomerAccountDialog(QWidget *parent = 0);
	~CustomerAccountDialog();
	void startNew();
private slots:
	void on_trvTenants_itemClicked(QTreeWidgetItem *item, int column);

private:
	Ui::CustomerAccountDialog *ui;
};

#endif // CUSTOMERACCOUNTDIALOG_H
