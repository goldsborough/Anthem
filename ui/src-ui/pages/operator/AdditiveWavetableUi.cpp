#include "AdditiveWavetableUi.hpp"

#include <QColor>
#include <cmath>


#include <QDebug>


namespace Global
{
	const unsigned short wavetableLength = 4095;

	const double pi = M_PI;

	const double twoPi = 2 * pi;

	const double tableIncrement = twoPi / wavetableLength;
}

namespace Util
{
	void round(double& val, unsigned int bins)
	{
		// the rounding factor
		double factor = 1.0 / bins;

		double n = val / factor;

		int nFloor = static_cast<int>(n);

		// If the absolute difference between the division
		// and the floored value is greater than 0.5, round
		// to the next whole factor else to the factor times
		// the floored value

		double fractional = n - nFloor;

		// Absolute value
		if (fractional < 0) fractional = -fractional;

		if (fractional >= 0.5)
		{
			// Next value depends on sign
			double add = (n > 0) ? 1 : -1;

			val = (nFloor + add) * factor;
		}

		else val = nFloor * factor;
	}
}

AdditiveWavetableUi::AdditiveWavetableUi(QWidget *parent,
										 number_t numberOfPartials,
										 number_t frequency,
										 double masterAmplitude,
										 number_t bitwidth,
										 bool sigmaApproximationEnabled)
: Plot(parent),
  partials_(numberOfPartials),
  frequency_(frequency),
  bitwidth_(bitwidth),
  master_(masterAmplitude),
  bins_(std::pow(2, bitwidth)),
  sigma_(sigmaApproximationEnabled)
{
	const double constant = Global::pi / numberOfPartials;

	for (number_t n = 0; n < numberOfPartials;)
	{
		auto& partial = partials_[n++];

		partial.sigma = constant * n;

		partial.amplitude = 0;

		partial.increment = n * Global::tableIncrement;
	}

	Plot::addGraph();

	Plot::yAxis->setRange(-1.2, 1.2);

	Plot::xAxis->setRange(1, Global::wavetableLength);

	data_ = Plot::graph()->data();

	for(number_t n = 0; n < Global::wavetableLength; ++n)
	{
		(*data_)[n] = QCPData(n, 0);
	}
}

void AdditiveWavetableUi::generate()
{
	// Create Wavetable and send to WavetableDatabase
}

void AdditiveWavetableUi::save()
{
	// Tell WavetableDatabase to save the Wavetable
}

void AdditiveWavetableUi::setFrequency(number_t frequency)
{
	frequency_ = frequency;

	// replot
}

AdditiveWavetableUi::number_t AdditiveWavetableUi::getFrequency() const
{
	return frequency_;
}

void AdditiveWavetableUi::clear()
{




	// !




}

void AdditiveWavetableUi::setPartialAmplitude(number_t number, double amplitude)
{
	Partial& partial = partials_[number - 1];

	if (sigma_) amplitude *= partial.sigma;

	const double difference = (amplitude * master_) - partial.amplitude;

	double phase = 0;

	for (auto& y : *data_)
	{
		y.value += difference * std::sin(phase);

		// Util::round(y, bins_);

		phase += partial.increment;

		if (phase >= Global::twoPi)
		{
			phase -= Global::twoPi;
		}
	}

	partial.amplitude = amplitude;

	Plot::replot();
}

double AdditiveWavetableUi::getPartialAmplitude(number_t number) const
{
	return partials_.at(number).amplitude;
}


void AdditiveWavetableUi::setMasterAmplitude(double amplitude)
{
	master_ = amplitude;
}

double AdditiveWavetableUi::getMasterAmplitude() const
{
	return master_;
}


void AdditiveWavetableUi::setSigmaApproximationEnabled(bool enabled)
{
	sigma_ = enabled;
}

bool AdditiveWavetableUi::sigmaApproximationEnabled() const
{
	return sigma_;
}


void AdditiveWavetableUi::setBitwidth(number_t bits)
{
	bitwidth_ = bits;
}

AdditiveWavetableUi::number_t AdditiveWavetableUi::getBitWidth() const
{
	return bitwidth_;
}
