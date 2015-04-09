#include "RecordUi.hpp"

#include <QPainter>
#include <QStyleOption>

RecordUi::RecordUi(QWidget* parent)
: QWidget(parent)
{
	setupUi();
}

void RecordUi::setupUi()
{

}

void RecordUi::paintEvent(QPaintEvent*)
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
