#include <iostream>
#include <string>

#include <QApplication>
#include "lib.hpp"

int main(int argc, char** argv)
{
  auto const message = "Hello!";
  std::cout << message << '\n';
  auto* app = new QApplication(argc, argv);
  auto* win = new Window(nullptr);
  win->show();
  app->exec();
}
