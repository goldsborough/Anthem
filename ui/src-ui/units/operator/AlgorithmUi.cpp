#include "AlgorithmUi.hpp"

#include <QPoint>
#include <QGridLayout>
#include <QLabel>
#include <QPainter>
#include <QStyleOption>
#include <QString>


#include <QDebug>



AlgorithmUi::AlgorithmUi(QWidget* parent)
: QAbstractButton(parent)
{ }

AlgorithmUi::AlgorithmUi(const QPoint& a,
						 const QPoint& b,
						 const QPoint& c,
						 const QPoint& d,
						 QWidget* parent)
: QAbstractButton(parent),
  layout_(new QGridLayout(this))
{
	positions_[A].reset(new QPoint(a));

	positions_[B].reset(new QPoint(b));

	positions_[C].reset(new QPoint(c));

	positions_[D].reset(new QPoint(d));

	setupUi();
}

void AlgorithmUi::setPosition(Unit unit, const QPoint& pos)
{
	QWidget* item = layout_->itemAtPosition(positions_[unit]->x(),
											positions_[unit]->y())->widget();

	layout_->removeWidget(item);

	*positions_[unit] = pos;

	layout_->addWidget(item, pos.x(), pos.y());
}

QPoint AlgorithmUi::getPosition(Unit unit) const
{
	return *positions_[unit];
}

void AlgorithmUi::setupUi()
{
	for (std::size_t unit = A; unit <= D; ++unit)
	{
		QString title(static_cast<char>('A' + unit));

		QLabel* label = new QLabel(title, this);

		label->setSizePolicy(QSizePolicy::Fixed,
							 QSizePolicy::Fixed);

		layout_->addWidget(label,
						   positions_[unit]->x(),
						   positions_[unit]->y());
	}

	QAbstractButton::setCursor(Qt::PointingHandCursor);

	QAbstractButton::setCheckable(true);

	QAbstractButton::setSizePolicy(QSizePolicy::Expanding,
							   QSizePolicy::Expanding);

	connect(this, &QAbstractButton::clicked,
			[=](){ qDebug() << "!"; });
}

void AlgorithmUi::paintEvent(QPaintEvent*)
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
