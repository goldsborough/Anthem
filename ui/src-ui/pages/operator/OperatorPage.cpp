#include "OperatorPage.hpp"
#include "UnitPage.hpp"
#include "AlgorithmPage.hpp"

#include <QGridLayout>
#include <QStyleOption>
#include <QPainter>
#include <QPushButton>

OperatorPage::OperatorPage(QWidget* parent)
	: QTabWidget(parent)
{
	setupUi();
}

void OperatorPage::setupUi()
{
	QTabWidget::addTab(new UnitPage(), "Units");

	QTabWidget::addTab(new AlgorithmPage(), "Algorithms");

	QTabWidget::setTabPosition(QTabWidget::South);

	QTabWidget::tabBar()->setCursor(Qt::PointingHandCursor);
}

void OperatorPage::paintEvent(QPaintEvent*)
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
