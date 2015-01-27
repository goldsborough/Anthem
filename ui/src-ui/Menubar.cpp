#include "Menubar.hpp"
#include "Projectbar.hpp"
#include "CopyDock.hpp"

#include <QLabel>
#include <QIcon>
#include <QPushButton>
#include <QDebug>

Menubar::Menubar(QWidget* parent)
    : QWidget(parent)
{
    setupUi();
}

void Menubar::setupUi()
{
    projectBar_ = new Projectbar(this);
    projectBar_->move(312,16);

    CopyDock* dockA = new CopyDock("A", this);

    dockA->move(16,16);

    CopyDock* dockB = new CopyDock("B", this);

    dockB->move(114,16);

    QPushButton* button = new QPushButton("Press",this);

    connect(button, &QPushButton::clicked,
            [=] () { dockA->setEnabled(true); });

    button->move(200,20);

    this->setFixedSize(1024,128);
}
