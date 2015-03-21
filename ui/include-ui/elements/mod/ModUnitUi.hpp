#ifndef MODUNITUI_HPP
#define MODUNITUI_HPP

#include <QString>

class ModUnit;

// Transports information
struct ModUnitUi
{
	enum class Range { NONE, LINEAR, PERIODIC };

    ModUnitUi(ModUnit* mod = nullptr,
			  const QString& textString = QString(),
			  Range valueRange = Range::NONE)
	: modUnit(mod),
	  text(textString),
	  range(valueRange)
    { }

	ModUnit* modUnit;

	QString text;

	Range range;
};

#endif // MODUNITUI_HPP
