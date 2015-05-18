#ifndef PARTIALS_HPP
#define PARTIALS_HPP

#include "Plot.hpp"

#include <QSharedPointer>

class ComboBox;
class QColor;
class QCPAxis;
class QCPRange;
class QPoint;
class QString;

class PartialsUi : public Plot
{
	Q_OBJECT

	Q_PROPERTY(QColor barColor READ getBarColor WRITE setBarColor)

	// For 8 Partials as base value
	Q_PROPERTY(double barWidth READ getBarWidth WRITE setBarWidth)


	Q_PROPERTY(int precision READ getPrecision WRITE setPrecision)


	Q_PROPERTY(double sideOffset READ getSideOffset WRITE setSideOffset)

	Q_PROPERTY(double baseValue READ getBaseValue WRITE setBaseValue)

	Q_PROPERTY(double displayFactor READ getDisplayFactor WRITE setDisplayFactor)

public:

	explicit PartialsUi(QWidget* parent = nullptr);


	void setNumberOfPartials(int number);

	int getNumberOfPartials() const;


	void setBarColor(const QColor& color);

	QColor getBarColor() const;


	void setBarWidth(double width);

	double getBarWidth() const;


	void setSideOffset(double offset);

	double getSideOffset() const;


	virtual void setPadding(double padding) override;

	virtual double getPadding() const override;


	void setDisplayFactor(double factor);

	double getDisplayFactor();


	void setPrecision(int precision);

	static int getPrecision();


	void setBaseValue(double value);

	double getBaseValue();


private:

	struct Partial : public QCPBars
	{
		Partial(unsigned short num,
				QCPAxis* x,
				QCPAxis* y);


		void setAmplitude(double amp);

		void setAmplitude(const QPoint& pos);

		void updateRange();


		static QSharedPointer<QCPRange> zero;

		static double displayFactor;

		static double baseValue;

		static int precision;

		static double range;


		/*! Stored as double to avoid casting */
		const double number;

		double amplitude;

		QSharedPointer<QString> string;
	};


	virtual void resizeEvent(QResizeEvent *event) override;

	void handleMouseMove(QMouseEvent* event);

	void setupConnections();


	QSharedPointer<QVector<Partial*>> partials_;

	QSharedPointer<QPoint> lastPosition_;

	QSharedPointer<QColor> barColor_;

	QPointer<Partial> activePartial_;

	QPointer<Partial> lastShown_;

	bool hasMovedAway_;

	double sideOffset_;

	double barWidth_;

	int number_;
};

#endif // PARTIALS_HPP
