#ifndef MODDIAL_HPP
#define MODDIAL_HPP

#include "CustomDial.hpp"

class ModDial : public CustomDial
{

public:

    ModDial(QWidget* parent = nullptr);

    ModDial(const QString& text,
            double factor,
            QWidget* parent = nullptr);

private:

};

#endif // MODDIAL_HPP
