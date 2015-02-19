#include "ModControl.hpp"
#include "ModDockUi.hpp"
#include "ModDial.hpp"

#include <QGridLayout>
#include <QStyleOption>
#include <QPainter>
#include <QVBoxLayout>

#include <QDebug>

ModControl::ModControl(const QString& title, QWidget* parent)
: QWidget(parent), title_(title.toUpper())
{
    setupUi();

    QWidget::setFixedSize(100, 200);
}

ModControl::~ModControl()
{ }

void ModControl::setupUi()
{
    QVBoxLayout* layout = new QVBoxLayout(this);

    layout->setSpacing(0);

    layout->setMargin(0);

    //layout->setContentsMargins(0,0,0,0);

    ModDial* dial = new ModDial(title_, 0.01, this);

    layout->addWidget(dial);

    ModDockUi* dock = new ModDockUi(4, 2, this);

    layout->addWidget(dock);

    QWidget::setLayout(layout);
}

void ModControl::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
