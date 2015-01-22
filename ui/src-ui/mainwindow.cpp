#include "mainwindow.hpp"
#include "menubar.hpp"
#include "container.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    initUI_();
}

void MainWindow::initUI_()
{
    Menubar* menubar = new Menubar(this);

    menubar->move(0,0);

    Container* container = new Container(this);

    container->move(0,128);

    this->setFixedSize(1024,768);
    this->setWindowTitle(tr("Anthem"));
}
