#include "AnthemUi.hpp"
#include "Menubar.hpp"
#include "OperatorPage.hpp"

#include <QTabWidget>
#include <QTabBar>
#include <QMoveEvent>
#include <QVBoxLayout>
#include <QStyleOption>
#include <QPainter>


#include <QDebug>
#include <QMenu>



AnthemUi::AnthemUi(QWidget *parent)
: QWidget(parent)
{
    setupUi();
}

void AnthemUi::setupUi()
{
	QVBoxLayout* layout = new QVBoxLayout(this);

    layout->setMargin(0);

	layout->setContentsMargins(0,0,0,0);


	Menubar* menubar = new Menubar(this);

	menubar->setSizePolicy(QSizePolicy::Expanding,
						   QSizePolicy::Expanding);

	layout->addWidget(menubar);


	QTabWidget* pages = new QTabWidget(this);

	pages->setSizePolicy(QSizePolicy::Preferred,
						 QSizePolicy::Preferred);

    pages->tabBar()->setCursor(Qt::PointingHandCursor);


	pages->addTab(new QWidget(this), "LFO");

	pages->addTab(new OperatorPage(this), "Operator");

	pages->addTab(new QWidget(this), "Master");

	pages->addTab(new QWidget(this), "Effect");

	pages->addTab(new QWidget(this), "Envelope");


	layout->addWidget(pages);


	QWidget::setSizePolicy(QSizePolicy::Fixed,
						   QSizePolicy::Fixed);

	QWidget::setWindowTitle("Anthem");
}

void AnthemUi::paintEvent(QPaintEvent*)
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
