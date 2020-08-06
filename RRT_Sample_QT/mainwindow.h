#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//Qt
#include <QMainWindow>
#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <QMouseEvent>
#include <QMessageBox>

//OpenCV
#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/core.hpp>
#include <opencv4/opencv2/calib3d/calib3d.hpp>
#include <opencv4/opencv2/highgui/highgui.hpp>
#include <opencv4/opencv2/imgproc/imgproc.hpp>

//OUTROS
#include <iostream>
#include <vector>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "GrafoRRT.h"

using namespace std;
using namespace cv;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_GoRRT_clicked();
    void Mouse_current_pos();
    void Mouse_pressed();


    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;

    string _mapa;
    Mat mapa_show;
    Mat mapa_operate;
    rrt_graph *rrt;
    Point init;
    Point goal;

    bool chv1 = false;
    bool chv2 = false;

    void update_map(Mat img);

    void RRT();
};
#endif // MAINWINDOW_H
