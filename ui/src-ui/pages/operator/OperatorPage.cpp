#include "OperatorPage.hpp"
#include "UnitPage.hpp"
#include "AlgorithmPage.hpp"

#include "Creator.hpp"

#include <QGridLayout>
#include <QStyleOption>
#include <QPainter>


#include <QDebug>


OperatorPage::OperatorPage(QWidget* parent)
: QTabWidget(parent)
{
	setupUi();
}

void OperatorPage::setupUi()
{
	auto unit = new UnitPage(this);

	QTabWidget::addTab(unit, "Units");

	QTabWidget::setMovable(true);


	auto alg = new AlgorithmPage(this);

	connect(alg, &AlgorithmPage::algorithmChanged,
			[] (AlgorithmPage::index_t index)
			{ qDebug() << index; });

	QTabWidget::addTab(alg, "Algorithms");


	auto creator = new Creator(this);

	QTabWidget::addTab(creator, "Create");


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
