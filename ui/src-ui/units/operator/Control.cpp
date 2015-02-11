#include "Control.hpp"
#include "CustomDial.hpp"

#include <QDial>
#include <QTabBar>
#include <QLabel>
#include <QStyleOption>
#include <QPainter>
#include <QPaintEvent>

namespace OperatorUi
{
    Control::Control(const QString& title, QWidget* parent)
        : QWidget(parent), title_(title)
    {
        setupUi();
    }

    void Control::setupUi()
    {
        CustomDial* dial = new CustomDial(title_, 0.01, this);

        QLabel* label = new QLabel(this);

        label->setFixedSize(100,100);

        label->move(0,100);

        QWidget::setFixedSize(200, 300);

        connect(dial, &CustomDial::scaledValueChanged,
                [=] (double value) { label->setText(QString::number(value)); });
    }

    void Control::paintEvent(QPaintEvent *)
    {
        QStyleOption opt;
        opt.init(this);
        QPainter p(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    }

}
