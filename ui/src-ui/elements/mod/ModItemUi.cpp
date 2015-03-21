#include "ModItemUi.hpp"
#include "ModUnitUi.hpp"

#include <QPainter>
#include <QPaintEvent>
#include <QMenu>
#include <QPoint>
#include <QMouseEvent>
#include <QApplication>


#include <QDebug>



ModItemUi::ModItemUi(QWidget* parent,
					 double factor,
					 int minimum,
					 int maximum,
					 int dialSpeed)
: QAbstractSlider(parent),
  mod_(nullptr),
  borderPen_(new QPen),// no make_shared, sorry
  lastPosition_(new QPoint),
  borders_(4),
  ratios_(4),
  borderWidth_(0),
  factor_(factor),
  speed_(dialSpeed)
{
	for (int i = 0; i < 4; ++i)
	{
		borders_[i].reset(new QLineF);
	}

	QAbstractSlider::setRange(minimum, maximum);

	setupUi();
}

void ModItemUi::setupUi()
{
	QAbstractSlider::setMouseTracking(false);

	QAbstractSlider::setTracking(true);

	QAbstractSlider::setSizePolicy(QSizePolicy::MinimumExpanding,
								   QSizePolicy::MinimumExpanding);

	// Display current value as tooltip
	connect(this, &QAbstractSlider::valueChanged,
			[=] (int value) { QAbstractSlider::setToolTip(QString::number(value)); });

	QMenu* context = new QMenu(this);

	connect(context->addAction("Insert from Dock A"), &QAction::triggered,
			[=] (bool) { insertModUnitUi({nullptr, "LFO", ModUnitUi::Range::PERIODIC}); });

	connect(context->addAction("Insert from Dock B"), &QAction::triggered,
			[=] (bool) { insertModUnitUi({nullptr, "ENV", ModUnitUi::Range::LINEAR}); });

	connect(context->addAction("Remove"), &QAction::triggered,
			[=] (bool) { removeModUnitUi(); });

	QAbstractSlider::setContextMenuPolicy(Qt::CustomContextMenu);

	connect(this, &QAbstractSlider::customContextMenuRequested,
			[=] (const QPoint& pos)
			{ context->popup(mapToGlobal(pos)); });

	setBorderRatios(1,1,1,1);
}

void ModItemUi::mouseMoveEvent(QMouseEvent* event)
{
	if (mod_)
	{
		if (QAbstractSlider::underMouse())
		{
			emit itemHovered();
		}

		// Mouse tracking is on for hovering, so check
		// if a button was actually pressed because
		// this method is also called when no button
		// is pressed if mouse tracking is enabled
		if (event->buttons() != Qt::NoButton)
		{
			// Get the delta in distance
			int distance = event->pos().y() - lastPosition_->y();

			// Save some function calls
			int value = QAbstractSlider::value();

			// No uneccesary changes
			if ((distance > 0 && value != QAbstractSlider::minimum()) ||
				(distance < 0 && value != QAbstractSlider::maximum()))
			{
				// A positive distance means the mouse moved
				// downwards from the origin, so the dial value
				// must decrease and vice-versa
				QAbstractSlider::setValue(value + ((distance > 0) ? -speed_ : speed_));

				// New value here, must call QAbstractSlider::value()
				emit depthChanged(QAbstractSlider::value() * factor_);
			}

			*lastPosition_ = event->pos();
		}
	}

	else event->ignore();
}

void ModItemUi::mouseDoubleClickEvent(QMouseEvent* event)
{
	// Reset the value on a double click
	QAbstractSlider::setValue(0);

	emit depthChanged(0);

	event->accept();
}

void ModItemUi::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		*lastPosition_ = event->pos();
	}
}

void ModItemUi::paintEvent(QPaintEvent*)
{
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);

    for (unsigned short border = LEFT; border <= BOTTOM; ++border)
    {
        borderPen_->setWidthF(borderWidth_ * ratios_[border]);

        painter.setPen(*borderPen_);

		painter.drawLine(*(borders_[border]));
	}

    painter.drawText(QAbstractSlider::rect(),
                     Qt::AlignCenter,
					 mod_ ? mod_->text : "-");
}

void ModItemUi::setBorderRatios(double left,
                                double right,
                                double top,
                                double bottom)
{
    ratios_[LEFT] = left;

    ratios_[RIGHT] = right;

    ratios_[TOP] = top;

    ratios_[BOTTOM] = bottom;
}

void ModItemUi::setBorderRatio(Side side, double ratio)
{
    ratios_[side] = ratio;
}

double ModItemUi::getBorderRatio(Side side) const
{
    return ratios_[side];
}

void ModItemUi::setBorderWidth(double width)
{
    borderWidth_ = width;
}

double ModItemUi::getBorderWidth() const
{
    return borderWidth_;
}

void ModItemUi::resizeEvent(QResizeEvent* event)
{
	QAbstractSlider::resizeEvent(event);

	unsigned long h = QAbstractSlider::height();

	unsigned long w = QAbstractSlider::width();

	borders_[LEFT]->setLine(0, 0, 0, h);

	borders_[TOP]->setLine(0, 0, w, 0);

	borders_[RIGHT]->setLine(w, 0, w, h);

	borders_[BOTTOM]->setLine(0, h, w, h);
}

void ModItemUi::setBorderColor(const QColor& color)
{
    borderPen_->setColor(color);
}

QColor ModItemUi::getBorderColor() const
{
    return borderPen_->color();
}

void ModItemUi::insertModUnitUi(const ModUnitUi& mod)
{
    mod_.reset(new ModUnitUi(mod));

	repaint();

	emit modUnitInserted(mod);

	QAbstractSlider::setMouseTracking(true);

	QAbstractSlider::setToolTip("0");
}

ModUnitUi ModItemUi::getModUnitUi() const
{
    return *mod_;
}

void ModItemUi::removeModUnitUi()
{
	if (mod_)
	{
		mod_.reset();

		repaint();

		emit modUnitRemoved();

		QAbstractSlider::setMouseTracking(false);

		// Reset the tool tip
		QAbstractSlider::setToolTip(QString());
	}
}

void ModItemUi::setDialSpeed(int speed)
{
	speed_ = speed;
}

int ModItemUi::getDialSpeed() const
{
	return speed_;
}
