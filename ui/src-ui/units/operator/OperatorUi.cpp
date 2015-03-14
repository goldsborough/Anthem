#include "OperatorUi.hpp"
#include "ModControl.hpp"

#include <QGridLayout>
#include <QStyleOption>
#include <QPainter>


#include <QDebug>



OperatorUi::OperatorUi(QWidget* parent)
    : QWidget(parent)
{
    setupUi();

    setObjectName("OperatorUi");
}

void OperatorUi::setupUi()
{
    QGridLayout* layout = new QGridLayout(this);

	ModControl* level = new ModControl("Level", this);

    connect(level, &ModControl::depthChanged,
            [&] (unsigned short index, double value)
            { qDebug() << index << value << endl; });

    layout->addWidget(level);

	ModControl* offset = new ModControl("Offset", this);

    layout->addWidget(offset, 0, 1);

	ModControl* ratio = new ModControl("Ratio", this);

	layout->addWidget(ratio, 0, 2);

    QWidget::setLayout(layout);
}

void OperatorUi::paintEvent(QPaintEvent*)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
