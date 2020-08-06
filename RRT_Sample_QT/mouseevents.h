#ifndef MOUSEEVENTS_H
#define MOUSEEVENTS_H

#include <QLabel>
#include <QMouseEvent>
#include <QDebug>

//OpenCV
#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/core.hpp>

class mouseevents : public QLabel
{
    Q_OBJECT
public:
    explicit mouseevents(QWidget *parent = 0);

    void mouseMoveEvent(QMouseEvent *ev) override;
    void mousePressEvent(QMouseEvent *ev) override;

    double x,y, x_press, y_press;



signals:

    void Mouse_pos();
    void Mouse_pressed();

};

#endif // MOUSEEVENTS_H
