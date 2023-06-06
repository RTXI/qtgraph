
#include <iostream>
#include <qnamespace.h>
#include <QTimer>
#include <cmath>
#include <QPainter>
#include "lib.hpp"

#define PI 3.14159
Window::Window(QWidget* parent) : QOpenGLWidget(parent)
{ 
  this->timer = new QTimer(this);
  timer->setTimerType(Qt::PreciseTimer);
  timer->start(1000);
  connect(this, &QOpenGLWidget::frameSwapped, this, &Window::updateOnFrame);
  connect(this->timer, &QTimer::timeout, this, &Window::frameCount);
}

void Window::updateOnFrame()
{
  this->update();
}

void Window::frameCount()
{
  std::cout << count << " frames/sec\n";
  this->count = 0;
}

void Window::paintEvent(QPaintEvent*  /*event*/)
{
  this->time++;
  this->xpos = width()/2 + (width()/2)*sin(this->time*2*PI/60);
  this->ypos = height()/2 + (height()/2)*sin(this->time*2*PI/30);
  //QRectF source(0.0, 0.0, target_width, target_height);
  //QImage image("/home/rtxi/projects/qtgraph/crosshair.jpg");
  
  QPainter painter(this);
  painter.setBrush(QColor(255,255,255,255));
  painter.drawRect(0,0,width(), height());
  painter.setBrush(QColor(255,0,0,255));
  QRect target(xpos, ypos, width()/20, height()/20);
  painter.drawEllipse(target);
  //painter.drawImage(target, image, source); 
  this->count++;
}
