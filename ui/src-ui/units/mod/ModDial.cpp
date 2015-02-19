#include "ModDial.hpp"

ModDial::ModDial(QWidget* parent)
: CustomDial(parent)
{ }

ModDial::ModDial(const QString& text,
                 double factor,
                 QWidget* parent)
: CustomDial(text, factor, parent)
{ }
