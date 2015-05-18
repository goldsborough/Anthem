#include "Plot.hpp"
#include "qcustomplot.h"

#include <QPen>
#include <QColor>

Plot::Plot(QWidget *parent)
: QCustomPlot(parent),
  padding_(0),
  zero_(new QPen),
  line_(new QPen),
  grid_(new QPen),
  background_(new QColor)
{
	QCustomPlot::xAxis->setTicks(false);

	QCustomPlot::xAxis->setBasePen(Qt::NoPen);

	QCustomPlot::yAxis->setTicks(false);

	QCustomPlot::yAxis->setBasePen(Qt::NoPen);

	QCustomPlot::axisRect()->setAutoMargins(QCP::MarginSide::msNone);

	setMargin(0);
}

Plot::~Plot() = default;

void Plot::setMargin(int margin)
{
	margin_ = margin;

	QCustomPlot::axisRect()->setMargins({margin, margin, margin, margin});
}

int Plot::getMargin() const
{
	return margin_;
}

void Plot::setPadding(double padding)
{
	padding_ = padding;

	QCustomPlot::yAxis->setRange(-1 - padding, 1 + padding);
}

double Plot::getPadding() const
{
	return padding_;
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
	}
}

double Plot::getLineWidth() const
{
	return line_->widthF();
}


void Plot::setXGridShown(bool shown)
{
	QCustomPlot::xAxis->grid()->setVisible(shown);
}

bool Plot::xGridShown() const
{
	return  QCustomPlot::xAxis->grid()->visible();
}

void Plot::setYGridShown(bool shown)
{
	QCustomPlot::yAxis->grid()->setVisible(shown);
}

bool Plot::yGridShown() const
{
	return QCustomPlot::yAxis->grid()->visible();
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
