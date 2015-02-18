#include "AnthemUi.hpp"
#include "Menubar.hpp"
#include "OperatorPage.hpp"

#include <QTabWidget>
#include <QTabBar>
#include <QVBoxLayout>

AnthemUi::AnthemUi(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi();
}

void AnthemUi::setupUi()
{
    QVBoxLayout* layout = new QVBoxLayout(this);

    layout->setMargin(0);


    Menubar* menubar = new Menubar(this);

    layout->addWidget(menubar);


    QTabWidget* pages = new QTabWidget(this);

    pages->tabBar()->setCursor(Qt::PointingHandCursor);

    layout->addWidget(pages);



    QWidget* lfoPage = new QWidget;

    lfoPage->setObjectName("ContainerPage");

    pages->addTab(lfoPage, "LFO");


    pages->addTab(new OperatorPage::ContainerPage(), "Operator");


    QWidget* masterPage = new QWidget;

    masterPage->setObjectName("ContainerPage");

    pages->addTab(masterPage, "Master");


    QWidget* effectPage = new QWidget;

    effectPage->setObjectName("ContainerPage");

    pages->addTab(effectPage, "Effect");


    QWidget* envelopePage = new QWidget;

    envelopePage->setObjectName("ContainerPage");

    pages->addTab(envelopePage, "Envelope");

    QWidget* container = new QWidget(this);

    container->setLayout(layout);

    QMainWindow::setCentralWidget(container);


    QMainWindow::setWindowTitle("Anthem");
}
