#ifndef MODUNITUI_HPP
#define MODUNITUI_HPP

#include <QString>

class ModUnit;

// Transports information
struct ModUnitUi
{
    ModUnitUi(ModUnit* mod = nullptr,
              const QString& modText = QString())
    : modUnit(mod),
      text(modText)
    { }

    ModUnit* modUnit;

    QString text;
};

#endif // MODUNITUI_HPP
