#include "WavetableUi.hpp"
#include "qcustomplot.h"

#include <QStyleOption>
#include <QPainter>
#include <QGridLayout>
#include <cmath>


#include <QDebug>

namespace Global
{
	const unsigned short wavetableLength = 4095;
};


QSharedPointer<QVector<double>> WavetableUi::initializeX()
{
	QSharedPointer<QVector<double>> temp(new QVector<double>(Global::wavetableLength * 3));

	double x = -Global::wavetableLength;

	for (auto& i : *temp) i = x++;

	return temp;
}

QSharedPointer<QVector<double>> WavetableUi::x_ = WavetableUi::initializeX();


WavetableUi::WavetableUi(QWidget *parent)
: QWidget(parent),
  gridShown_(true),
  conversion_(360.0 / Global::wavetableLength),
  y_(new QVector<double>(3 * Global::wavetableLength)),
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


	QWidget::setCursor(Qt::OpenHandCursor);
}

void WavetableUi::setupPlot()
{
	plot_ = new QCustomPlot(this);

	connect(plot_, &QCustomPlot::mousePress,
			[=] (QMouseEvent*)
			{ QWidget::setCursor(Qt::ClosedHandCursor); });

	connect(plot_, &QCustomPlot::mouseRelease,
			[=] (QMouseEvent*)
			{ QWidget::setCursor(Qt::OpenHandCursor); });

	connect(plot_, &QCustomPlot::mouseDoubleClick,
			[=] (QMouseEvent*)
			{ plot_->xAxis->setRange(0, Global::wavetableLength); });


	graph_ = plot_->addGraph();


	plot_->xAxis->setTicks(false);

	plot_->xAxis->setBasePen(Qt::NoPen);

	plot_->xAxis->setRange(0, Global::wavetableLength); // Global::wavetableLength


	plot_->yAxis->setTicks(false);

	plot_->yAxis->setBasePen(Qt::NoPen);


	plot_->setInteraction(QCP::Interaction::iRangeDrag);

	plot_->axisRect()->setRangeDrag(Qt::Horizontal);

	// Wow so short amaze such concise
	auto signal = static_cast<void(QCPAxis::*)(const QCPRange&)>(&QCPAxis::rangeChanged);

	// Ensures the maximum or minimum range is not trespassed
	connect(plot_->xAxis, signal,
			[=] (const QCPRange& range)
			{
				if (range.lower < -Global::wavetableLength)
				{
					plot_->xAxis->setRange(-Global::wavetableLength, 0);
				}

				else if (range.upper > Global::wavetableLength * 2)
				{
					plot_->xAxis->setRange(Global::wavetableLength,
										   Global::wavetableLength * 2);
				}

				emit phaseChanged(range.lower * conversion_);
			});


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

	double increment = twoPi / Global::wavetableLength;

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

void WavetableUi::setMargin(double margin)
{
	margin_ = margin;

	margin /= 100;

	plot_->yAxis->setRange(-1 - margin, 1 + margin);
}

double WavetableUi::getMargin() const
{
	return margin_;
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
