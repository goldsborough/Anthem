#include "ModDial.hpp"
#include "ModUnitUi.hpp"

#include <QPainter>

ModDial::ModArc::ModArc() = default;

ModDial::ModArc::ModArc(const ModUnitUi& modUnit)
: mod(new ModUnitUi(modUnit)), arcRect(new QRectF),
  displayedValue(0), scaledValue(0)
{ }


ModDial::ModDial(QWidget* parent)
: CustomDial(parent)
{ }

ModDial::ModDial(const QString& text,
				 QWidget* parent,
				 double factor,
				 int minimum,
				 int maximum)
: CustomDial(text, parent, factor,
			 minimum, maximum),
  displayedModArc_(nullptr)
{
    CustomDial::setMouseTracking(true);
}


void ModDial::mouseMoveEvent(QMouseEvent* event)
{
    CustomDial::mouseMoveEvent(event);
}

void ModDial::paintEvent(QPaintEvent* event)
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

	painter.drawText(*textRect_,
					 Qt::AlignHCenter | Qt::AlignBottom,
					 displayedModArc_ ? // Check if nullptr
					 displayedModArc_->mod->text : text_);

	painter.drawText(*valueRect_,
					 Qt::AlignCenter,
					 displayedModArc_ ? // Check if nullptr
					 QString::number(displayedModArc_->displayedValue) : valueString_);

	painter.setPen(*arcPen_);

	painter.drawArc(*arcRect_, startAngle_, angleSpan_);
}

void ModDial::updateValue()
{
	CustomDial::updateValue();


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


void ModDial::setModArcValue(index_t index, double value)
{
	// Bounds checking included in QVector::at()
	mods_[index].scaledValue = value;

	// Bounds checking already performed
	mods_[index].displayedValue = value * factor_;
}

double ModDial::getScaledModArcValue(index_t index) const
{
	return mods_[index].scaledValue;
}

double ModDial::getDisplayedModArcValue(index_t index) const
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
