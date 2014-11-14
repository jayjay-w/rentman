#include "terminateoccupationdialog.h"
#include "ui_terminateoccupationdialog.h"

TerminateOccupationDialog::TerminateOccupationDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::TerminateOccupationDialog)
{
	ui->setupUi(this);
	ui->dtpDateTerminated->setDate(QDate::currentDate());
}

TerminateOccupationDialog::~TerminateOccupationDialog()
{
	delete ui;
}

void TerminateOccupationDialog::on_cmdTerminate_clicked()
{

}
