#include "ModControl.hpp"
#include "ModDockUi.hpp"
#include "CustomDial.hpp"

#include <QGridLayout>
#include <QStyleOption>
#include <QPainter>

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
    CustomDial* dial = new CustomDial(title_, 0.01, this);

    ModDockUi* dock = new ModDockUi(4, 2, this);

    dock->move(0, 100);
}

void ModControl::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
