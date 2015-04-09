#include "VolumeUi.hpp"
#include "ModControl.hpp"

#include <QPainter>
#include <QStyleOption>

VolumeUi::VolumeUi(QWidget* parent)
: QWidget(parent)
{
	setupUi();
}

void VolumeUi::setupUi()
{
	ModControl* volume = new ModControl("Volume", 2, 2, this);
}

void VolumeUi::paintEvent(QPaintEvent*)
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
