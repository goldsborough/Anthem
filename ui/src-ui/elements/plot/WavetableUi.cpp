#include "WavetableUi.hpp"
#include "qcustomplot.h"

#include <QStyleOption>
#include <QPainter>
#include <QGridLayout>
#include <cmath>


#include <QDebug>




QSharedPointer<QVector<double>> WavetableUi::initializeX()
{
	QSharedPointer<QVector<double>> temp(new QVector<double>(4096 * 3));

	double x = -4096; // use Global::WavetableLength

	for (auto& i : *temp) i = x++;

	return temp;
}

QSharedPointer<QVector<double>> WavetableUi::x_ = WavetableUi::initializeX();


WavetableUi::WavetableUi(QWidget *parent)
: QWidget(parent),
  gridShown_(true),
  y_(new QVector<double>(3 * 4096)), // Global::WavetableLength
  id_(new QString),
  background_(new QColor),
  line_(new QPen),
  grid_(new QPen),
  zero_(new QPen)
{
	setupPlot();


	auto layout = new QGridLayout(this);

	layout->setSpacing(0);

	layout->setContentsMargins(0, 0, 0, 0);

	layout->setMargin(0);

	layout->addWidget(plot_);


	QWidget::setCursor(Qt::SizeHorCursor);
}

void WavetableUi::setupPlot()
{
	plot_ = new QCustomPlot(this);

	graph_ = plot_->addGraph();


	plot_->xAxis->setTicks(false);

	plot_->yAxis->setTicks(false);

	plot_->yAxis->setRange(-1.05, 1.05);

	plot_->xAxis->setRange(0, 4096);


	plot_->setInteraction(QCP::Interaction::iRangeDrag);


	plot_->axisRect()->setRangeDrag(Qt::Horizontal);

	plot_->axisRect()->setAutoMargins(QCP::MarginSide::msNone);

	plot_->axisRect()->setMargins({0, 0, 0, 0});


	setWavetable(QString());
}

void WavetableUi::setWavetable(const QString &id)
{
	*id_ = id;

	plot_->setToolTip(id);

	// Get data from WavetableDatabase

	const double twoPi = 2 * M_PI;

	double phase = 0;

	double increment = twoPi / 4096;

	for (auto& i : *y_)
	{
		i = std::sin(phase);

		phase += increment;

		if (phase >= twoPi) phase -= twoPi;
	}

	graph_->setData(*x_, *y_);
}

QString WavetableUi::getWavetableId() const
{
	return *id_;
}

void  WavetableUi::setBackgroundColor(const QColor& color)
{
	*background_ = color;

	plot_->setBackground({color});
}

QColor WavetableUi::getBackgroundColor() const
{
	return *background_;
}

void WavetableUi::setLineColor(const QColor& color)
{
	line_->setColor(color);

	graph_->setPen(*line_);
}

QColor WavetableUi::getLineColor() const
{
	return line_->color();
}

void WavetableUi::setLineWidth(double width)
{
	line_->setWidthF(width);

	graph_->setPen(*line_);
}

double WavetableUi::getLineWidth() const
{
	return line_->widthF();
}


void WavetableUi::setGridShown(bool shown)
{
	gridShown_ = shown;

	plot_->xAxis->grid()->setVisible(shown);

	plot_->yAxis->grid()->setVisible(shown);
}

bool WavetableUi::getGridShown() const
{
	return gridShown_;
}


void WavetableUi::setGridColor(const QColor& color)
{
	grid_->setColor(color);

	plot_->xAxis->grid()->setPen(*grid_);

	plot_->yAxis->grid()->setPen(*grid_);
}

QColor WavetableUi::getGridColor() const
{
	return grid_->color();
}


void WavetableUi::setGridWidth(double width)
{
	grid_->setWidthF(width);

	plot_->xAxis->grid()->setPen(*grid_);

	plot_->yAxis->grid()->setPen(*grid_);
}

double WavetableUi::getGridWidth() const
{
	return grid_->widthF();
}


void WavetableUi::setZeroColor(const QColor& color)
{
	zero_->setColor(color);

	plot_->xAxis->grid()->setZeroLinePen(*zero_);

	plot_->yAxis->grid()->setZeroLinePen(*zero_);
}

QColor WavetableUi::getZeroColor() const
{
	return zero_->color();
}


void WavetableUi::setZeroWidth(double width)
{
	zero_->setWidthF(width);

	plot_->xAxis->grid()->setZeroLinePen(*zero_);

	plot_->yAxis->grid()->setZeroLinePen(*zero_);
}

double WavetableUi::getZeroWidth() const
{
	return zero_->widthF();
}

void WavetableUi::clear()
{
	y_.clear();

	graph_->clearData();
}
