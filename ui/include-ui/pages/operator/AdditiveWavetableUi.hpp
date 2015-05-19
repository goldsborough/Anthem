#ifndef ADDITIVEWAVETABLE_HPP
#define ADDITIVEWAVETABLE_HPP

#include "Plot.hpp"

#include <QSharedPointer>
#include <QVector>
#include <QMap>

class QPen;

class AdditiveWavetableUi : public Plot
{
	Q_OBJECT

	Q_PROPERTY(int frequency READ getFrequency WRITE setFrequency)

	Q_PROPERTY(QColor separatorColor READ getSeparatorColor WRITE setSeparatorColor)

	Q_PROPERTY(double separatorWidth READ getSeparatorWidth WRITE setSeparatorWidth)

public:

	using size_t = unsigned short;


	explicit AdditiveWavetableUi(QWidget* parent = nullptr,
								 size_t samples = 1000,
								 double frequency = 3,
								 size_t numberOfPartials = 64,
								 size_t bitwidth = 16,
								 bool sigmaApproximationEnabled = false);

	void generate();

	void save();

	void clear();


	void setActivePartials(size_t number);

	size_t getActivePartials(); // non-const


	void setFrequency(size_t frequency);

	size_t getFrequency() const;


	void setAmplitude(size_t number, double amplitude);

	double getAmplitude(size_t number) const;


	void setSigmaApproximationEnabled(bool enabled);

	bool sigmaApproximationEnabled() const;


	void setBitwidth(size_t bits);

	size_t getBitWidth() const;


	void setSeparatorColor(const QColor& color);

	QColor getSeparatorColor() const;


	void setSeparatorWidth(double width);

	double getSeparatorWidth() const;


private:

	struct Partial
	{
		size_t number;

		double sigma;

		double amplitude;

		double increment;

		double phase;
	};

	void recompute_(bool recomputeSigma = false,
					bool changeSigma = false);

	void reinitialize_(Partial& partial,
					   bool recomputeSigma,
					   bool changeSigma);

	double round_(double value);

	void display_(double max);


	QSharedPointer<QPen> separator_;

	double approximationConstant_;

	QMap<size_t, double> data_;

	QVector<Partial> partials_;

	double frequency_;

	size_t bitwidth_;

	size_t samples_;

	size_t active_;

	double bins_;

	bool sigma_;

};

#endif // ADDITIVEWAVETABLE_HPP
