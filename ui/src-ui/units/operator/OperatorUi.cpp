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


	QHBoxLayout* layout = new QHBoxLayout(this);

	layout->setMargin(0);

	layout->setSpacing(0);

	layout->setContentsMargins(0,0,0,0);


	ModControl* level = new ModControl("Level", this);

	layout->addWidget(level);

	ModControl* offset = new ModControl("Offs", this);

	layout->addWidget(offset);

	ModControl* ratio = new ModControl("Ratio", this);

	layout->addWidget(ratio);


	QVBoxLayout* activityLayout = new QVBoxLayout;

	activityLayout->setContentsMargins(0,0,0,0);

	activityLayout->setSpacing(0);

	activityLayout->setMargin(0);

	QPushButton* activityButton = new QPushButton(QString(title++));

	activityButton->setCheckable(true);

	activityButton->setChecked(true);

	activityButton->setCursor(Qt::PointingHandCursor);

	activityButton->setSizePolicy(QSizePolicy::Fixed,
								  QSizePolicy::Fixed);

	connect(activityButton, &QPushButton::toggled,
			[=] (bool) { /* setActive() on operator (non-ui operator) */});

	QSpacerItem* spacer = new QSpacerItem(0, 0, QSizePolicy::Maximum,
												QSizePolicy::MinimumExpanding);

	activityLayout->addWidget(activityButton);

	activityLayout->addItem(spacer);

	layout->addLayout(activityLayout);
}

void OperatorUi::paintEvent(QPaintEvent*)
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
