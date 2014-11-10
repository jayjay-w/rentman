#ifndef PROPERTIESDIALOG_H
#define PROPERTIESDIALOG_H

#include <QDialog>

class QTreeWidgetItem;

namespace Ui {
class PropertiesDialog;
}

class PropertiesDialog : public QDialog
{
	Q_OBJECT

public:
	explicit PropertiesDialog(QWidget *parent = 0);
	~PropertiesDialog();
	void publicEdit(QString id);
public slots:
	void reloadProperties();
	void editProperty();

private slots:
	void newProperty();
	void saveChanges();
	void deleteProperty();
	void itemChanged(QTreeWidgetItem *item, int column);
private:
	Ui::PropertiesDialog *ui;
	QString m_currentProperty;
};

#endif // PROPERTIESDIALOG_H
