#ifndef RENTMANAGERMAINWINDOW_H
#define RENTMANAGERMAINWINDOW_H

#include <QMainWindow>
#include <QtSql>

class QActionGroup;
class CompaniesDialog;
class PropertiesDialog;

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
	void reloadBrowser();
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
	bool initializeCompanyFile();
	static RentManagerMainWindow *m_instance;
	//Dialogs
	CompaniesDialog *m_companiesDialog;
	PropertiesDialog *m_propertiesDialog;
private slots:
	void openRecentFile();
	void startNewFile();

	void on_actionCompanies_triggered();
	void on_actionProperties_triggered();
};

#endif // RENTMANAGERMAINWINDOW_H
