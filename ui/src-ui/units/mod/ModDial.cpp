#include "ModDial.hpp"

#include <stdexcept>

ModDial::ModDial(QWidget* parent)
: CustomDial(parent)
{ }

ModDial::ModDial(const QString& text,
                 QWidget* parent,
                 double factor,
                 int minimum,
                 int maximum)
: CustomDial(text, parent, factor,
             minimum, maximum)
{
    CustomDial::setMouseTracking(true);
}

void ModDial::mouseMoveEvent(QMouseEvent* event)
{
    CustomDial::mouseMoveEvent(event);
}

void ModDial::addModArc(Range range, const QString& text)
{

}

void ModDial:removeModArc(index_t index)
{
    mods_.erase(mods_.begin() + index);
}

ModDial::index_t ModDial::getIndexFromModArcText(const QString& text) const
{
    QVector::const_iterator itr = std::find_if(mods_.begin(),
                                               mods_.end(),
                                               [&] (const ModArc& mod)
                                               { return mod.text == text; });

    if (itr == mods_.end())
    {
        throw std::runtime_error("Error finding index for invalid mod arc text!");
    }

    else return std::distance(mods_.begin(), itr);
}

void setModArcText(const QString& text);

QString getModArcText() const;


void setModArcRange(Range range);

Range getModArcRange() const;


void setModArcValue(double value);

double getScaledModArcValue() const;

double getDisplayedModArcValue() const;


void showModArcValue(index_t index);

void showControlValue();
