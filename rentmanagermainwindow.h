#ifndef RENTMANAGERMAINWINDOW_H
#define RENTMANAGERMAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class RentManagerMainWindow;
}

class RentManagerMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit RentManagerMainWindow(QWidget *parent = 0);
	~RentManagerMainWindow();

private:
	Ui::RentManagerMainWindow *ui;
};

#endif // RENTMANAGERMAINWINDOW_H
