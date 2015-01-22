#include "menubar.hpp"
#include "filebar.hpp"

#include <QLabel>

Menubar::Menubar(QWidget* parent)
    : QWidget(parent)
{
    initUI_();
}

void Menubar::initUI_()
{
    filebar_ = new Filebar(this);
    filebar_->move(312,16);

    this->setFixedSize(1024,128);
}
