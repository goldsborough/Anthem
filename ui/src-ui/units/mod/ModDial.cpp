#include "ModDial.hpp"
#include "ModUnitUi.hpp"

#include <QPainter>
#include <QResizeEvent>


#include <QDebug>


ModDial::ModArc::ModArc() = default;

ModDial::ModArc::ModArc(const ModUnitUi& modUnit)
: mod(new ModUnitUi(modUnit)), arcRect(new QRectF),
  displayedValue(0), angleSpan(0), value(0)
{ }


ModDial::ModDial(QWidget* parent)
: CustomDial(parent)
{ }

ModDial::ModDial(const QString& text,
				 QWidget* parent,
				 double factor,
				 double modFactor,
				 int minimum,
				 int maximum)
: CustomDial(text, parent, factor,
			 minimum, maximum),
  modFactor_(modFactor),
  displayedModArc_(nullptr),
  contentsRect_(new QRectF)
{
    CustomDial::setMouseTracking(true);

	updateValue();

	updateContents_();
}


void ModDial::mouseMoveEvent(QMouseEvent* event)
{
    CustomDial::mouseMoveEvent(event);
}

void ModDial::paintEvent(QPaintEvent*)
{
	QPainter painter(this);

	// So that we can use the background color
	painter.setBackgroundMode(Qt::OpaqueMode);

	// Smooth out the circle
	painter.setRenderHint(QPainter::Antialiasing);

	// Use background color
	painter.setBrush(painter.background());

	// Get current pen before resetting so we have
	// access to the color() method which returns the
	// color from the sytlesheet
	QPen textPen = painter.pen();

	// No border
	painter.setPen(QPen(Qt::NoPen));

	// Draw background circle
	painter.drawEllipse(QDial::rect());

	painter.setPen(textPen);

	// Draw the text / title (e.g. LEVEL)
	painter.drawText(*textRect_,
					 Qt::AlignHCenter | Qt::AlignBottom,
					 displayedModArc_ ? // Check if nullptr
					 displayedModArc_->mod->text : text_);

	// Draw the value of the dial
	painter.drawText(*valueRect_,
					 Qt::AlignCenter,
					 displayedModArc_ ? // Check if nullptr
					 QString::number(displayedModArc_->displayedValue) : valueString_);

	painter.setPen(*arcPen_);

	// Draw the dial value rect
	painter.drawArc(*arcRect_, startAngle_, angleSpan_);

	// The starting angle for modArcs is where the
	// dial value arc ends
	double modStart = startAngle_ + angleSpan_;

	for (QVector<ModArc>::size_type i = 0, end = mods_.size(); i < end; ++i)
	{
		// Skip modArcs with null-ModUnitUis
		if (! mods_[i].mod) continue;

		double span = mods_[i].angleSpan;

		if (mods_[i].mod->range == ModUnitUi::Range::PERIODIC)
		{
			if (span > 0) span = -span;

			// Offset modsStart by one span length
			// (half of the periodic span length)
			modStart -= span;

			if (modStart > startAngle_)
			{
				// Normally span += span, but here
				// add span + the part that's hidden
				// because modStart is already past
				// the startAngle_ (note that span is
				// negative because we're moving ccw)
				span += span + (modStart - startAngle_);

				modStart = startAngle_;
			}

			// Twice the span because
			// left and right modulation
			else span += span;
		}

		// This range check only applies to LINEAR
		// Because the span can be positive
		else if (modStart + span > startAngle_)
		{
			span = startAngle_ - modStart;
		}

		// Range checking, mod arc must not
		// be further than end Angle (these
		// are negative values because we're
		// moving counter-clockwise)
		if (modStart + span < endAngle_)
		{
			span = endAngle_ - modStart;
		}

		// The start angle for
		painter.drawArc(*(mods_[i].arcRect),
						modStart,
						span);
	}
}


void ModDial::addModArc()
{
	mods_.push_back(ModArc());
}

void ModDial::addModArc(const ModUnitUi& mod)
{
	mods_.push_back(mod);
}


void ModDial::insertModArc(index_t index)
{
	mods_.insert(mods_.begin() + index, ModArc());
}

void ModDial::insertModArc(index_t index, const ModUnitUi &mod)
{
	mods_.insert(mods_.begin() + index, mod);
}


void ModDial::setModUnitUiForModArc(index_t index, const ModUnitUi& mod)
{
	mods_[index].mod.reset(new ModUnitUi(mod));
}

void ModDial::removeModunitUiForModArc(index_t index)
{
	mods_[index].mod.reset();

	mods_[index].arcRect.clear();
}

ModUnitUi ModDial::getModUnitUiFromModArc(index_t index)
{
	return *(mods_[index].mod);
}


void ModDial::removeLastModArc()
{
	mods_.erase(mods_.end() - 1);
}

void ModDial::removeModArc(index_t index)
{
	mods_.erase(mods_.begin() + index);
}


ModDial::index_t ModDial::getModArcIndexFromModUnitUiText(const QString& text) const
{
	QVector<ModArc>::const_iterator itr = std::find_if(mods_.begin(),
													   mods_.end(),
													   [&] (const ModArc& modArc)
													   { return modArc.mod->text == text; });

	return std::distance(mods_.begin(), itr);
}

void ModDial::updateContents_()
{
	// arcWidths_ * 2 for padding between arcs
	double arcs = (arcWidth_ * 2) * mods_.size();

	// + dial value rect
	arcs += arcWidth_;

	*contentsRect_ = QRectF(arcs,
							arcs,
							QDial::width() - (2 * arcs),
							QDial::height() - (2 * arcs));

	double heightHalf = contentsRect_->height() / 2;

	*textRect_ = QRectF(contentsRect_->x(),
						contentsRect_->y(),
						contentsRect_->width(),
						heightHalf);

	*valueRect_ = QRectF(contentsRect_->x(),
						 heightHalf,
						 contentsRect_->width(),
						 heightHalf);

	*arcRect_ = QRectF(contentsRect_->x() - (arcWidth_ / 2),
					   contentsRect_->y() - (arcWidth_ / 2),
					   contentsRect_->width() + arcWidth_,
					   contentsRect_->height() + arcWidth_);
}

void ModDial::resizeEvent(QResizeEvent* event)
{
	QDial::setMinimumSize(event->size());

	updateContents_();

	updateModArcRects_();
}

void ModDial::setArcWidth(double px)
{
	arcWidth_ = px;

	arcPen_->setWidth(arcWidth_);

	updateContents_();

	updateModArcRects_();
}

void ModDial::updateModArcRects_()
{
	for(QVector<ModArc>::size_type i = 0, end = mods_.size(); i < end; ++i)
	{
		// i + 1 because there has to be an offset
		// another + 1 because of the dial value rect
		// arcWidth_ * 2 for padding
		double offset = (i + 2) * (arcWidth_ * 2);

		// For dial value rect
		offset += arcWidth_;

		double half = offset / 2;

		*(mods_[i].arcRect) = QRectF(contentsRect_->x() - half,
									 contentsRect_->y() - half,
									 contentsRect_->width() + offset,
									 contentsRect_->height() + offset);
	}
}

void ModDial::setModArcValue(index_t index, double value)
{
	mods_[index].value = value;

	mods_[index].displayedValue = value / modFactor_;

	// Get circle ratio
	double ratio = mods_[index].displayedValue / static_cast<double>(CustomDial::maximum());

	mods_[index].angleSpan = maximumAngleSpan_ * ratio;
}

double ModDial::getModArcValue(index_t index) const
{
	return mods_[index].value;
}

int ModDial::getDisplayedModArcValue(index_t index) const
{
	return mods_[index].displayedValue;
}


void ModDial::showModArc(index_t index)
{
	displayedModArc_ = &(mods_[index]);
}

void ModDial::showControl()
{
	displayedModArc_ = nullptr;
}
