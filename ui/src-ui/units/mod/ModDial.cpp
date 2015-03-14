#include "ModDial.hpp"
#include "ModUnitUi.hpp"

#include <QPainter>
#include <QResizeEvent>
#include <cmath>

#include <QDebug>


ModDial::ModArc::ModArc() = default;

ModDial::ModArc::ModArc(const ModUnitUi& modUnit)
: mod(new ModUnitUi(modUnit)),
  arcRect(new QRectF),
  angleSpan(0),
  displayedValue(0),
  value(0)
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
: CustomDial(text,
			 parent,
			 factor,
			 minimum,
			 maximum),
  arcPadding_(0),
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
					 Qt::AlignCenter,
					 displayedModArc_ ? // Check if nullptr
					 displayedModArc_->mod->text : text_);
/*
	// Draw the value of the dial
	painter.drawText(*valueRect_,
					 Qt::AlignCenter,
					 displayedModArc_ ? // Check if nullptr
					 QString::number(displayedModArc_->displayedValue) : valueString_);
*/
	painter.setPen(*arcPen_);

	// Draw the dial value rect
	painter.drawArc(*arcRect_, startAngle_, angleSpan_);

	QPen temp(*arcPen_);

	temp.setColor("#27272B");

	painter.setPen(temp);

	for (QVector<ModArc>::size_type i = 0, end = mods_.size(); i < end; ++i)
	{
		// Skip modArcs with null-ModUnitUis
		if (! mods_[i].mod) continue;

		// The starting angle for modArcs is where the
		// dial value arc ends
		double modStart = startAngle_ + angleSpan_;

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
	static const double sin45 = 0.7071067811865476;

	// Calculate opposite side of triangle
	// - arcWidth to have the rect really "inside"
	double side = sin45 * (arcRect_->width() / 2) - arcWidth_;

	double pos = (QDial::width() / 2) - side;

	// Need the diameter
	side *= 2;

	*contentsRect_ = QRectF(pos,
							pos,
							side,
							side);

	*textRect_ = QRectF(contentsRect_->left(),
						contentsRect_->top(),
						contentsRect_->width(),
						contentsRect_->height() / 2);

	*valueRect_ = QRectF(textRect_->left(),
						 textRect_->bottom(),
						 textRect_->width(),
						 textRect_->height());
}

void ModDial::resizeEvent(QResizeEvent* event)
{
	QDial::setMinimumSize(event->size());

	updateArcRects_();

	updateContents_();
}

void ModDial::setArcWidth(double px)
{
	arcWidth_ = px;

	arcPen_->setWidth(arcWidth_);

	updateContents_();

	updateArcRects_();
}

void ModDial::updateArcRects_()
{
	// Initial offset for furthest out arc
	// because the arc is drawn left and right
	// of the arc (it's just a border) and we
	// offset it by half to get the full arc
	// inside the dial rect
	double offset = arcWidth_ / 2;

	for(QVector<ModArc>::size_type i = 0, end = mods_.size(); i < end; ++i)
	{
		if (i > 0) offset += arcWidth_ + arcPadding_;

		*(mods_[i].arcRect) = QRectF(offset,
									 offset,
									 QDial::width() - (2 * offset),
									 QDial::height() - (2 * offset));
	}

	offset += arcWidth_ + arcPadding_;

	*arcRect_ = QRectF(offset,
					   offset,
					   QDial::width() - (2 * offset),
					   QDial::height() - (2 * offset));
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

void ModDial::setArcPadding(double padding)
{
	arcPadding_ = padding;
}

double ModDial::getArcPadding() const
{
	return arcPadding_;
}
