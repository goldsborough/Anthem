#ifndef WAVETABLEUI_HPP
#define WAVETABLEUI_HPP

#include <QSharedPointer>

#include "Plot.hpp"

class QString;

class WavetableUi : public Plot
{
	Q_OBJECT

public:

	explicit WavetableUi(QWidget* parent = nullptr,
						 int frequency = 1,
						 bool phaseShiftEnabled = true);


	void setWavetable(const QString& id);

	QString getWavetableId() const;


	void setFrequency(int number);

	int getFrequency() const;


	void setPhaseShiftingEnabled(bool enabled);

	bool phaseShiftingEnabled() const;

signals:

	void phaseChanged(int degrees) const;

private:

	static const double conversion_;

	static QSharedPointer<QVector<double>> x_;

	static QSharedPointer<QVector<double>> initializeX();


	int frequency_;

	bool phaseShiftingEnabled_;

	QSharedPointer<QString> id_;

	QSharedPointer<QVector<double>> y_;
};

#endif // WAVETABLEUI_HPP
