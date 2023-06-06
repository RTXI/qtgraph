#pragma once

#include <QOpenGLWidget>

class Window : public QOpenGLWidget
{
  Q_OBJECT
public:
  Window(QWidget* parent);
  ~Window() override = default;

  void paintEvent(QPaintEvent* event) override;

public slots:
  void updateOnFrame();
  void frameCount();

private:
  size_t count = 0;
  size_t time = 0;
  int xpos = 0;
  int ypos = 0;
  QTimer* timer=nullptr;

};
