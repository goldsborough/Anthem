#include "OperatorUi.hpp"

#include <QGridLayout>
#include <QStyleOption>
#include <QPainter>
#include <QPushButton>

OperatorUi::OperatorUi(QWidget* parent)
: QWidget(parent)
{
    setupUi();
}

void OperatorUi::setupUi()
{
    QGridLayout* layout = new QGridLayout(this);

    QPushButton* button = new QPushButton("Test", this);

    layout->addWidget(button);

    QWidget::setLayout(layout);
}

void OperatorUi::paintEvent(QPaintEvent*)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
