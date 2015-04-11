#ifndef MODUNITUI_HPP
#define MODUNITUI_HPP

#include <QString>

class ModUnit;

struct ModUnitUi
{	
	enum class Range { NONE, LINEAR, PERIODIC };

    ModUnitUi(ModUnit* mod = nullptr,
			  const QString& text = QString(),
			  Range valueRange = Range::NONE)
	: modUnit(mod),
	  id(text),
	  range(valueRange)
    { }

	ModUnit* modUnit;

	QString id;

	Range range;
};

#endif // MODUNITUI_HPP
