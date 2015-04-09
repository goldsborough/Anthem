#include "OperatorUi.hpp"
#include "ModControl.hpp"

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


	QHBoxLayout* top = new QHBoxLayout;

	top->setMargin(0);

	top->setSpacing(0);

	top->setContentsMargins(0,0,0,0);


	QPushButton* wavetable = new QPushButton("2-Bit Sine Wave", this);

	wavetable->setCursor(Qt::PointingHandCursor);

	wavetable->setSizePolicy(QSizePolicy::Expanding,
							 QSizePolicy::Expanding);


	top->addWidget(wavetable);


	QPushButton* activityButton = new QPushButton(QString(title++), this);

	activityButton->setCheckable(true);

	activityButton->setChecked(true);

	activityButton->setCursor(Qt::PointingHandCursor);

	activityButton->setSizePolicy(QSizePolicy::Minimum,
								  QSizePolicy::Minimum);

	connect(activityButton, &QPushButton::toggled,
			[=] (bool) { });

	top->addWidget(activityButton);


	QHBoxLayout* bottom = new QHBoxLayout;

	bottom->setMargin(0);

	bottom->setSpacing(0);

	bottom->setContentsMargins(0,0,0,0);


	ModControl* level = new ModControl("Level", 3, 3, this);

	bottom->addWidget(level);


	ModControl* offset = new ModControl("Offs", 3, 3, this);

	bottom->addWidget(offset);


	ModControl* ratio = new ModControl("Ratio", 3, 3, this);

	bottom->addWidget(ratio);


	QVBoxLayout* layout = new QVBoxLayout(this);

	layout->setMargin(0);

	layout->setSpacing(0);

	layout->setContentsMargins(0,0,0,0);


	layout->addLayout(top);

	layout->addLayout(bottom);
}

void OperatorUi::paintEvent(QPaintEvent*)
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
