#include "AnthemUi.hpp"
#include "Menubar.hpp"
#include "OperatorPage.hpp"
#include "MasterPage.hpp"

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

	pages->setMovable(true);

	pages->tabBar()->setCursor(Qt::PointingHandCursor);


	pages->addTab(new QWidget(this), "LFO");

	pages->addTab(new OperatorPage(this), "Operator");



	MasterPage* master = new MasterPage(this);

	connect(master, &MasterPage::volumeChanged,
			menubar, &Menubar::setVolume);

	pages->addTab(master, "Master");



	pages->addTab(new QWidget(this), "Effect");

	pages->addTab(new QWidget(this), "Envelope");


	pages->setCurrentIndex(2);


	layout->addWidget(pages);

	QWidget::setWindowTitle("Anthem");
}
