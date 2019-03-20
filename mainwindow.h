#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "harris.h"
#include <QPushButton>

using namespace cv;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void displayFrame();

private:
    Ui::MainWindow *ui;

    Mat image;

    harris *theIm;

    harris *detect;
    harris *getCornerMap;
    harris *getCorners;
    harris *drawOnImage;
    harris *doCorners;
    harris *compare;




};

#endif // MAINWINDOW_H
