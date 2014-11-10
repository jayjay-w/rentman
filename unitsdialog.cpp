#include "unitsdialog.h"
#include "ui_unitsdialog.h"

UnitsDialog::UnitsDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::UnitsDialog)
{
	ui->setupUi(this);
}

UnitsDialog::~UnitsDialog()
{
	delete ui;
}
