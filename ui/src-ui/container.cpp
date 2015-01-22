#include "container.hpp"

#include <QPushButton>

Container::Container(QWidget* parent)
    : QWidget(parent)
{
    initUI_();
}

void Container::initUI_()
{
    this->setFixedSize(1024,650);
}
