#include "Plot.hpp"
#include "qcustomplot.h"

#include <QPen>
#include <QColor>

Plot::Plot(QWidget *parent)
: QCustomPlot(parent),
  gridShown_(true),
  margin_(0),
  zero_(new QPen),
  line_(new QPen),
  grid_(new QPen),
  background_(new QColor)
{
	setupPlot();
}

Plot::~Plot() = default;

void Plot::setupPlot()
{
	QCustomPlot::xAxis->setTicks(false);

	QCustomPlot::xAxis->setBasePen(Qt::NoPen);

	QCustomPlot::yAxis->setTicks(false);

	QCustomPlot::yAxis->setBasePen(Qt::NoPen);

	QCustomPlot::axisRect()->setAutoMargins(QCP::MarginSide::msNone);

	QCustomPlot::axisRect()->setMargins({0, 0, 0, 0});
}

void Plot::setMargin(double margin)
{
	margin_ = margin;

	margin /= 100;

	QCustomPlot::yAxis->setRange(-1 - margin, 1 + margin);
}

double Plot::getMargin() const
{
	return margin_;
}

void  Plot::setBackgroundColor(const QColor& color)
{
	*background_ = color;

	QCustomPlot::setBackground({color});
}

QColor Plot::getBackgroundColor() const
{
	return *background_;
}

void Plot::setLineColor(const QColor& color)
{
	line_->setColor(color);

	for (int i = 0; i < Plot::plottableCount(); ++i)
	{
		Plot::plottable(i)->setPen(*line_);

		Plot::plottable()->setSelectedPen(*line_);
	}
}

QColor Plot::getLineColor() const
{
	return line_->color();
}

void Plot::setLineWidth(double width)
{
	if (! width) *line_ = QPen(Qt::NoPen);

	else line_->setWidthF(width);

	for (int i = 0; i < Plot::plottableCount(); ++i)
	{
		Plot::plottable(i)->setPen(*line_);

		Plot::plottable()->setSelectedPen(*line_);
	}
}

double Plot::getLineWidth() const
{
	return line_->widthF();
}


void Plot::setGridShown(bool shown)
{
	gridShown_ = shown;

	QCustomPlot::xAxis->grid()->setVisible(shown);

	QCustomPlot::yAxis->grid()->setVisible(shown);
}

bool Plot::getGridShown() const
{
	return gridShown_;
}


void Plot::setGridColor(const QColor& color)
{
	grid_->setColor(color);

	QCustomPlot::xAxis->grid()->setPen(*grid_);

	QCustomPlot::yAxis->grid()->setPen(*grid_);
}

QColor Plot::getGridColor() const
{
	return grid_->color();
}


void Plot::setGridWidth(double width)
{
	grid_->setWidthF(width);

	QCustomPlot::xAxis->grid()->setPen(*grid_);

	QCustomPlot::yAxis->grid()->setPen(*grid_);
}

double Plot::getGridWidth() const
{
	return grid_->widthF();
}

void Plot::setGridStyle(int style)
{
	grid_->setStyle(static_cast<Qt::PenStyle>(style));

	QCustomPlot::xAxis->grid()->setPen(*grid_);

	QCustomPlot::yAxis->grid()->setPen(*grid_);
}

int Plot::getGridStyle() const
{
	return grid_->style();
}


void Plot::setZeroColor(const QColor& color)
{
	zero_->setColor(color);

	QCustomPlot::xAxis->grid()->setZeroLinePen(*zero_);

	QCustomPlot::yAxis->grid()->setZeroLinePen(*zero_);
}

QColor Plot::getZeroColor() const
{
	return zero_->color();
}


void Plot::setZeroWidth(double width)
{
	zero_->setWidthF(width);

	QCustomPlot::xAxis->grid()->setZeroLinePen(*zero_);

	QCustomPlot::yAxis->grid()->setZeroLinePen(*zero_);
}

double Plot::getZeroWidth() const
{
	return zero_->widthF();
}
