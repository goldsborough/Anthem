#ifndef PLOT_HPP
#define PLOT_HPP

#include "qcustomplot.h"

#include <QSharedPointer>

class QPen;
class QColor;

class Plot : public QCustomPlot
{
	Q_OBJECT

	Q_PROPERTY(int margin READ getMargin WRITE setMargin)

	Q_PROPERTY(double padding READ getPadding WRITE setPadding)

	Q_PROPERTY(QColor backgroundColor READ getBackgroundColor WRITE setBackgroundColor)


	Q_PROPERTY(QColor lineColor READ getLineColor WRITE setLineColor)

	Q_PROPERTY(double lineWidth READ getLineWidth WRITE setLineWidth)


	Q_PROPERTY(bool xGridShown READ xGridShown WRITE setXGridShown)

	Q_PROPERTY(bool yGridShown READ yGridShown WRITE setYGridShown)


	Q_PROPERTY(QColor gridColor READ getGridColor WRITE setGridColor)

	Q_PROPERTY(double gridWidth READ getGridWidth WRITE setGridWidth)

	Q_PROPERTY(int gridStyle READ getGridStyle WRITE setGridStyle)


	Q_PROPERTY(QColor zeroColor READ getZeroColor WRITE setZeroColor)

	Q_PROPERTY(double zeroWidth READ getZeroWidth WRITE setZeroWidth)


public:

	explicit Plot(QWidget* parent = nullptr);

	virtual ~Plot();


	virtual void setMargin(int margin);

	virtual int getMargin() const;


	virtual void setPadding(double padding);

	virtual double getPadding() const;


	virtual void setBackgroundColor(const QColor& color);

	virtual QColor getBackgroundColor() const;


	virtual void setLineColor(const QColor& color);

	virtual QColor getLineColor() const;


	virtual void setLineWidth(double width);

	virtual double getLineWidth() const;


	virtual void setXGridShown(bool shown);

	virtual bool xGridShown() const;


	virtual void setYGridShown(bool shown);

	virtual bool yGridShown() const;


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

	double margin_;

	double padding_;


	QSharedPointer<QPen> zero_;

	QSharedPointer<QPen> line_;

	QSharedPointer<QPen> grid_;

	QSharedPointer<QColor> background_;
};

#endif // PLOT_HPP
