#include "AlgorithmUi.hpp"

#include <QPoint>
#include <QGridLayout>
#include <QLabel>
#include <QPainter>
#include <QStyleOption>
#include <QString>
#include <QMouseEvent>


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
  units_(4),
  layout_(new QGridLayout(this)),
  active_(false)
{
	units_[A].reset(new QPoint(a));

	units_[B].reset(new QPoint(b));

	units_[C].reset(new QPoint(c));

	units_[D].reset(new QPoint(d));

	setupUi();
}

void AlgorithmUi::setPosition(Unit unit, const QPoint& pos)
{
	QWidget* item = layout_->itemAtPosition(units_[unit]->x(),
											units_[unit]->y())->widget();

	layout_->removeWidget(item);

	*units_[unit] = pos;

	layout_->addWidget(item, pos.x(), pos.y());
}

QPoint AlgorithmUi::getPosition(Unit unit) const
{
	return *units_[unit];
}

void AlgorithmUi::setActive(bool state)
{
	active_ = state;

	for (std::size_t unit = A; unit <= D; ++unit)
	{
		layout_->itemAt(unit)->widget()->setEnabled(state);
	}
}

bool AlgorithmUi::isActive() const
{
	return active_;
}

void AlgorithmUi::setupUi()
{
	for (std::size_t unit = A; unit <= D; ++unit)
	{
		QString title(static_cast<char>('A' + unit));

		QLabel* label = new QLabel(title, this);

		label->setEnabled(false);

		label->setSizePolicy(QSizePolicy::Fixed,
							 QSizePolicy::Fixed);

		layout_->addWidget(label,
						   units_[unit]->x(),  // row, not "x"
						   units_[unit]->y()); // column, not "y"
	}

	QAbstractButton::setCursor(Qt::PointingHandCursor);

	QAbstractButton::setCheckable(true);

	QAbstractButton::setSizePolicy(QSizePolicy::Expanding,
							   QSizePolicy::Expanding);

	setActive(false);
}

void AlgorithmUi::paintEvent(QPaintEvent*)
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
