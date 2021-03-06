#include "ModDial.hpp"
#include "ModUnitUi.hpp"

#include <QPainter>
#include <QResizeEvent>
#include <QColor>

#include <cmath>

ModDial::ModArc::ModArc()
: mod(nullptr),
  arcRect(new QRectF)
{ }

ModDial::ModArc::ModArc(const ModUnitUi& modUnit)
: mod(new ModUnitUi(modUnit)),
  arcRect(new QRectF)
{ }


ModDial::ModDial(QWidget* parent)
:  Dial(parent)
{ }

ModDial::ModDial(const QString& text,
				 QWidget* parent,
				 index_t numberOfArcs,
				 double factor,
				 double modFactor,
				 int minimum,
				 int maximum)
:  Dial(text,
			 parent,
			 factor,
			 minimum,
			 maximum),
  activeModArcs_(0),
  arcPadding_(0),
  modFactor_(modFactor),
  displayedModArc_(nullptr),
  mods_(numberOfArcs),
  contentsRect_(new QRectF),
  displayedArcColor_(new QColor)
{
	setupUi();
}

void ModDial::setupUi()
{
	 Dial::setMouseTracking(true);

	setSizePolicy(QSizePolicy::Fixed,
				  QSizePolicy::Fixed);

	updateValue();

	updateArcRects_();

	updateContents_();
}

void ModDial::mouseMoveEvent(QMouseEvent* event)
{
	 Dial::mouseMoveEvent(event);

	if ( Dial::underMouse())
	{
		showControl();
	}
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
					 displayedModArc_->mod->id : text_);

	int value;

	// Boundary checking for displayed value of mod arc
	if (displayedModArc_)
	{
		if (displayedModArc_->value > 0)
		{
			value = displayedModArc_->displayedValue;

			if (( Dial::value() + value) >  Dial::maximum())
			{
				value =  Dial::maximum() -  Dial::value();
			}
		}

		else
		{
			value = displayedModArc_->displayedValue;

			if (( Dial::value() + value) <  Dial::minimum())
			{
				value = - Dial::value();
			}
		}
	}

	else value =  Dial::value();

	// Draw the current value of the dial
	painter.drawText(*valueRect_,
					 Qt::AlignCenter,
					 QString::number(value));

	painter.setPen(*arcPen_);

	// Draw the dial setting arc
	painter.drawArc(*arcRect_, startAngle_, angleSpan_);

	for (std::size_t i = 0, end = mods_.size(); i < end; ++i)
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

			// Offset modStart by one span length
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

		QPen pen(*arcPen_);

		if (displayedModArc_ && displayedModArc_ == &(mods_[i]))
		{
			pen.setColor(*displayedArcColor_);
		}

		else pen.setColor(*arcColor_);

		painter.setPen(pen);

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

	activeModArcs_++;

	updateArcRects_();

	updateContents_();

	QDial::update();
}

void ModDial::removeModUnitUiForModArc(index_t index)
{
	mods_[index].mod.reset();

	activeModArcs_--;

	updateArcRects_();

	updateContents_();

	showControl();

	QDial::update();
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


ModDial::index_t ModDial::getModArcIndexFromModUnitUiId(const QString& id) const
{
	QVector<ModArc>::const_iterator itr = std::find_if(mods_.begin(),
													   mods_.end(),
													   [&] (const ModArc& modArc)
													   { return modArc.mod->id == id; });

	return std::distance(mods_.begin(), itr);
}

ModDial::index_t ModDial::activeModArcs() const
{
	return activeModArcs_;
}

void ModDial::updateContents_()
{	
	static const double sin45 = 0.7071067811865476;

	double radius = arcRect_->width() / 2;

	// Calculate opposite side of triangle
	// - arcWidth to have the rect really "inside"
	double heightHalf = sin45 * radius - arcWidth_;

	// Divide by two to lower the opposite side
	// for the following calculation of widthHalf
	// because we want to center the text
	if (! valueShown_) heightHalf /= 2;

	// Do some trigonometry to get the adjacent side
	double widthHalf = sqrt((radius*radius) - (heightHalf*heightHalf));

	// Need the full radius here, including
	// all modArcs (other radius is relative
	// to the control arc's rect)
	double dialRadius = QDial::width() / 2;

	*contentsRect_ = QRectF(dialRadius - widthHalf,
							dialRadius - heightHalf,
							widthHalf * 2,
							heightHalf * 2);

	*textRect_ = QRectF(contentsRect_->left(),
						contentsRect_->top(),
						contentsRect_->width(),
						(valueShown_) ? heightHalf : heightHalf * 2);

	if (valueShown_)
	{
		*valueRect_ = QRectF(textRect_->left(),
							 textRect_->bottom(),
							 textRect_->width(),
							 textRect_->height());
	}
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

	updateArcRects_();

	updateContents_();
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
		if (mods_[i].mod)
		{
			offset += arcPadding_;

			if (i > 0) offset += arcWidth_;

			*(mods_[i].arcRect) = QRectF(offset,
										 offset,
										 QDial::width() - (2 * offset),
										 QDial::height() - (2 * offset));
		}

		else *(mods_[i].arcRect) = QRectF();
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
	double ratio = mods_[index].displayedValue /
				   static_cast<double>( Dial::maximum());

	mods_[index].angleSpan = maximumAngleSpan_ * ratio;

	QDial::update();
}

void ModDial::resetModArcValues()
{
	for (index_t i = 0; i < mods_.size(); ++i)
	{
		setModArcValue(i, 0);
	}
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
	if (! displayedModArc_ || displayedModArc_ != &(mods_[index]))
	{
		displayedModArc_ = &(mods_[index]);

		arcPen_->setColor(*arcColor_);

		QDial::update();
	}
}

void ModDial::showControl()
{
	if (activeModArcs_)
	{
		arcPen_->setColor(*displayedArcColor_);
	}

	else arcPen_->setColor(*arcColor_);

	if (displayedModArc_)
	{
		displayedModArc_ = nullptr;

		QDial::update();
	}
}

void ModDial::setArcPadding(double padding)
{
	arcPadding_ = padding;
}

double ModDial::getArcPadding() const
{
	return arcPadding_;
}

void ModDial::setDisplayedArcColor(const QColor &color)
{
	*displayedArcColor_ = color;
}

QColor ModDial::getDisplayedArcColor() const
{
	return *displayedArcColor_;
}
