#include "additemtoinvoicedialog.h"
#include "ui_additemtoinvoicedialog.h"

AddItemToInvoiceDialog::AddItemToInvoiceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddItemToInvoiceDialog)
{
    ui->setupUi(this);
}

AddItemToInvoiceDialog::~AddItemToInvoiceDialog()
{
    delete ui;
}
QString AddItemToInvoiceDialog::itemName() const
{
    return m_itemName;
}

void AddItemToInvoiceDialog::setItemName(const QString &itemName)
{
    m_itemName = itemName;
}
QString AddItemToInvoiceDialog::itemDescription() const
{
    return m_itemDescription;
}

void AddItemToInvoiceDialog::setItemDescription(const QString &itemDescription)
{
    m_itemDescription = itemDescription;
}
QString AddItemToInvoiceDialog::itemCost() const
{
    return m_itemCost;
}

void AddItemToInvoiceDialog::setItemCost(const QString &itemCost)
{
    m_itemCost = itemCost;
}




void AddItemToInvoiceDialog::on_cmdOK_clicked()
{
    setItemName(ui->txtItemName->text());
    setItemDescription(ui->txtDescription->text());
    setItemCost(ui->txtAmount->text());
    this->accept();
}
