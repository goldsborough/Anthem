#include <QtGui>

#include "MainWindow.h"
#include "qcustomplot/qcustomplot.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    _initUI();
}

void MainWindow::_initUI()
{
    QVector<double> x(101),y(101);

    for (unsigned short n = 0; n <= 100; ++n)
    {
        x[n] = (n/50.0) - 0.5;
        y[n] = x[n]*x[n];
    }

    QCustomPlot* plot = new QCustomPlot();

    plot->addGraph();

    plot->graph(0)->setData(x,y);

    plot->xAxis->setLabel("x");
    plot->yAxis->setLabel("y");

    plot->xAxis->setRange(-1, 1);
    plot->yAxis->setRange(0, 1);
    plot->replot();

    this->setGeometry(100,100,1000,760);
    this->setWindowTitle(tr("Anthem"));

    this->setCentralWidget(plot);
}
