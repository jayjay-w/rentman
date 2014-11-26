#ifndef TERMINATELEASEDIALOG_H
#define TERMINATELEASEDIALOG_H

#include <QDialog>
class QTreeWidgetItem;

namespace Ui {
class TerminateLeaseDialog;
}

class TerminateLeaseDialog : public QDialog
{
	Q_OBJECT

public:
	explicit TerminateLeaseDialog(QWidget *parent = 0);
	~TerminateLeaseDialog();
	void startNew();
private slots:
	void on_cmdTerminateLease_clicked();

	void on_trvLeases_itemClicked(QTreeWidgetItem *item, int column);

private:
	Ui::TerminateLeaseDialog *ui;
	QString m_leaseID;
};

#endif // TERMINATELEASEDIALOG_H
