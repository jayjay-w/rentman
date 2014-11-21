#ifndef ADDITEMTOINVOICEDIALOG_H
#define ADDITEMTOINVOICEDIALOG_H

#include <QDialog>

namespace Ui {
class AddItemToInvoiceDialog;
}

class AddItemToInvoiceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddItemToInvoiceDialog(QWidget *parent = 0);
    ~AddItemToInvoiceDialog();

    QString itemName() const;
    void setItemName(const QString &itemName);

    QString itemDescription() const;
    void setItemDescription(const QString &itemDescription);

    QString itemCost() const;
    void setItemCost(const QString &itemCost);

private slots:
    void on_cmdOK_clicked();

private:
    Ui::AddItemToInvoiceDialog *ui;

    QString m_itemName;
    QString m_itemDescription;
    QString m_itemCost;
};

#endif // ADDITEMTOINVOICEDIALOG_H
