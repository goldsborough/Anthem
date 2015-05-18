#ifndef ADDITIVEWAVETABLE_HPP
#define ADDITIVEWAVETABLE_HPP

#include "Plot.hpp"

#include <QSharedPointer>
#include <QVector>

class QColor;

class AdditiveWavetableUi : public Plot
{
	Q_OBJECT

	Q_PROPERTY(int frequency READ getFrequency WRITE setFrequency)

public:

	using number_t = unsigned short;


	explicit AdditiveWavetableUi(QWidget* parent = nullptr,
								 number_t numberOfPartials = 64,
								 number_t frequency = 3,
								 double masterAmplitude = 1,
								 number_t bitwidth = 16,
								 bool sigmaApproximationEnabled = false);

	void generate();

	void save();


	void setFrequency(number_t frequency);

	number_t getFrequency() const;


	void setNumberOfPartials(number_t number);

	number_t getNumberOfPartials() const;


	void setPartialAmplitude(number_t number, double amplitude);

	double getPartialAmplitude(number_t number) const;


	void setMasterAmplitude(double amplitude);

	double getMasterAmplitude() const;


	void setSigmaApproximationEnabled(bool enabled);

	bool sigmaApproximationEnabled() const;


	void setBitwidth(number_t bits);

	number_t getBitWidth() const;


	void clear();

private:

	struct Partial
	{
		double sigma;

		double amplitude;

		double increment;
	};

	QSharedPointer<QColor> overflowColor_;

	QVector<Partial> partials_;

	number_t frequency_;

	number_t bitwidth_;

	QCPDataMap* data_;

	double master_;

	double bins_;

	bool sigma_;

};

#endif // ADDITIVEWAVETABLE_HPP
