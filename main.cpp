#include "rentmanagermainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	a.setApplicationName("rentmanager");
#if QT_VERSION > 0x50000
	a.setApplicationDisplayName("Rent Manager");
#endif
	a.setApplicationVersion("0.1");
	a.setOrganizationName("Panther Technologies");
	a.setOrganizationDomain("panthertechno.co.ke");

	RentManagerMainWindow w;
	w.show();

	return a.exec();
}
