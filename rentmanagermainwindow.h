#ifndef RENTMANAGERMAINWINDOW_H
#define RENTMANAGERMAINWINDOW_H

#include <QMainWindow>
#include <QtSql>

class QActionGroup;

namespace Ui {
class RentManagerMainWindow;
}

class RentManagerMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit RentManagerMainWindow(QWidget *parent = 0);
	~RentManagerMainWindow();
	static RentManagerMainWindow *instance();

	void showQueryError(QSqlQuery qu, QString title = "Error", QString textBefore = "Ther following error has occured:", QString textAfter = "Please contact your administrator.");

public slots:
	void startOpenFile();
	void closeFile();
private:
	Ui::RentManagerMainWindow *ui;
	QSqlDatabase db;
	QString currentFilePath;
	void loadFile(const QString &fileName);
	bool saveFile(const QString &fileName);
	void setCurrentFile(const QString &fileName);
	QString strippedName(const QString &fullFileName);
	QString curFile;
	QString monthName;
	QString yearName;
	void updateRecentFileActions();
	enum { MaxRecentFiles = 5 };
	QAction *recentFileActs[MaxRecentFiles];
	QActionGroup *actionsToDisable;
	void initializeCompanyFile();
	static RentManagerMainWindow *m_instance;
	//Dialogs

private slots:
	void openRecentFile();
	void startNewFile();

};

#endif // RENTMANAGERMAINWINDOW_H
