#include "Menubar.hpp"
#include "Projectbar.hpp"

#include <QLabel>
#include <QIcon>

Menubar::Menubar(QWidget* parent)
    : QWidget(parent)
{
    setupUi();
}

void Menubar::setupUi()
{
    projectBar_ = new Projectbar(this);
    projectBar_->move(312,16);

    QLabel* dockA = new QLabel(this);

    dockA->setPixmap(QIcon(":/icons/dock-a.png").pixmap(80,80));

    dockA->setCursor(Qt::OpenHandCursor);

    dockA->move(16,16);

    this->setFixedSize(1024,128);
}
