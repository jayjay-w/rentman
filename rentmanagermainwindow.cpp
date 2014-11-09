#include "rentmanagermainwindow.h"
#include "ui_rentmanagermainwindow.h"

RentManagerMainWindow::RentManagerMainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::RentManagerMainWindow)
{
	ui->setupUi(this);
}

RentManagerMainWindow::~RentManagerMainWindow()
{
	delete ui;
}
