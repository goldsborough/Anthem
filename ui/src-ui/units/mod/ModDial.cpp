#include "ModDial.hpp"

ModDial::ModDial(QWidget* parent)
: CustomDial(parent)
{ }

ModDial::ModDial(const QString& text,
                 double factor,
                 QWidget* parent)
: CustomDial(text, factor, parent)
{
    CustomDial::setMouseTracking(true);
}

void ModDial::mouseMoveEvent(QMouseEvent* event)
{
    CustomDial::mouseMoveEvent(event);
}
