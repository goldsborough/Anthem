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


	ModControl* level = new ModControl("Level", 4, 2, this);

	layout->addWidget(level);

	ModControl* offset = new ModControl("Offs", 4, 2, this);

	layout->addWidget(offset);

	ModControl* ratio = new ModControl("Ratio", 4, 2, this);

	layout->addWidget(ratio);


	QPushButton* activityButton = new QPushButton(QString(title++));

	activityButton->setCheckable(true);

	activityButton->setChecked(true);

	activityButton->setCursor(Qt::PointingHandCursor);

	activityButton->setSizePolicy(QSizePolicy::Fixed,
								  QSizePolicy::Expanding);

	connect(activityButton, &QPushButton::toggled,
			[=] (bool) { /* setActive() on operator (non-ui operator) */});

	layout->addWidget(activityButton);
}

void OperatorUi::paintEvent(QPaintEvent*)
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
