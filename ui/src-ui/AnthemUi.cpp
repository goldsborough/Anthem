#include "AnthemUi.hpp"
#include "Menubar.hpp"
#include "OperatorPage.hpp"

#include <QTabWidget>
#include <QTabBar>
#include <QVBoxLayout>

AnthemUi::AnthemUi()
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
