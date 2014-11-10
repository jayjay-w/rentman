#ifndef COMPANIESDIALOG_H
#define COMPANIESDIALOG_H

#include <QDialog>

namespace Ui {
class CompaniesDialog;
}

class CompaniesDialog : public QDialog
{
	Q_OBJECT

public:
	explicit CompaniesDialog(QWidget *parent = 0);
	~CompaniesDialog();

private:
	Ui::CompaniesDialog *ui;
};

#endif // COMPANIESDIALOG_H
