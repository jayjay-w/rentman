#ifndef UNITSDIALOG_H
#define UNITSDIALOG_H

#include <QDialog>

namespace Ui {
class UnitsDialog;
}

class UnitsDialog : public QDialog
{
	Q_OBJECT

public:
	explicit UnitsDialog(QWidget *parent = 0);
	~UnitsDialog();
	QString m_propertyID;
	QString m_unitID;
	bool isAdding;
	void edit(QString id);
	void addNew(QString propertyID);
private slots:
	void on_cmdSave_clicked();

	void on_cmdEndOccupation_clicked();

private:
	Ui::UnitsDialog *ui;
};

#endif // UNITSDIALOG_H
