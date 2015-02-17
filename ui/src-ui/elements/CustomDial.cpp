#include "CustomDial.hpp"

#include <QPainter>
#include <QColor>
#include <QLabel>
#include <QRectF>
#include <QPen>

#include <QDebug>

CustomDial::CustomDial(const QString& text,
                       double factor,
                       QWidget* parent)

: QDial(parent), factor_(factor), text_(text),
  arcRect_(new QRectF), valueRect_(new QRectF),
  textRect_(new QRectF), arcColor_(new QColor),
  arcPen_(new QPen)

{
    QDial::setRange(0,999);

    QDial::setCursor(Qt::PointingHandCursor);

    connect(this, &QDial::valueChanged,
            this, &CustomDial::updateValue);

    setFixedSize(100,100);

    setMaximumAngle(-360);

    setStartAngle(270);

    setArcWidth(3);

    updateValue();
}

CustomDial::~CustomDial()
{ }

void CustomDial::paintEvent(QPaintEvent*)
{
    QPainter painter(this);

    // So that we can use the background color
    painter.setBackgroundMode(Qt::OpaqueMode);

    // Smooth out the circle
    painter.setRenderHint(QPainter::Antialiasing);

    // Use background color
    painter.setBrush(painter.background());

    // Get current pen before resetting so we have
    // access to the color() method which returns the
    // color from the sytlesheet
    QPen textPen = painter.pen();

    // No border
    painter.setPen(QPen(Qt::NoPen));

    // Draw background circle
    painter.drawEllipse(QDial::rect());

    painter.setPen(textPen);

    painter.drawText(*textRect_, Qt::AlignHCenter | Qt::AlignBottom, text_);

    painter.drawText(*valueRect_, Qt::AlignCenter, valueString_);

    painter.setPen(*arcPen_);

    painter.drawArc(*arcRect_, startAngle_, angleSpan_);

}

void CustomDial::setFixedSize(int w, int h)
{
    QDial::setFixedSize(w, h);

    double width = QDial::width() - 10;

    double height = width / 2;

    *textRect_ = QRectF(5, 5, width, height);

    *valueRect_ = QRectF(5, height, width, height);
}

void CustomDial::updateValue()
{
    double value = QDial::value();

    // Get ratio between current value and maximum to calculate angle
    double ratio = value / QDial::maximum();

    angleSpan_ = maximumAngle_ * ratio;

    valueString_ = QString::number(value);

    emit scaledValueChanged(value * factor_);
}

void CustomDial::setArcWidth(double px)
{
    arcWidth_ = px;

    *arcRect_ = QRectF(arcWidth_ / 2,
                       arcWidth_ / 2,
                       QDial::width() - arcWidth_,
                       QDial::height() - arcWidth_);

    arcPen_->setWidth(arcWidth_);
}

void CustomDial::setFactor(double factor)
{
    factor_ = factor;
}

double CustomDial::getFactor() const
{
    return factor_;
}

void CustomDial::setText(const QString& text)
{
    text_ = text;
}

QString CustomDial::getText() const
{
    return text_;
}

double CustomDial::getArcWidth() const
{
    return arcWidth_;
}

void CustomDial::setMaximumAngle(double angle)
{
    maximumAngle_ = angle * 16;
}

double CustomDial::getMaximumAngle() const
{
    return maximumAngle_ / 16;
}

void CustomDial::setStartAngle(double angle)
{
    startAngle_ = angle * 16;
}

double CustomDial::getStartAngle() const
{
    return startAngle_ / 16;
}

void CustomDial::setArcColor(const QString& color)
{
    arcColor_->setNamedColor(color);

    arcPen_->setColor(*arcColor_);
}

QString CustomDial::getArcColor() const
{
    return arcColor_->name();
}
