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
        CustomDial* dial = new CustomDial(title_, this);
    }

    void Control::paintEvent(QPaintEvent *)
    {
        QStyleOption opt;
        opt.init(this);
        QPainter p(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    }

}
