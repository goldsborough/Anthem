#ifndef WAVETABLEUI_HPP
#define WAVETABLEUI_HPP

#include "Plot.hpp"

#include <QSharedPointer>
#include <QVector>

class QString;

class WavetableUi : public Plot
{
	Q_OBJECT

public:

	explicit WavetableUi(QWidget* parent = nullptr);


	void setWavetable(const QString& id);

	QString getWavetableId() const;


	virtual void setZeroColor(const QColor& color) override;

	virtual void setZeroWidth(double width) override;


signals:

	void phaseChanged(int degrees) const;

private:

	static QVector<double> x_;

	static QVector<double> initializeX();

	static const double conversion_;


	QVector<double> y_;

	QSharedPointer<QString> id_;
};

#endif // WAVETABLEUI_HPP
