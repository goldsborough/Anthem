#include "Creator.hpp"
#include "ComboBox.hpp"
#include "AdditiveWavetableUi.hpp"
#include "PartialsUi.hpp"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>

#include <QDebug>


Creator::Creator(QWidget *parent)
: QWidget(parent),
  layout_(new QHBoxLayout(this)),
  menu_(new QVBoxLayout),
  wavetable_(new AdditiveWavetableUi(this)),
  partials_(new PartialsUi(this))
{
	setupMenu();

	layout_->setSpacing(0);

	layout_->setContentsMargins({0, 0, 0, 20});

	layout_->addLayout(menu_);


	QWidget::setSizePolicy(QSizePolicy::Expanding,
						   QSizePolicy::Maximum);

	auto plots = new QVBoxLayout;

	partials_->setSizePolicy(QSizePolicy::Expanding,
							 QSizePolicy::Expanding);

	plots->addWidget(partials_);

	wavetable_->setSizePolicy(QSizePolicy::Expanding,
							  QSizePolicy::Expanding);

	plots->addWidget(wavetable_);

	layout_->addLayout(plots);


	setupConnections();
}

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


	sigma_ = new QPushButton(QString(0x03C3), this);

	sigma_->setToolTip("Sigma Approximation");

	sigma_->setSizePolicy(QSizePolicy::Maximum,
						  QSizePolicy::Expanding);

	sigma_->setCheckable(true);

	sigma_->setCursor(Qt::PointingHandCursor);

	menu_->addWidget(sigma_);


	bits_ = new ComboBox(this);

	bits_->setToolTip("Bitwidth");

	bits_->setSizePolicy(QSizePolicy::Maximum,
						QSizePolicy::Expanding);

	bits_->setCursor(Qt::PointingHandCursor);

	bits_->addItems({"4", "8", "16"});

	bits_->setCurrent(2);

	menu_->addWidget(bits_);


	number_ = new ComboBox(this);

	number_->setToolTip("Partials");

	number_->setSizePolicy(QSizePolicy::Maximum,
							QSizePolicy::Expanding);

	number_->setCursor(Qt::PointingHandCursor);

	number_->addItems({"8", "16", "32", "64"});

	menu_->addWidget(number_);
}

void Creator::setupConnections()
{
	connect(generate_, &QPushButton::clicked,
			wavetable_, &AdditiveWavetableUi::generate);

	connect(save_, &QPushButton::clicked,
			wavetable_, &AdditiveWavetableUi::save);

	connect(sigma_, &QPushButton::clicked,
			wavetable_, &AdditiveWavetableUi::setSigmaApproximationEnabled);

	connect(bits_, &ComboBox::currentChanged,
			[=] (const QString& text)
	{ wavetable_->setBitwidth(text.toInt()); });

	connect(number_, &ComboBox::currentChanged,
			[=] (const QString& text)
	{
		int number = text.toInt();

		partials_->setNumberOfPartials(number);

		wavetable_->setActivePartials(number);
	});

	connect(partials_, &PartialsUi::amplitudeChanged,
			wavetable_, &AdditiveWavetableUi::setAmplitude);

	connect(partials_, &PartialsUi::allAmplitudesCleared,
			wavetable_, &AdditiveWavetableUi::clear);

}

void Creator::paintEvent(QPaintEvent*)
{
	QStyleOption option;

	option.init(this);

	QPainter painter(this);

	style()->drawPrimitive(QStyle::PE_Widget, &option, &painter, this);
}
