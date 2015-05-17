#ifndef PLOT_HPP
#define PLOT_HPP

#include "qcustomplot.h"

#include <QSharedPointer>

class QPen;
class QColor;

class Plot : public QCustomPlot
{
	Q_OBJECT

	Q_PROPERTY(double margin READ getMargin WRITE setMargin)

	Q_PROPERTY(QColor backgroundColor READ getBackgroundColor WRITE setBackgroundColor)


	Q_PROPERTY(QColor lineColor READ getLineColor WRITE setLineColor)

	Q_PROPERTY(double lineWidth READ getLineWidth WRITE setLineWidth)


	Q_PROPERTY(bool gridShown READ getGridShown WRITE setGridShown)

	Q_PROPERTY(QColor gridColor READ getGridColor WRITE setGridColor)

	Q_PROPERTY(double gridWidth READ getGridWidth WRITE setGridWidth)

	Q_PROPERTY(int gridStyle READ getGridStyle WRITE setGridStyle)


	Q_PROPERTY(QColor zeroColor READ getZeroColor WRITE setZeroColor)

	Q_PROPERTY(double zeroWidth READ getZeroWidth WRITE setZeroWidth)


public:

	explicit Plot(QWidget* parent = nullptr);

	virtual ~Plot();


	virtual void setMargin(double margin);

	virtual double getMargin() const;


	virtual void setBackgroundColor(const QColor& color);

	virtual QColor getBackgroundColor() const;


	virtual void setLineColor(const QColor& color);

	virtual QColor getLineColor() const;


	virtual void setLineWidth(double width);

	virtual double getLineWidth() const;


	virtual void setGridShown(bool shown);

	virtual bool getGridShown() const;


	virtual void setGridColor(const QColor& color);

	virtual QColor getGridColor() const;


	virtual void setGridWidth(double width);

	virtual double getGridWidth() const;


	virtual void setGridStyle(int width);

	virtual int getGridStyle() const;


	virtual void setZeroColor(const QColor& color);

	virtual QColor getZeroColor() const;


	virtual void setZeroWidth(double width);

	virtual double getZeroWidth() const;

protected:

	virtual void setupPlot();


	bool gridShown_;

	double margin_;


	QSharedPointer<QPen> zero_;

	QSharedPointer<QPen> line_;

	QSharedPointer<QPen> grid_;

	QSharedPointer<QColor> background_;
};

#endif // PLOT_HPP
