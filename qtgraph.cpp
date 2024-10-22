
#include <iostream>
#include <QBoxLayout>
#include <QTimer>
#include <cmath>
#include <QPainter>
#include <QButtonGroup>
#include <qpushbutton.h>
#include <rtxi/fifo.hpp>
#include <rtxi/rtos.hpp>
#include "qtgraph.hpp"

#define PI 3.14159

std::unique_ptr<Widgets::Plugin> createRTXIPlugin(Event::Manager* ev_manager)
{
  return std::make_unique<RTDisplay::Plugin>(ev_manager);
}

Widgets::Panel* createRTXIPanel(QMainWindow* main_window,
                                Event::Manager* ev_manager)
{
  return new RTDisplay::Panel(main_window, ev_manager);
}

std::unique_ptr<Widgets::Component> createRTXIComponent(
    Widgets::Plugin* host_plugin)
{
  return std::make_unique<RTDisplay::Component>(host_plugin);
}

Widgets::FactoryMethods fact;

extern "C"
{
Widgets::FactoryMethods* getFactories()
{
  fact.createPanel = &createRTXIPanel;
  fact.createComponent = &createRTXIComponent;
  fact.createPlugin = &createRTXIPlugin;
  return &fact;
}
};



RTDisplay::RTWindow::RTWindow(QWidget* parent, RT::OS::Fifo* fifo) 
  : QOpenGLWidget(parent)
  , timer(new QTimer(this))
  , m_source_fifo(fifo)
{ 
  
  timer->setTimerType(Qt::PreciseTimer);
  timer->start(1000);
  connect(this, &QOpenGLWidget::frameSwapped, this, &RTWindow::updateOnFrame);
  connect(this->timer, &QTimer::timeout, this, &RTWindow::frameCount);
}

void RTDisplay::RTWindow::updateOnFrame()
{
  this->update();
}

void RTDisplay::RTWindow::frameCount()
{
  std::cout << count << " frames/sec\n";
  this->count = 0;
}

void RTDisplay::RTWindow::paintEvent(QPaintEvent*  /*event*/)
{
  bool newdata = false;
  RTDisplay::sample sample {0.0,0.0};
  while(this->m_source_fifo->read(&sample, sizeof(RTDisplay::sample)) > 0){
    newdata = true;
  }
  if(!newdata) { return; }
  //QRectF source(0.0, 0.0, target_width, target_height);
  //QImage image("/home/rtxi/projects/qtgraph/crosshair.jpg");
  
  QPainter painter(this);
  painter.setBrush(QColor(255,255,255,255));
  painter.drawRect(0,0,width(), height());
  painter.setBrush(QColor(255,0,0,255));
  QRect target(sample.xpos, sample.ypos, width()/20, height()/20);
  painter.drawEllipse(target);
  //painter.drawImage(target, image, source); 
  this->count++;
}

RTDisplay::Plugin::Plugin(Event::Manager* ev_manager)
  : Widgets::Plugin(ev_manager, std::string(RTDisplay::MODULE_NAME))
{
}

RT::OS::Fifo* RTDisplay::Plugin::getComponentFifo()
{
  if(getComponent() == nullptr) { return nullptr; }
  auto* component = dynamic_cast<RTDisplay::Component*>(this->getComponent());
  return component->getFifo();
}

RTDisplay::Component::Component(Widgets::Plugin* hplugin)
  : Widgets::Component(hplugin,
                       std::string(RTDisplay::MODULE_NAME),
                       std::vector<IO::channel_t>(),
                       RTDisplay::get_default_vars())
{
  RT::OS::getFifo(this->m_fifo, 10000*sizeof(RTDisplay::sample));
}

void RTDisplay::Component::execute()
{
  RTDisplay::sample sample;
  switch (this->getState()) {
    case RT::State::EXEC:
      this->time+=1.0;
      sample.xpos = this->width/2 + (this->width/2)*sin(this->time*2*PI/60);
      sample.ypos = this->height/2 + (this->height/2)*sin(this->time*2*PI/30);    
      this->m_fifo->writeRT(&sample, sizeof(RTDisplay::sample));
      break;
    case RT::State::MODIFY:
    case RT::State::INIT:
      this->xspeed = getValue<double>(RTDisplay::PARAMETER::X_SPEED);
      this->yspeed = getValue<double>(RTDisplay::PARAMETER::Y_SPEED);
      this->width = getValue<double>(RTDisplay::PARAMETER::WIDTH);
      this->height = getValue<double>(RTDisplay::PARAMETER::HEIGHT);
      setValue(RTDisplay::PARAMETER::STATE, RT::State::EXEC);
      break;
    case RT::State::PERIOD:
    case RT::State::PAUSE:
    case RT::State::UNPAUSE:
    default:
      break;
  }
}

RTDisplay::Panel::Panel(QMainWindow* main_window, Event::Manager* ev_manager)
  : Widgets::Panel(std::string(RTDisplay::MODULE_NAME),
                   main_window,
                   ev_manager)
{
  createGUI(RTDisplay::get_default_vars(), { RTDisplay::PARAMETER::STATE});
  auto* widget_layout = dynamic_cast<QVBoxLayout*>(this->layout());
  auto* buttonGroup = new QButtonGroup();
  auto* show_button = new QPushButton("Show RT Display");
  auto* hide_button = new QPushButton("Hide RT Display");
  buttonGroup->addButton(show_button);
  buttonGroup->addButton(hide_button);
  widget_layout->addWidget(show_button);
  widget_layout->addWidget(hide_button);
  connect(show_button, SIGNAL(clicked()), this, SLOT(showRealTimeDisplay()));
  connect(hide_button, SIGNAL(clicked()), this, SLOT(hideRealTimeDisplay()));
}

void RTDisplay::Panel::showRealTimeDisplay()
{
  if(this->m_realtime_display == nullptr){
    auto* hplugin = dynamic_cast<RTDisplay::Plugin*>(this->getHostPlugin());
    RT::OS::Fifo* fifo = hplugin->getComponentFifo();
    this->m_realtime_display = new RTDisplay::RTWindow(nullptr, fifo);
  }
  m_realtime_display->show();
}

void RTDisplay::Panel::hideRealTimeDisplay()
{
  if(this->m_realtime_display == nullptr) { return; }
  this->m_realtime_display->hide();
}
