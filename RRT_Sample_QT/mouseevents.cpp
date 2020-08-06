#include "mouseevents.h"

using namespace cv;

mouseevents::mouseevents(QWidget *parent) :
     QLabel(parent)
{

}

void mouseevents::mouseMoveEvent(QMouseEvent *ev)
{
    this->x = ev->x();
    this->y = ev->y();

    emit Mouse_pos();
}

void mouseevents::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::LeftButton)
        {
            x_press = ev->localPos().x();
            y_press = ev->localPos().y();

            emit Mouse_pressed();

        }
}
