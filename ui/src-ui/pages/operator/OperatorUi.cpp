#include "OperatorUi.hpp"
#include "ModControl.hpp"
#include "CustomDial.hpp"
#include "ModDial.hpp"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStyleOption>
#include <QPainter>
#include <QPushButton>

#include <QDebug>

OperatorUi::OperatorUi(QWidget* parent)
    : QWidget(parent)
{
    setupUi();
}

void OperatorUi::setupUi()
{
	static char title = 'A';


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



	QWidget* secondary = new QWidget(this);

	secondary->setObjectName("SecondaryOperatorUi");

	secondary->hide();

	QGridLayout* secondaryLayout = new QGridLayout(secondary);
/*
	secondaryLayout->setMargin(0);

	secondaryLayout->setSpacing(0);

	secondaryLayout->setContentsMargins(0,0,0,0);*/


	CustomDial* phase = new CustomDial("PHASE", secondary, 1, 0, 359);

	phase->setSizePolicy(QSizePolicy::Fixed,
						 QSizePolicy::Fixed);

	secondaryLayout->addWidget(phase, 0, 0);

	CustomDial* partials = new CustomDial("PARTIALS", secondary, 1, 1, 6, true);

	partials->setSizePolicy(QSizePolicy::Fixed,
							QSizePolicy::Fixed);

	secondaryLayout->addWidget(partials, 0, 1);


	QPushButton* sigma = new QPushButton("SIGMA", this);

	sigma->setCheckable(true);

	sigma->setCursor(Qt::PointingHandCursor);

	connect(sigma, &QPushButton::clicked,
			[=] (bool checked) { qDebug() << checked; });

	secondaryLayout->addWidget(sigma, 1, 0);


	QPushButton* custom = new QPushButton("CUSTOM", this);

	custom->setCheckable(true);

	custom->setCursor(Qt::PointingHandCursor);

	connect(custom , &QPushButton::clicked,
			[=] (bool checked) { qDebug() << checked; });

	secondaryLayout->addWidget(custom, 1, 1);


	QHBoxLayout* top = new QHBoxLayout;

	top->setMargin(0);

	top->setSpacing(0);

	top->setContentsMargins(0,0,0,0);


	QPushButton* settings = new QPushButton("Sine Wave", this);

	settings->setCursor(Qt::PointingHandCursor);

	settings->setSizePolicy(QSizePolicy::Expanding,
							QSizePolicy::Expanding);

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


	top->addWidget(settings);


	QPushButton* activityButton = new QPushButton(QString(title++), this);

	activityButton->setCheckable(true);

	activityButton->setChecked(true);

	activityButton->setCursor(Qt::PointingHandCursor);

	activityButton->setSizePolicy(QSizePolicy::Minimum,
								  QSizePolicy::Minimum);

	connect(activityButton, &QPushButton::toggled,
			[=] (bool) { });

	top->addWidget(activityButton);


	QVBoxLayout* layout = new QVBoxLayout(this);

	layout->setMargin(0);

	layout->setSpacing(0);

	layout->setContentsMargins(0,0,0,0);


	layout->addLayout(top);

	layout->addWidget(primary);

	layout->addWidget(secondary);
}

void OperatorUi::paintEvent(QPaintEvent*)
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
