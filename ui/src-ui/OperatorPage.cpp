#include "OperatorPage.hpp"
#include "OperatorUi.hpp"

#include <QGridLayout>
#include <QStyleOption>
#include <QPainter>
#include <QPushButton>

OperatorPage::OperatorPage(QWidget* parent)
: QWidget(parent)
{
    QWidget::setObjectName("ContainerPage");

    setupUi();
}

void OperatorPage::setupUi()
{
    QGridLayout* layout = new QGridLayout(this);

    QList<OperatorUi*> operators;

    for(short i = 0; i < 4; ++i)
    { operators.append(new OperatorUi(this)); }

    layout->addWidget(operators[0], 0, 0);

    layout->addWidget(operators[1], 0, 1);

    layout->addWidget(operators[2], 1, 0);

    layout->addWidget(operators[3], 1, 1);

    QWidget::setLayout(layout);
}

void OperatorPage::paintEvent(QPaintEvent*)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
