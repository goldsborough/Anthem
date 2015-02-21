#include "ModItemUi.hpp"
#include "ModUnitUi.hpp"

#include <QPainter>
#include <QPaintEvent>

#include <QDebug>

ModItemUi::ModItemUi(QWidget* parent)
: ModItemUi(ModUnitUi(), parent)
{ }

ModItemUi::ModItemUi(const ModUnitUi& mod,
                         QWidget* parent)
: QAbstractSlider(parent), mod_(new ModUnitUi(mod)),
  borderPen_(new QPen), borderWidth_(0),
  borders_(new QLineF [4], [&] (QLineF lines []) { delete [] lines; } ),
  ratios_(4)
{
    QAbstractSlider::setFixedSize(40, 40);

    QAbstractSlider::setMouseTracking(true);

    QAbstractSlider::setRange(0,999);

    connect(this, &QAbstractSlider::valueChanged,
            [=] (int value) { emit depthChanged(value * 0.001); });

    setBorderRatios(1,1,1,1);
}

void ModItemUi::paintEvent(QPaintEvent*)
{
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);

    for (unsigned short border = LEFT; border <= BOTTOM; ++border)
    {
        borderPen_->setWidthF(borderWidth_ * ratios_[border]);

        painter.setPen(*borderPen_);

        painter.drawLine(borders_.data()[border]);
    }

    painter.drawText(QAbstractSlider::rect(),
                     Qt::AlignCenter,
                     mod_->text);
}

void ModItemUi::setBorderRatios(double left,
                                double right,
                                double top,
                                double bottom)
{
    ratios_[LEFT] = left;

    ratios_[RIGHT] = right;

    ratios_[TOP] = top;

    ratios_[BOTTOM] = bottom;
}

void ModItemUi::setBorderRatio(Side side, double ratio)
{
    ratios_[side] = ratio;
}

double ModItemUi::getBorderRatio(Side side) const
{
    return ratios_[side];
}

void ModItemUi::setBorderWidth(double width)
{
    borderWidth_ = width;
}

double ModItemUi::getBorderWidth() const
{
    return borderWidth_;
}

void ModItemUi::mouseMoveEvent(QMouseEvent* event)
{
    if (QAbstractSlider::underMouse())
    {
        emit itemHovered();
    }
}

void ModItemUi::resizeEvent(QResizeEvent* event)
{
    QAbstractSlider::resizeEvent(event);

    borders_.data()[LEFT] = QLineF(0,
                                   0,
                                   0,
                                   QAbstractSlider::height());

    borders_.data()[TOP] = QLineF(0,
                                  0,
                                  QAbstractSlider::width(),
                                  0);

    borders_.data()[RIGHT] = QLineF(QAbstractSlider::width(),
                                    0,
                                    QAbstractSlider::width(),
                                    QAbstractSlider::height());


    borders_.data()[BOTTOM] = QLineF(0,
                                     QAbstractSlider::height(),
                                     QAbstractSlider::width(),
                                     QAbstractSlider::height());
}

void ModItemUi::setBorderColor(const QColor& color)
{
    borderPen_->setColor(color);
}

QColor ModItemUi::getBorderColor() const
{
    return borderPen_->color();
}

void ModItemUi::setModUnitUi(const ModUnitUi& mod)
{
    mod_.reset(new ModUnitUi(mod));

    QAbstractSlider::repaint();

    emit modUnitChanged(mod);
}

ModUnitUi ModItemUi::getModUnitUi() const
{
    return *mod_;
}
