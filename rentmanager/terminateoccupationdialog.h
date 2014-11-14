#ifndef TERMINATEOCCUPATIONDIALOG_H
#define TERMINATEOCCUPATIONDIALOG_H

#include <QDialog>

namespace Ui {
class TerminateOccupationDialog;
}

class TerminateOccupationDialog : public QDialog
{
	Q_OBJECT

public:
	explicit TerminateOccupationDialog(QWidget *parent = 0);
	~TerminateOccupationDialog();

private slots:
	void on_cmdTerminate_clicked();

private:
	Ui::TerminateOccupationDialog *ui;
};

#endif // TERMINATEOCCUPATIONDIALOG_H
