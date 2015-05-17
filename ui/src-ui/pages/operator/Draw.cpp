#include "Draw.hpp"

#include <QStyleOption>
#include <QPainter>

Draw::Draw(QWidget* parent)
: Creator(parent)
{
	setupMenu();

	setupPlot();
}

void Draw::setupPlot()
{

}

void Draw::paintEvent(QPaintEvent *)
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
