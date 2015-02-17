#include "ModDockItem.hpp"

#include <QPainter>
#include <QPaintEvent>

#include <QDebug>

ModDockItem::ModDockItem(const QString& text,
                         QWidget* parent)
: QAbstractSlider(parent), text_(text),
  borderWidth_(0), borderPen_(new QPen),
  borders_(new QLineF [4], [&] (QLineF lines []) { delete [] lines; } )
{
    setFixedSize(40, 40);

    QAbstractSlider::setRange(0,999);

    connect(this, &QAbstractSlider::valueChanged,
            [=] (int value) { emit scaledValueChanged(value * 0.001); });

    setBorderRatios(1,1,1,1);
}

void ModDockItem::paintEvent(QPaintEvent*)
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
                     text_);
}

void ModDockItem::setBorderRatios(double left,
                                 double right,
                                 double top,
                                 double bottom)
{
    ratios_[LEFT] = left;

    ratios_[RIGHT] = right;

    ratios_[TOP] = top;

    ratios_[BOTTOM] = bottom;
}

void ModDockItem::setBorderRatio(Side side, double ratio)
{
    ratios_[side] = ratio;
}

double ModDockItem::getBorderRatio(Side side) const
{
    return ratios_[side];
}

void ModDockItem::setBorderWidth(double width)
{
    borderWidth_ = width;
}

double ModDockItem::getBorderWidth() const
{
    return borderWidth_;
}

void ModDockItem::setFixedSize(int w, int h)
{
    setFixedSize(QSize(w, h));
}

void ModDockItem::setFixedSize(const QSize& size)
{
    QAbstractSlider::setFixedSize(size);

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

void ModDockItem::setBorderColor(const QColor& color)
{
    borderPen_->setColor(color);
}

QColor ModDockItem::getBorderColor() const
{
    return borderPen_->color();
}

void ModDockItem::setText(const QString& text)
{
    text_ = text;
}

QString ModDockItem::getText() const
{
    return text_;
}
