#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    theIm = new harris();
    QTimer *qTimer = new QTimer(this);
    connect(qTimer, SIGNAL(timeout()),this, SLOT(displayFrame()));
    qTimer->start(10);

    ui->imDisplay->setScaledContents(true);
    ui->imDisplay2->setScaledContents(true);
    ui->imDisplay3->setScaledContents(true);

    detect = new harris();
    getCornerMap = new harris();
    getCorners = new harris();
    drawOnImage = new harris();
    doCorners = new harris();
    compare = new harris();


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::displayFrame(){

    Mat image1 = imread("/home/paul/Desktop/Comp 490/HarrisCorners/test.JPG",1);
    Mat image2 = imread("/home/paul/Desktop/Comp 490/HarrisCorners/test2.png",1);

    compare->compare(image1,image2);
    compare->displayImage(*ui->imDisplay);
    compare->displayImage2(*ui->imDisplay2);
    compare->displayImage3(*ui->imDisplay3);


}
