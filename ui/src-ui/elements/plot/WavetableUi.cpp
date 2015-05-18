#include "WavetableUi.hpp"

#include <cmath>

namespace Global
{
	const unsigned short wavetableLength = 4095;
};

const double WavetableUi::conversion_ = 360.0 / Global::wavetableLength;

QSharedPointer<QVector<double>> WavetableUi::initializeX()
{
	QSharedPointer<QVector<double>> temp(new QVector<double>(Global::wavetableLength * 3));

	double x = -Global::wavetableLength;

	for (auto& i : *temp) i = x++;

	return temp;
}

QSharedPointer<QVector<double>> WavetableUi::x_ = WavetableUi::initializeX();


WavetableUi::WavetableUi(QWidget *parent,
						 int frequency,
						 bool phaseShiftingEnabled)
: Plot(parent),
  frequency_(frequency),
  id_(new QString),
  y_(new QVector<double>(3 * Global::wavetableLength))
{
	// Also connects signals, besides setting the member
	setPhaseShiftingEnabled(phaseShiftingEnabled);

	Plot::addGraph();

	yAxis->setRange(-1, 1);

	xAxis->setRange(1, Global::wavetableLength);

	Plot::axisRect()->setRangeDrag(Qt::Horizontal);

	// Wow so short amaze such concise
	auto signal = static_cast<void(QCPAxis::*)(const QCPRange&)>(&QCPAxis::rangeChanged);

	// Ensures the maximum or minimum range is not trespassed
	connect(xAxis, signal,
			[=] (const QCPRange& range)
			{
				if (range.lower < -Global::wavetableLength)
				{
					xAxis->setRange(-Global::wavetableLength, 0);
				}

				else if (range.upper > Global::wavetableLength * 2)
				{
					xAxis->setRange(Global::wavetableLength,
									Global::wavetableLength * 2);
				}

				emit phaseChanged(range.lower * conversion_);
			});

	setWavetable(QString());
}

void WavetableUi::setWavetable(const QString &id)
{
	*id_ = id;

	Plot::setToolTip(id);

	// Get data from WavetableDatabase

	const double twoPi = frequency_ * 2 * M_PI;

	double phase = 0;

	double increment = twoPi / Global::wavetableLength;

	for (auto& i : *y_)
	{
		i = std::sin(phase);

		phase += increment;

		if (phase >= twoPi) phase -= twoPi;
	}

	Plot::graph()->setData(*x_, *y_);
}

QString WavetableUi::getWavetableId() const
{
	return *id_;
}

void WavetableUi::setFrequency(int number)
{
	frequency_ = number;

	// replot
}

int WavetableUi::getFrequency() const
{
	return frequency_;
}


void WavetableUi::setPhaseShiftingEnabled(bool enabled)
{
	Plot::setInteraction(QCP::Interaction::iRangeDrag, enabled);

	if (enabled && ! phaseShiftingEnabled_)
	{
		QWidget::setCursor(Qt::OpenHandCursor);

		connect(this, &Plot::mousePress,
				[=] (QMouseEvent*)
				{ QWidget::setCursor(Qt::ClosedHandCursor); });

		connect(this, &Plot::mouseRelease,
				[=] (QMouseEvent*)
				{ QWidget::setCursor(Qt::OpenHandCursor); });

		connect(this, &Plot::mouseDoubleClick,
				[=] (QMouseEvent*)
				{ xAxis->setRange(0, Global::wavetableLength); });
	}

	else if (! enabled && phaseShiftingEnabled_) Plot::disconnect();

	phaseShiftingEnabled_ = enabled;
}

bool WavetableUi::phaseShiftingEnabled() const
{
	return phaseShiftingEnabled_;
}
