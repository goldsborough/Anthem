#include "OperatorUi.hpp"
#include "ModControl.hpp"

#include <QGridLayout>
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
	QGridLayout* layout = new QGridLayout(this);

	layout->setMargin(0);

	layout->setSpacing(0);

	layout->setContentsMargins(0,0,0,0);

	ModControl* level = new ModControl("Level", this);

	layout->addWidget(level);

	ModControl* offset = new ModControl("Offs", this);

    layout->addWidget(offset, 0, 1);

	ModControl* ratio = new ModControl("Ratio", this);

	layout->addWidget(ratio, 0, 2);

}

void OperatorUi::paintEvent(QPaintEvent*)
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
