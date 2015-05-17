#ifndef WAVETABLEUI_HPP
#define WAVETABLEUI_HPP

#include <QSharedPointer>

#include "Plot.hpp"

class QString;

class WavetableUi : public Plot
{
	Q_OBJECT

public:

	explicit WavetableUi(QWidget* parent = nullptr);


	void setWavetable(const QString& id);

	QString getWavetableId() const;

signals:

	void phaseChanged(int degrees) const;

private:

	static QSharedPointer<QVector<double>> x_;

	static QSharedPointer<QVector<double>> initializeX();


	void setupPlot();


	double conversion_;

	QSharedPointer<QVector<double>> y_;

	QSharedPointer<QString> id_;
};

#endif // WAVETABLEUI_HPP
