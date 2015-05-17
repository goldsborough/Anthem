#include "Creator.hpp"
#include "ComboBox.hpp"
#include "Plot.hpp"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>

#include <QDebug>


Creator::Creator(QWidget *parent)
: QWidget(parent),
  layout_(new QHBoxLayout(this)),
  menu_(new QVBoxLayout),
  plot_(new Plot(this))
{
	layout_->setSpacing(0);

	layout_->setMargin(0);

	layout_->setContentsMargins({0, 0, 0, 0});

	layout_->addLayout(menu_);

	layout_->addWidget(plot_);
}

Creator::~Creator() = default;

void Creator::setupMenu()
{
	menu_->setSpacing(0);

	menu_->setContentsMargins(0, 0, 0, 0);

	menu_->setMargin(0);


	generate_ = new QPushButton("+", this);

	generate_->setToolTip("Generate Wavetable");

	generate_->setObjectName("CreatorGenerateButton");

	generate_->setSizePolicy(QSizePolicy::Maximum,
							QSizePolicy::Expanding);

	generate_->setCursor(Qt::PointingHandCursor);

	menu_->addWidget(generate_);


	save_ = new QPushButton(QString(0x2193), this);

	save_->setToolTip("Save Wavetable");

	save_->setSizePolicy(QSizePolicy::Maximum,
						QSizePolicy::Expanding);

	save_->setCursor(Qt::PointingHandCursor);

	menu_->addWidget(save_);


	bits_ = new ComboBox(this);

	bits_->setToolTip("Bitwidth");

	bits_->setSizePolicy(QSizePolicy::Maximum,
						QSizePolicy::Expanding);

	bits_->setCursor(Qt::PointingHandCursor);

	bits_->addItems({"4", "8", "16"});

	menu_->addWidget(bits_);
}
