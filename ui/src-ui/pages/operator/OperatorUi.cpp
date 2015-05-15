#include "OperatorUi.hpp"
#include "ModControl.hpp"
#include "CustomDial.hpp"
#include "ModDial.hpp"
#include "WavetableUi.hpp"
#include "BrowserUi.hpp"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStyleOption>
#include <QPainter>
#include <QPushButton>
#include <QListWidget>
#include <QTabWidget>
#include <QLabel>
#include <QToolTip>

#include <QDebug>

OperatorUi::OperatorUi(QWidget* parent)
: QWidget(parent)
{
	layout_ = new QVBoxLayout(this);

	layout_->setMargin(0);

	layout_->setSpacing(0);

	layout_->setContentsMargins(0,0,0,0);


	setupBar();

	setupPrimary();

	setupSecondary();

	QWidget::setSizePolicy(QSizePolicy::Maximum,
						   QSizePolicy::Maximum);
}

void OperatorUi::setupBar()
{
	static char title = 'A';


	auto top = new QHBoxLayout;

	top->setMargin(0);

	top->setSpacing(0);

	top->setContentsMargins(0,0,0,0);


	toggle_ = new QPushButton("Sine", this);

	toggle_->setSizePolicy(QSizePolicy::Expanding,
						  QSizePolicy::Maximum);

	toggle_->setCursor(Qt::PointingHandCursor);

	top->addWidget(toggle_);


	connect(toggle_, &QPushButton::clicked,
			[=] (bool)
	{
		if (primary_->isHidden())
		{
			secondary_->hide();
			primary_->show();
		}

		else
		{
			primary_->hide();
			secondary_->show();
		}
	});


	auto activityButton = new QPushButton(QString(title++), this);

	activityButton->setSizePolicy(QSizePolicy::Maximum,
								  QSizePolicy::Maximum);

	activityButton->setCheckable(true);

	activityButton->setChecked(true);

	activityButton->setCursor(Qt::PointingHandCursor);

	connect(activityButton, &QPushButton::toggled,
			[=] (bool) { });

	top->addWidget(activityButton);


	layout_->addLayout(top);
}

void OperatorUi::setupPrimary()
{
	primary_ = new QWidget(this);


	auto primaryLayout = new QHBoxLayout(primary_);

	primaryLayout->setMargin(0);

	primaryLayout->setSpacing(0);

	primaryLayout->setContentsMargins(0,0,0,0);


	ModControl* level = new ModControl("LEVEL", 3, 3, primary_);

	primaryLayout->addWidget(level);


	ModControl* offset = new ModControl("OFFS", 3, 3, primary_);

	primaryLayout->addWidget(offset);


	ModControl* ratio = new ModControl("RATIO", 3, 3, primary_);

	primaryLayout->addWidget(ratio);


	layout_->addWidget(primary_);
}

void OperatorUi::setupSecondary()
{
	secondary_ = new QTabWidget(this);

	secondary_->setMovable(true);

	secondary_->tabBar()->setCursor(Qt::PointingHandCursor);

	secondary_->setTabPosition(QTabWidget::South);


	setupWavesTab();

	setupCustomTab();


	layout_->addWidget(secondary_);

	secondary_->hide();
}

void OperatorUi::setupWavesTab()
{
	auto waves = new QWidget(secondary_);

	waves->setObjectName("Waves");

	secondary_->addTab(waves, "Waves");


	auto layout = new QHBoxLayout(waves);

	layout->setMargin(10);

	//layout->setSpacing(0);


	auto browser = new BrowserUi(this);

	connect(browser, &BrowserUi::wavetableSelected,
			[=] (const QString& id) { toggle_->setText(id); });

	layout->addWidget(browser);


	auto wavetable = new WavetableUi(waves);

	wavetable->setSizePolicy(QSizePolicy::Expanding,
							 QSizePolicy::Expanding);

	layout->addWidget(wavetable);

	connect(wavetable, &WavetableUi::phaseChanged,
			[=] (int degrees)
			{
				QToolTip::showText(wavetable->cursor().pos(),
								   ((degrees > 0) ? "+" : "") +
								   QString::number(degrees)   +
								   QString(0x00B0));
			});

}

void OperatorUi::setupCustomTab()
{
	auto custom = new QWidget(this);

	secondary_->addTab(custom, "Custom");
}

void OperatorUi::paintEvent(QPaintEvent*)
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
