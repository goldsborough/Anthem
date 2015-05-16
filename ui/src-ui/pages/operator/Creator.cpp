#include "Creator.hpp"

#include <QPainter>
#include <QStyleOption>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QComboBox>
#include <QPushButton>

Creator::Creator(QWidget *parent)
: QWidget(parent),
  layout_(new QHBoxLayout(this)),
  container_(new QTabWidget(this))
{
	layout_->setSpacing(0);

	layout_->setContentsMargins(0, 0, 0, 0);

	layout_->setMargin(0);


	setupMenu();

	setupContainer();

	setupPartials();

	setupDraw();
}

void Creator::setupContainer()
{
	container_->setTabPosition(QTabWidget::East);

	container_->tabBar()->setSizePolicy(QSizePolicy::Maximum,
										QSizePolicy::Maximum);

	container_->tabBar()->setCursor(Qt::PointingHandCursor);

	layout_->addWidget(container_);
}

void Creator::setupMenu()
{
	auto menu = new QVBoxLayout;

	menu->setSpacing(0);

	menu->setContentsMargins(0, 0, 0, 0);

	menu->setMargin(0);


	auto generate = new QPushButton("+", this);

	generate->setObjectName("CreatorGenerateButton");

	generate->setSizePolicy(QSizePolicy::Maximum,
							QSizePolicy::Expanding);

	generate->setCursor(Qt::PointingHandCursor);

	menu->addWidget(generate);


	auto save = new QPushButton(QString(0x2193), this);

	save->setSizePolicy(QSizePolicy::Maximum,
						QSizePolicy::Expanding);

	save->setCursor(Qt::PointingHandCursor);

	menu->addWidget(save);


	auto bits = new QPushButton("16", this);

	bits->setSizePolicy(QSizePolicy::Maximum,
						QSizePolicy::Expanding);

	bits->setCursor(Qt::PointingHandCursor);

	//bits->addItems({"4", "8", "16"});

	menu->addWidget(bits);


	auto partials = new QPushButton("64", this);//QComboBox(this);

	partials->setSizePolicy(QSizePolicy::Maximum,
							QSizePolicy::Expanding);

	partials->setCursor(Qt::PointingHandCursor);

	//partials->addItems({"8", "16", "32", "64"});

	menu->addWidget(partials);

	connect(container_, &QTabWidget::currentChanged,
		   [=] (int) { partials->setVisible(! partials->isVisible()); });

	layout_->addLayout(menu);
}

void Creator::setupPartials()
{
	container_->addTab(new QWidget(this), "P");
}

void Creator::setupDraw()
{
	container_->addTab(new QWidget(this), "D");
}

void Creator::paintEvent(QPaintEvent*)
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
