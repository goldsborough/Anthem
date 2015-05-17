#include "PartialsUi.hpp"
#include "ComboBox.hpp"
#include "Plot.hpp"

#include <QToolTip>
#include <QColor>
#include <QPoint>
#include <QString>
#include <QVector>

#include <QDebug>


double PartialsUi::Partial::displayFactor = 100;

double PartialsUi::Partial::baseValue = 0.1;

double PartialsUi::Partial::step = 0.02;

int PartialsUi::Partial::precision = 0;

PartialsUi::Partial::Partial(unsigned short num,
						   QCPAxis *x,
						   QCPAxis *y)
: QCPBars(x, y),
  number(num),
  amplitude(0),
  string(new QString("0"))
{ }

void PartialsUi::Partial::setAmplitude(double amp)
{
	if (amp > 1) amp = 1;

	else if (amp < 0) amp = 0;

	amplitude = amp;

	// Otherwise we get -0
	if (! amp) *string = QString("0");

	else *string = QString::number(amp * displayFactor, 'f', precision);

	QCPBars::setData({number}, {amp + baseValue});
}

void PartialsUi::Partial::up()
{
	setAmplitude(amplitude + step);
}

void PartialsUi::Partial::down()
{
	setAmplitude(amplitude - step);
}


PartialsUi::PartialsUi(QWidget *parent)
: Creator(parent),
  partials_(new QVector<Partial*>(64)),
  lastPosition_(new QPoint),
  barColor_(new QColor),
  activePartial_(nullptr),
  lastShown_(nullptr),
  hasMovedAway_(true),
  sideOffset_(0),
  topOffset_(0),
  barWidth_(10),
  number_(8)
{
	setupMenu();

	setupPlot();
}

void PartialsUi::setupMenu()
{
	Creator::setupMenu();

	auto numberBox = new ComboBox(this);

	numberBox->setToolTip("PartialsUi");

	numberBox->setSizePolicy(QSizePolicy::Maximum,
							QSizePolicy::Expanding);

	numberBox->setCursor(Qt::PointingHandCursor);

	numberBox->addItems({"8", "16", "32", "64"});

	connect(numberBox, &ComboBox::currentChanged,
			[=] (const QString& text)
	{
		number_ = text.toInt();

		double factor = 8.0 / number_;

		for (auto& p : *partials_) p->setWidth(barWidth_ * factor);

		plot_->xAxis->setRangeUpper(sideOffset_ + number_);

		plot_->replot();
	});


	menu_->addWidget(numberBox);
}

void PartialsUi::setupPlot()
{
	setSideOffset(0.5);

	setTopOffset(0.1);

	double x = 1;

	for (auto& partial : *partials_)
	{
		partial = new Partial(x, plot_->xAxis, plot_->yAxis);

		partial->addData({x++}, {Partial::baseValue});

		plot_->addPlottable(partial);

		// Sets width to be in pixels and not graph coordinates
		partial->setWidthType(QCPBars::WidthType::wtAbsolute);
	}

	connect(plot_, &Plot::mouseDoubleClick,
			[=] (QMouseEvent* event)
	{
		auto object = plot_->plottableAt(event->pos());

		// Reset only the partial that was double clicked
		if (object) dynamic_cast<Partial*>(object)->setAmplitude(0);

		// Reset all partials
		else for (auto& p : *partials_) p->setAmplitude(0);
	});

	connect(plot_, &Plot::mousePress,
			[=] (QMouseEvent* event)
	{
		*lastPosition_ = event->pos();

		activePartial_ = dynamic_cast<Partial*>(plot_->plottableAt(event->pos()));
	});

	connect(plot_, &Plot::mouseRelease,
			[=] (QMouseEvent*)
	{
		activePartial_ = nullptr;

		Creator::setCursor(Qt::ArrowCursor);
	});

	connect(plot_, &Plot::mouseMove,
			this, &PartialsUi::handleMouseMove);
}

void PartialsUi::handleMouseMove(QMouseEvent *event)
{
	// activePartial_ is set to nullptr when the
	// mouse is released so we can assume that
	// event->buttons() == Qt::LeftButton and
	// don't need the check.
	if (activePartial_)
	{
		double distance = event->pos().y() - lastPosition_->y();

		if (distance > 0 && activePartial_->amplitude > 0)
		{
			activePartial_->down();
		}

		else if (distance < 0 && activePartial_->amplitude < 1)
		{
			activePartial_->up();
		}

		plot_->replot();

		*lastPosition_ = event->pos();

		Creator::setCursor(Qt::ClosedHandCursor);

		QToolTip::showText(event->globalPos(), *activePartial_->string);
	}

	else if (plot_->plottableAt(event->pos()))
	{
		Creator::setCursor(Qt::OpenHandCursor);

		auto partial = dynamic_cast<Partial*>(plot_->plottableAt(event->pos()));

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
		Creator::setCursor(Qt::ArrowCursor);

		hasMovedAway_ = true;
	}
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

void PartialsUi::setStep(double step)
{
	Partial::step = step / Partial::displayFactor;
}

double PartialsUi::getStep()
{
	return Partial::step * Partial::displayFactor;
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

	for (auto& partial : *partials_)
	{
		if (partial->amplitude == Partial::baseValue)
		{
			partial->amplitude = value;
		}
	}

	Partial::baseValue = value;
}

double PartialsUi::getBaseValue()
{
	return Partial::baseValue * Partial::displayFactor;
}

void PartialsUi::setSideOffset(double offset)
{
	sideOffset_ = offset;

	plot_->xAxis->setRange(offset, number_ + offset);
}

double PartialsUi::getSideOffset() const
{
	return sideOffset_;
}

void PartialsUi::setTopOffset(double offset)
{
	topOffset_ = offset / Partial::displayFactor;

	plot_->yAxis->setRange(0, Partial::baseValue + 1 + topOffset_);
}

double PartialsUi::getTopOffset() const
{
	return topOffset_ * Partial::displayFactor;
}


void PartialsUi::paintEvent(QPaintEvent*)
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
