#include "Menubar.hpp"
#include "Projectbar.hpp"

#include <QLabel>

Menubar::Menubar(QWidget* parent)
    : QWidget(parent)
{
    initUI_();
}

void Menubar::initUI_()
{
    projectBar_ = new Projectbar(this);
    projectBar_->move(312,16);

    this->setFixedSize(1024,128);
}
