#include "OperatorUi.hpp"
#include "Control.hpp"

#include <QGridLayout>
#include <QStyleOption>
#include <QPainter>
#include <QPushButton>
#include <QDial>

namespace OperatorUi
{
    Container::Container(QWidget* parent)
    : QWidget(parent)
    {
        setupUi();
    }

    void Container::setupUi()
    {
        QGridLayout* layout = new QGridLayout(this);

        Control* control = new Control(this);

        layout->addWidget(control);

        QWidget::setLayout(layout);
    }

    void Container::paintEvent(QPaintEvent*)
    {
        QStyleOption opt;
        opt.init(this);
        QPainter p(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    }
}
