#include "PartialsUi.hpp"

#include <QToolTip>
#include <QColor>
#include <QPoint>
#include <QString>
#include <QVector>

#include <QDebug>

QSharedPointer<QCPRange> PartialsUi::Partial::zero(new QCPRange);

double PartialsUi::Partial::displayFactor = 100;

double PartialsUi::Partial::baseValue = 0.1;

int PartialsUi::Partial::precision = 0;

double PartialsUi::Partial::range = 0;

PartialsUi::Partial::Partial(unsigned short num,
						   QCPAxis *x,
						   QCPAxis *y)
: QCPBars(x, y),
  number(num),
  string(new QString("0"))
{
	setAmplitude(0);
}

void PartialsUi::Partial::setAmplitude(double amp)
{	
	if (amp > 1) amp = 1;

	else if (amp < -1) amp = -1;

	amplitude = amp;

	*string = QString::number(amp * displayFactor, 'f', precision);

	if (amp >= 0) amp += baseValue;

	else amp -= baseValue;

	QCPBars::setData({number}, {amp});
}

void PartialsUi::Partial::setAmplitude(const QPoint& pos)
{
	if (pos.y() <= zero->lower || pos.y() >= zero->upper)
	{
		double y = (pos.y() <= zero->lower) ? zero->lower - pos.y() :
											  zero->upper - pos.y();

		setAmplitude(y / range);
	}
}

void PartialsUi::Partial::updateRange()
{
	zero->upper = QCPBars::coordsToPixels(number, -baseValue).y();

	zero->lower = range = QCPBars::coordsToPixels(number, baseValue).y();

	range -= QCPBars::coordsToPixels(number, baseValue + 1).y();
}

PartialsUi::PartialsUi(QWidget* parent)
: Plot(parent),
  partials_(new QVector<Partial*>(64)),
  barColor_(new QColor),
  activePartial_(nullptr),
  lastShown_(nullptr),
  hasMovedAway_(true),
  sideOffset_(0),
  barWidth_(10),
  number_(8)
{
	setSideOffset(0.5);

	double x = 1;

	for (auto& partial : *partials_)
	{
		partial = new Partial(x++, Plot::xAxis, Plot::yAxis);

		Plot::addPlottable(partial);

		// Sets width to be in pixels and not graph coordinates
		partial->setWidthType(QCPBars::WidthType::wtAbsolute);
	}

	Plot::yAxis->setRange(-1, 1);

	setupConnections();
}

void PartialsUi::setupConnections()
{
	connect(this, &Plot::mouseDoubleClick,
			[=] (QMouseEvent* event)
	{
		auto object = Plot::plottableAt(event->pos());

		// Reset only the partial that was double clicked
		if (object) dynamic_cast<Partial*>(object)->setAmplitude(0);

		// Reset all partials
		else for (auto& p : *partials_) p->setAmplitude(0);
	});

	connect(this, &Plot::mousePress,
			[=] (QMouseEvent* event)
	{
		activePartial_ = dynamic_cast<Partial*>(Plot::plottableAt(event->pos()));

		Plot::setCursor(Qt::ClosedHandCursor);
	});

	connect(this, &Plot::mouseRelease,
			[=] (QMouseEvent*)
	{
		activePartial_ = nullptr;

		Plot::setCursor(Qt::ArrowCursor);
	});

	connect(this, &Plot::mouseMove,
			this, &PartialsUi::handleMouseMove);
}

void PartialsUi::resizeEvent(QResizeEvent *event)
{
	Plot::resizeEvent(event);

	partials_->front()->updateRange();
}

void PartialsUi::handleMouseMove(QMouseEvent *event)
{
	// activePartial_ is set to nullptr when the
	// mouse is released so we can assume that
	// event->buttons() == Qt::LeftButton and
	// don't need the check.
	if (activePartial_)
	{	
		activePartial_->setAmplitude(event->pos());

		Plot::replot();

		QToolTip::showText(event->globalPos(), *activePartial_->string);
	}

	else if (Plot::plottableAt(event->pos()))
	{
		Plot::setCursor(Qt::OpenHandCursor);

		auto partial = dynamic_cast<Partial*>(Plot::plottableAt(event->pos()));

		// The tool tip should only be shown once for each
		// hover, so we check here if the partial currently
		// hovered is the same as the last one hovered and if
		// it is, then check if the cursor has since moved away
		// Otherwise the tool tip will show repedeatedly if the
		// user stops the cursor over a partial (first show) and
		// then moves it again over the same partial (second show)
		// which is ugly and weird
		if (partial != lastShown_ || hasMovedAway_)
		{
			QToolTip::showText(event->globalPos(),
							   *partial->string,
							   this,
							   { },
							   500);

			hasMovedAway_ = false;

			lastShown_ = partial;
		}
	}

	else
	{
		Plot::setCursor(Qt::ArrowCursor);

		hasMovedAway_ = true;
	}
}

void PartialsUi::setNumberOfPartials(int number)
{
	number_ = number;

	double factor = 8.0 / number_;

	for (auto& p : *partials_) p->setWidth(barWidth_ * factor);

	Plot::xAxis->setRangeUpper(sideOffset_ + number_);

	Plot::replot();
}

int PartialsUi::getNumberOfPartials() const
{
	return number_;
}

void PartialsUi::setBarColor(const QColor& color)
{
	*barColor_ = color;

	for (auto& p : *partials_)
	{
		p->setBrush({color});

		p->setSelectedBrush({color});
	}
}

QColor PartialsUi::getBarColor() const
{
	return *barColor_;
}

void PartialsUi::setBarWidth(double width)
{
	barWidth_ = width;

	for(auto& p : *partials_) p->setWidth(width);
}

double PartialsUi::getBarWidth() const
{
	return barWidth_;
}


void PartialsUi::setPrecision(int precision)
{
	Partial::precision = precision;
}

int PartialsUi::getPrecision()
{
	return Partial::precision;
}

void PartialsUi::setDisplayFactor(double factor)
{
	Partial::displayFactor = factor;
}

double PartialsUi::getDisplayFactor()
{
	return Partial::displayFactor;
}

void PartialsUi::setBaseValue(double value)
{
	value /= Partial::displayFactor;

	qDebug() << value;

	for (auto& partial : *partials_)
	{
		if (partial->amplitude == Partial::baseValue)
		{
			partial->amplitude = value;
		}
	}

	Partial::baseValue = value;

	partials_->front()->updateRange();
}

double PartialsUi::getBaseValue()
{
	return Partial::baseValue * Partial::displayFactor;
}

void PartialsUi::setSideOffset(double offset)
{
	sideOffset_ = offset;

	Plot::xAxis->setRange(offset, number_ + offset);
}

double PartialsUi::getSideOffset() const
{
	return sideOffset_;
}

void PartialsUi::setPadding(double padding)
{
	padding_ = padding / Partial::displayFactor;

	auto range = Partial::baseValue + 1 + padding_;

	Plot::yAxis->setRange(-range, range);
}

double PartialsUi::getPadding() const
{
	return padding_ * Partial::displayFactor;
}
