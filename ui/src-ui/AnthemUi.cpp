#include "AnthemUi.hpp"
#include "Menubar.hpp"
#include "OperatorPage.hpp"

#include <QTabWidget>
#include <QTabBar>

AnthemUi::AnthemUi(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi();
}

void AnthemUi::setupUi()
{
    Menubar* menubar = new Menubar(this);

    menubar->move(0,0);

    QTabWidget* container = new QTabWidget(this);


    QWidget* lfoPage = new QWidget;

    lfoPage->setObjectName("ContainerPage");

    container->addTab(lfoPage, "LFO");


    container->addTab(new OperatorPage::ContainerPage(), "Operator");


    QWidget* masterPage = new QWidget;

    masterPage->setObjectName("ContainerPage");

    container->addTab(masterPage, "Master");


    QWidget* effectPage = new QWidget;

    effectPage->setObjectName("ContainerPage");

    container->addTab(effectPage, "Effect");


    QWidget* envelopePage = new QWidget;

    envelopePage->setObjectName("ContainerPage");

    container->addTab(envelopePage, "Envelope");


    container->tabBar()->setCursor(Qt::PointingHandCursor);

    container->setFixedSize(1024,640);

    container->move(0,128);


    QMainWindow::setFixedSize(1024,768);
    QMainWindow::setWindowTitle("Anthem");
}
