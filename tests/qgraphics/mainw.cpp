#include "mainw.h"
#include "ui_mainw.h"



Main::Main(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Main)
{
    ui->setupUi(this);
    scene = new QGraphicsScene(this);

    QGraphicsTextItem *mainHeader = scene->addText("Payment Calendar");


    int sceneWidth = scene->width();

    QPoint pos;
    pos.setX(sceneWidth/2);
    pos.setY(0 - (scene->height() / 2));

    mainHeader->setPos(pos);

    ui->graphicsView->setScene(scene);
}

Main::~Main()
{
    delete ui;
}
