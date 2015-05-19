#include "AdditiveWavetableUi.hpp"

#include <QPen>
#include <cmath>


#include <QDebug>

namespace Global
{
	const double pi = M_PI;

	const double twoPi = 2 * pi;
}

namespace Util
{
	void round(double& value, unsigned int bins)
	{
		// the rounding factor
		double factor = 2.0 / bins;

		double n = value / factor;

		int floored = static_cast<int>(n);

		// If the absolute difference between the division
		// and the floored value is greater than 0.5, round
		// to the next whole factor else to the factor times
		// the floored value

		double fractional = n - floored;

		// Absolute value
		if (fractional < 0) fractional = -fractional;

		if (fractional >= 0.5)
		{
			// Next value depends on sign
			double next = (n > 0) ? 1 : -1;

			value = (floored + next) * factor;
		}

		else value = floored * factor;
	}
}

AdditiveWavetableUi::AdditiveWavetableUi(QWidget *parent,
										 size_t samples,
										 double frequency,
										 size_t numberOfPartials,
										 size_t bitwidth,
										 bool sigmaApproximationEnabled)
: Plot(parent),
  separator_(new QPen(Qt::NoPen)),
  approximationConstant_(Global::pi / 8),
  partials_(numberOfPartials),
  frequency_(frequency),
  bitwidth_(bitwidth),
  samples_(samples),
  active_(8),
  bins_(1 << bitwidth),
  sigma_(sigmaApproximationEnabled)
{
	const double fundamental = frequency_ * (Global::twoPi / samples_);

	for (size_t n = 0; n < numberOfPartials; /* In loop */)
	{
		auto& partial = partials_[n++];

		partial.number = n;

		double sigma = approximationConstant_ * n;

		partial.sigma = std::sin(sigma) / sigma;

		partial.amplitude = 0;

		partial.increment = n * fundamental;
	}

	Plot::addGraph();

	Plot::yAxis->setRange(-1 - padding_, 1 + padding_);

	Plot::xAxis->setRange(1, samples_);

	for(size_t x = 0; x < samples_; ++x)
	{
		data_[x] = 0;

		Plot::graph()->addData(x, 0);
	}

	const double period = samples_ / frequency_;


	for (size_t i = 1; i < frequency_; ++i)
	{
		const double pos = i * period - 1;

		auto item = new QCPItemStraightLine(this);

		item->setPen(*separator_);

		item->point1->setType(QCPItemPosition::PositionType::ptPlotCoords);

		item->point1->setCoords(pos, Plot::yAxis->range().lower);

		item->point2->setType(QCPItemPosition::PositionType::ptPlotCoords);

		item->point2->setCoords(pos, Plot::yAxis->range().upper);

		Plot::addItem(item);
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

void AdditiveWavetableUi::clear()
{
	for (auto& i : partials_) i.amplitude = 0;

	auto i = Plot::graph()->data()->begin();

	for(auto& y : data_) y = (*i++).value = 0;

	Plot::replot();
}

void AdditiveWavetableUi::setFrequency(size_t frequency)
{
	frequency_ = frequency;
}

AdditiveWavetableUi::size_t AdditiveWavetableUi::getFrequency() const
{
	return frequency_;
}

void AdditiveWavetableUi::setAmplitude(size_t number, double amplitude)
{
	Partial& partial = partials_[number - 1];

	if (sigma_) amplitude *= partial.sigma;

	// Get the difference between the new amplitude we're gonna'
	// add and the old amplitude we're gonna' subtract
	const double difference = amplitude - partial.amplitude;

	partial.phase = 0;

	// Iterator for setting the displayed data
	auto i = Plot::graph()->data()->begin();

	double max = 0;

	for (auto& y : data_)
	{
		y += difference * std::sin(partial.phase);

		if (y > max) max = y;

		(*i++).value = y;

		partial.phase += partial.increment;

		if (partial.phase >= Global::twoPi)
		{
			partial.phase -= Global::twoPi;
		}
	}

	display_(max);

	partial.amplitude = amplitude;
}

void AdditiveWavetableUi::setActivePartials(size_t number)
{
	active_ = number;

	approximationConstant_ = Global::pi / number;

	recompute_(true);
}

AdditiveWavetableUi::size_t AdditiveWavetableUi::getActivePartials()
{
	return active_;
}

double AdditiveWavetableUi::getAmplitude(size_t number) const
{
	return partials_.at(number).amplitude;
}


void AdditiveWavetableUi::setSigmaApproximationEnabled(bool enabled)
{
	sigma_ = enabled;

	recompute_(false, true);
}

bool AdditiveWavetableUi::sigmaApproximationEnabled() const
{
	return sigma_;
}


void AdditiveWavetableUi::setBitwidth(size_t bits)
{
	bitwidth_ = bits;

	bins_ = 1 << bits;

	recompute_();
}

AdditiveWavetableUi::size_t AdditiveWavetableUi::getBitWidth() const
{
	return bitwidth_;
}

void AdditiveWavetableUi::setSeparatorColor(const QColor& color)
{
	separator_->setColor(color);

	for (size_t i = 0, end = Plot::itemCount(); i < end; ++i)
	{
		dynamic_cast<QCPItemStraightLine*>(Plot::item(i))->setPen(*separator_);
	}
}

QColor AdditiveWavetableUi::getSeparatorColor() const
{
	return separator_->color();
}

void AdditiveWavetableUi::setSeparatorWidth(double width)
{
	separator_->setWidthF(width);

	for (size_t i = 0, end = Plot::itemCount(); i < end; ++i)
	{
		dynamic_cast<QCPItemStraightLine*>(Plot::item(i))->setPen(*separator_);
	}
}

double AdditiveWavetableUi::getSeparatorWidth() const
{
	return separator_->widthF();
}

void AdditiveWavetableUi::recompute_(bool recomputeSigma,
									 bool changeSigma)
{
	// Grab iterator to set displayed value
	auto x = Plot::graph()->data()->begin();

	double max = 0;

	for (auto& y : data_)
	{
		// Reset
		y = 0;

		for (size_t n = 0; n < active_; ++n)
		{
			Partial& partial = partials_[n];

			// Some initializations and optimizations
			// according to the flags set, at the start
			if (! x->key) reinitialize_(partial, recomputeSigma, changeSigma);

			y += partial.amplitude * std::sin(partial.phase);

			partial.phase += partial.increment;

			if (partial.phase >= Global::twoPi)
			{
				partial.phase -= Global::twoPi;
			}
		}

		if (y > max) max = y;

		// Set the displayed value for later
		(*x++).value = y;
	}

	display_(max);
}

void AdditiveWavetableUi::reinitialize_(Partial& partial,
										bool recomputeSigma,
										bool changeSigma)
{
	// The approximation constant has changed so
	// we need to reset the sigma factor for each
	// partial and apply it if sigma_ is currently on
	if (recomputeSigma)
	{
		if (sigma_) partial.amplitude /= partial.sigma;

		double sigma = approximationConstant_ * (partial.number + 1);

		partial.sigma = std::sin(sigma) / sigma;

		if (sigma_) partial.amplitude *= partial.sigma;
	}

	// The sigma_ has changed so apply or remove
	// the partial's internal sigma factor
	else if (changeSigma)
	{
		partial.amplitude *= (sigma_) ? partial.sigma
									  : 1 / partial.sigma;
	}

	// Always need this
	partial.phase = 0;
}

double AdditiveWavetableUi::round_(double value)
{
	if (bitwidth_ < 16) Util::round(value, bins_);

	return value;
}

void AdditiveWavetableUi::display_(double max)
{
	double factor = (max > 1) ? 1 / max : 1;

	for (auto& i : *(Plot::graph()->data()))
	{
		// Set to the scaled then rounded value
		i.value = round_(i.value * factor);
	}

	Plot::replot();
}
