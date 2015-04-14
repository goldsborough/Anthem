#include "WavetableUi.hpp"
#include "qcustomplot.h"

#include <QStyleOption>
#include <QPainter>
#include <QGridLayout>
#include <cmath>


#include <QDebug>


WavetableUi::WavetableUi(QWidget *parent)
: QWidget(parent),
  plot_(new QCustomPlot(this))
{
	setupUi();
}

void WavetableUi::setupUi()
{
	QGridLayout* layout = new QGridLayout(this);

	layout->addWidget(plot_.data());


	plot_->addGraph();

	plot_->xAxis->setVisible(false);

	plot_->yAxis->setVisible(false);


	QVector<double> x(4096), y(4096);

	const double twoPi = 2 * std::atan2(0, -1);

	double radians = 0;

	double incr = twoPi / 4096;

	for (std::size_t i = 0; i < 4096; ++i)
	{
		x[i] = i/4096.0;

		y[i] = std::sin(radians);

		radians += incr;
	}

	plot_->graph(0)->setData(x, y);

	plot_->graph(0)->rescaleAxes();
}

void WavetableUi::paintEvent(QPaintEvent *)
{
	QStyleOption option;

	option.init(this);

	QPainter painter(this);

	style()->drawPrimitive(QStyle::PE_Widget, &option, &painter, this);
}
