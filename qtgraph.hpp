#ifndef QTGRAPH_HPP
#define QTGRAPH_HPP

#include <QOpenGLWidget>
#include <rtxi/module.hpp>

namespace RTDisplay 
{

constexpr std::string_view MODULE_NAME = "Real-Time Display";

enum PARAMETER : Modules::Variable::Id
{
  STATE=0,
  X_SPEED,
  Y_SPEED,
  WIDTH,
  HEIGHT,
};

struct sample{
  double xpos=0;
  double ypos=0;
};

inline  std::vector<Modules::Variable::Info> get_default_vars() 
{
  return {
      {PARAMETER::STATE,
       "State",
       "Real-Time display state",
       Modules::Variable::STATE,
       Modules::Variable::INIT},
      {PARAMETER::X_SPEED,
       "X Velocity",
       "X direction oscillation speed",
       Modules::Variable::DOUBLE_PARAMETER,
       1.0},
      {PARAMETER::Y_SPEED,
       "Y Velocity",
       "Y Direction oscillation speed",
       Modules::Variable::DOUBLE_PARAMETER,
       1.0},
      {PARAMETER::WIDTH,
       "Width",
       "Width of the output screen",
       Modules::Variable::DOUBLE_PARAMETER,
       100.0},
      {PARAMETER::HEIGHT,
       "Height",
       "Height of the output screen",
       Modules::Variable::DOUBLE_PARAMETER,
       100.0}
  };
}

class RTWindow : public QOpenGLWidget
{
  Q_OBJECT
public:
  RTWindow(QWidget* parent, RT::OS::Fifo* fifo);

  void paintEvent(QPaintEvent* event) override;

public slots:
  void updateOnFrame();
  void frameCount();

private:
  size_t count = 0;
  int xpos = 0;
  int ypos = 0;
  QTimer* timer=nullptr;
  RT::OS::Fifo* m_source_fifo=nullptr;
};

class Plugin : public Modules::Plugin
{
public:
  explicit Plugin(Event::Manager* ev_manager);  
  RT::OS::Fifo* getComponentFifo();
};

class Component : public Modules::Component
{
public:
  explicit Component(Modules::Plugin* hplugin);
  void execute() override;
  RT::OS::Fifo* getFifo(){ return this->m_fifo.get(); }
private:
  double xspeed=0;
  double yspeed=0;
  double width=0;
  double height=0;
  double time=0;
  std::unique_ptr<RT::OS::Fifo> m_fifo;
};

class Panel : public Modules::Panel
{
  Q_OBJECT
public:
  Panel(QMainWindow* main_window, Event::Manager* ev_manager);

public slots:
  void showRealTimeDisplay();
  void hideRealTimeDisplay();
private:
  RTWindow* m_realtime_display=nullptr;
  RT::OS::Fifo* m_component_fifo=nullptr;
};

} // namespace qtgraph

#endif
