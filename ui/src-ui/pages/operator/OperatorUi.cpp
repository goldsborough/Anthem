#include "OperatorUi.hpp"
#include "ModControl.hpp"
#include "CustomDial.hpp"
#include "ModDial.hpp"
#include "WavetableUi.hpp"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStyleOption>
#include <QPainter>
#include <QPushButton>
#include <QListWidget>
#include <cmath>

#include <QDebug>

OperatorUi::OperatorUi(QWidget* parent)
: QWidget(parent)
{
    setupUi();
}

void OperatorUi::setupUi()
{
	static char title = 'A';

	/* ==================== Top Bar ==================== */

	QHBoxLayout* top = new QHBoxLayout;

	top->setMargin(0);

	top->setSpacing(0);

	top->setContentsMargins(0,0,0,0);


	QPushButton* settings = new QPushButton("Sine", this);

	settings->setSizePolicy(QSizePolicy::Expanding,
							QSizePolicy::Maximum);

	settings->setCursor(Qt::PointingHandCursor);

	top->addWidget(settings);


	QPushButton* activityButton = new QPushButton(QString(title++), this);

	activityButton->setSizePolicy(QSizePolicy::Maximum,
								  QSizePolicy::Maximum);

	activityButton->setCheckable(true);

	activityButton->setChecked(true);

	activityButton->setCursor(Qt::PointingHandCursor);

	connect(activityButton, &QPushButton::toggled,
			[=] (bool) { });

	top->addWidget(activityButton);


	/* ==================== Primary Widget ==================== */

	QWidget* primary = new QWidget(this);

	QHBoxLayout* primaryLayout = new QHBoxLayout(primary);

	primaryLayout->setMargin(0);

	primaryLayout->setSpacing(0);

	primaryLayout->setContentsMargins(0,0,0,0);


	ModControl* level = new ModControl("LEVEL", 3, 3, primary);

	primaryLayout->addWidget(level);


	ModControl* offset = new ModControl("OFFS", 3, 3, primary);

	primaryLayout->addWidget(offset);


	ModControl* ratio = new ModControl("RATIO", 3, 3, primary);

	primaryLayout->addWidget(ratio);


	/* ==================== Secondary Widget ==================== */

	QWidget* secondary = new QWidget(this);

	secondary->setObjectName("SecondaryOperatorUi");

	QGridLayout* secondaryLayout = new QGridLayout(secondary);
/*
	secondaryLayout->setMargin(0);

	secondaryLayout->setSpacing(0);

	secondaryLayout->setContentsMargins(0,0,0,0);*/


	WavetableUi* wavetable = new WavetableUi(this);

	wavetable->setSizePolicy(QSizePolicy::Expanding,
							 QSizePolicy::Expanding);

	secondaryLayout->addWidget(wavetable);

/*
	QListWidget* browser = new QListWidget(secondary);

	browser->setSizePolicy(QSizePolicy::Expanding,
						   QSizePolicy::Expanding);

	browser->setSizeAdjustPolicy(QListWidget::AdjustToContents);

	browser->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	// Removes focus rectangle
	browser->setAttribute(Qt::WA_MacShowFocusRect, false);

	const QList<QString> waves = {"Sine",
								  "Ramp",
								  "Square",
								  "Sawtooth",
								  "Triangle"};

	for (const auto& wave : waves)
	{
		for (int bits = 2; bits <= 64; bits *= 2)
		{
			QListWidgetItem* item = new QListWidgetItem(wave + QString::number(bits));

			item->setTextAlignment(Qt::AlignCenter);

			browser->addItem(item);
		}
	}

	browser->setCursor(Qt::PointingHandCursor);

	browser->setCurrentRow(0);

	connect(browser, &QListWidget::currentTextChanged,
			[=] (const QString& text) { settings->setText(text); });

	secondaryLayout->addWidget(browser, 1, 2);*/


	/* ==================== OperatorUi Settings ==================== */


	QWidget::setSizePolicy(QSizePolicy::Maximum,
						   QSizePolicy::Maximum);

	QVBoxLayout* layout = new QVBoxLayout(this);

	layout->setMargin(0);

	layout->setSpacing(0);

	layout->setContentsMargins(0,0,0,0);


	layout->addLayout(top);

	layout->addWidget(primary);

	layout->addWidget(secondary);

	secondary->hide();

	connect(settings, &QPushButton::clicked,
			[=] (bool)
	{
		if (primary->isHidden())
		{
			secondary->hide();
			primary->show();
		}

		else
		{
			primary->hide();
			secondary->show();
		}
	});
}

void OperatorUi::paintEvent(QPaintEvent*)
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
