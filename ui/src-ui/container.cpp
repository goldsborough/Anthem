#include "Container.hpp"

#include <QTabBar>

Container::Container(QWidget* parent)
: QTabWidget(parent)
{
    setupUi();
}

void Container::setupUi()
{
    QTabWidget::addTab(new QWidget(), "LFO");

    QTabWidget::addTab(new QWidget(), "Operator");

    QTabWidget::addTab(new QWidget(), "Master");

    QTabWidget::addTab(new QWidget(), "Effect");

    QTabWidget::addTab(new QWidget(), "Envelope");

    QTabWidget::tabBar()->setCursor(Qt::PointingHandCursor);

    QTabWidget::setFixedSize(1024,640);
}
