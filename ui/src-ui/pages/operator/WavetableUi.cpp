#include "WavetableUi.hpp"

#include <cmath>

namespace Global
{
	const unsigned short wavetableLength = 4095;
};

const double WavetableUi::conversion_ = 360.0 / Global::wavetableLength;


QVector<double> WavetableUi::initializeX()
{
	QVector<double> temp(3 * Global::wavetableLength);

	double x = -Global::wavetableLength;

	for (auto& i : temp) i = x++;

	return temp;
}

QVector<double> WavetableUi::x_ = WavetableUi::initializeX();

WavetableUi::WavetableUi(QWidget *parent)
: Plot(parent),
  y_(3 * Global::wavetableLength),
  id_(new QString)
{
	Plot::setInteraction(QCP::Interaction::iRangeDrag);

	Plot::setCursor(Qt::OpenHandCursor);

	connect(this, &Plot::mousePress,
			[=] (QMouseEvent*)
			{ Plot::setCursor(Qt::ClosedHandCursor); });

	connect(this, &Plot::mouseRelease,
			[=] (QMouseEvent*)
			{ Plot::setCursor(Qt::OpenHandCursor); });

	connect(this, &Plot::mouseDoubleClick,
			[=] (QMouseEvent*)
			{ xAxis->setRange(0, Global::wavetableLength); });

	Plot::addGraph();

	Plot::yAxis->setRange(-1, 1);

	Plot::xAxis->setRange(1, Global::wavetableLength);

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

	const double twoPi = 2 * M_PI;

	double phase = 0;

	double increment = twoPi / Global::wavetableLength;

	for (auto& i : y_)
	{
		i = std::sin(phase);

		phase += increment;

		if (phase >= twoPi) phase -= twoPi;
	}

	Plot::graph()->setData(x_, y_);
}

QString WavetableUi::getWavetableId() const
{
	return *id_;
}
