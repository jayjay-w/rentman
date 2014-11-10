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

private:
	Ui::UnitsDialog *ui;
};

#endif // UNITSDIALOG_H
