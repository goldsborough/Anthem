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

        setObjectName("OperatorUi");
    }

    void Container::setupUi()
    {
        QGridLayout* layout = new QGridLayout(this);

        Control* level = new Control("Level", this);

        layout->addWidget(level);

        Control* offset = new Control("Offset", this);

        layout->addWidget(offset, 0, 1);

        Control* ratio = new Control("Ratio", this);

        layout->addWidget(ratio, 0, 2);

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
