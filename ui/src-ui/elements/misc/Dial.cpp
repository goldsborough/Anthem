#include "Dial.hpp"

#include <QPainter>
#include <QColor>
#include <QLabel>
#include <QRectF>
#include <QPen>
#include <QResizeEvent>


#include <QDebug>


Dial::Dial(QWidget* parent)
: QDial(parent)
{ }

Dial::Dial(const QString& text,
					   QWidget* parent,
                       double factor,
					   int minimum,
					   int maximum,
					   bool baseTwo)

: QDial(parent),
  valueShown_(false),
  baseTwo_(baseTwo),
  factor_(factor),
  text_(text),
  arcRect_(new QRectF),
  valueRect_(new QRectF),
  textRect_(new QRectF),
  arcColor_(new QColor),
  arcPen_(new QPen)

{
	QDial::setRange(minimum, maximum);

	setupUi();
}

Dial::~Dial() = default;

void Dial::setupUi()
{
	QDial::setCursor(Qt::PointingHandCursor);

	connect(this, &QDial::valueChanged,
			this, &Dial::updateValue);

	setMinimumSize(100,100);

	setMaximumAngle(-360);

	setStartAngle(270);

	updateValue();
}

void Dial::paintEvent(QPaintEvent*)
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

	painter.drawText(*textRect_, Qt::AlignCenter, text_);

	if (valueShown_)
	{
		painter.drawText(*valueRect_, Qt::AlignCenter, valueString_);
	}

    painter.setPen(*arcPen_);

    painter.drawArc(*arcRect_, startAngle_, angleSpan_);

}

void Dial::resizeEvent(QResizeEvent* event)
{
	QDial::setMinimumSize(event->size());

	double width = QDial::width() - (2 * arcWidth_);

	double height = (valueShown_) ? (width / 2) : width;

	*arcRect_ = QRectF(arcWidth_ / 2,
					   arcWidth_ / 2,
					   width, width);

	*textRect_ = QRectF(arcWidth_,
						arcWidth_,
						width,
						height);

	if (valueShown_)
	{
		*valueRect_ = QRectF(textRect_->left(),
							 textRect_->bottom(),
							 width, height);
	}
}

void Dial::updateValue()
{
	int value = QDial::value();

	// Get ratio between current value and maximum to calculate angle
	double ratio = (static_cast<double>(value) - QDial::minimum()) /
				   (QDial::maximum() - QDial::minimum());

	angleSpan_ = maximumAngleSpan_ * ratio;

	if (baseTwo_) value = 0x01 << value;

	valueString_ = QString::number(value);

	emit scaledValueChanged(value * factor_);
}

void Dial::setArcWidth(double px)
{
    arcWidth_ = px;

	*arcRect_ = QRectF(arcWidth_ / 2,
					   arcWidth_ / 2,
					   QDial::width() - arcWidth_,
					   QDial::height() - arcWidth_);

    arcPen_->setWidth(arcWidth_);
}

void Dial::setBaseTwo(bool state)
{
	baseTwo_ = state;
}

bool Dial::isBaseTwo() const
{
	return baseTwo_;
}

void Dial::setValueShown(bool state)
{
	valueShown_ = state;

	QDial::update();
}

bool Dial::valueIsShown() const
{
	return valueShown_;
}

double Dial::getScaledValue() const
{
	return QDial::value() * factor_;
}

void Dial::setFactor(double factor)
{
    factor_ = factor;
}

double Dial::getFactor() const
{
    return factor_;
}

void Dial::setText(const QString& text)
{
    text_ = text;
}

QString Dial::getText() const
{
    return text_;
}

double Dial::getArcWidth() const
{
    return arcWidth_;
}

void Dial::setMaximumAngle(double angle)
{
	maximumAngleSpan_ = angle * 16;

	endAngle_ = startAngle_ + maximumAngleSpan_;
}

double Dial::getMaximumAngle() const
{
	return maximumAngleSpan_ / 16;
}

void Dial::setStartAngle(double angle)
{
    startAngle_ = angle * 16;

	endAngle_ = startAngle_ + maximumAngleSpan_;
}

double Dial::getStartAngle() const
{
    return startAngle_ / 16;
}

void Dial::setArcColor(const QColor& color)
{
	*arcColor_ = color;

    arcPen_->setColor(*arcColor_);
}

QColor Dial::getArcColor() const
{
	return *arcColor_;
}
