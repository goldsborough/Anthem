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
  borderPen_(new QPen), borders_(4), ratios_(4),
  borderWidth_(0)
{
    for (int i = 0; i < 4; ++i) borders_[i].reset(new QLineF);

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

        painter.drawLine(*(borders_[border]));
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

    unsigned long h = QAbstractSlider::height();

    unsigned long w = QAbstractSlider::width();

    borders_[LEFT]->setLine(0, 0, 0, h);

    borders_[TOP]->setLine(0, 0, w, 0);

    borders_[RIGHT]->setLine(w, 0, w, h);

    borders_[BOTTOM]->setLine(0, h, w, h);
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
