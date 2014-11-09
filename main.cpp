#include "rentmanagermainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	RentManagerMainWindow w;
	w.show();

	return a.exec();
}
