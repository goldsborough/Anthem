#include "ModControl.hpp"
#include "ModDockUi.hpp"
#include "ModDial.hpp"
#include "ModUnitUi.hpp"

#include <QStyleOption>
#include <QPainter>
#include <QVBoxLayout>
#include <QMouseEvent>

#include <QDebug>

ModControl::ModControl(const QString& title, QWidget* parent)
: QWidget(parent),
  title_(title.toUpper())
{
    setupUi();

	QWidget::setMouseTracking(true);
}

ModControl::~ModControl()
{ }

void ModControl::setTitle(const QString& title)
{
	title_ = title;
}

QString ModControl::getTitle() const
{
	return title_;
}

void ModControl::setupUi()
{
	QVBoxLayout* layout = new QVBoxLayout(this);

	ModDial* dial = new ModDial(title_, this, 4);
/*
	ModUnitUi mod{nullptr, "LFO A", ModUnitUi::Range::PERIODIC};

	dial->addModArc(mod);

	dial->setModArcValue(0, 0.25);


	dial->addModArc({nullptr, "LFO B", ModUnitUi::Range::LINEAR});

	dial->setModArcValue(1, -0.5);


	dial->addModArc({nullptr, "ENV C", ModUnitUi::Range::LINEAR});

	dial->setModArcValue(2, 0.2);

	dial->addModArc({nullptr, "ENV D", ModUnitUi::Range::LINEAR});

	dial->setModArcValue(3, 0.8);
*/

	layout->addWidget(dial);

	ModDockUi* dock = new ModDockUi(4, 2, this);

	connect(dock, &ModDockUi::depthChanged,
			[=] (ModDockUi::index_t index, double value)
			{
				dial->setModArcValue(index, value);
				emit depthChanged(index, value);
			});

	connect(dock, &ModDockUi::modUnitInserted,
			[=] (ModDockUi::index_t index, const ModUnitUi& mod)
			{
				emit modUnitInserted(index, mod);
				dial->setModUnitUiForModArc(index, mod);
			});

	connect(dock, &ModDockUi::modUnitRemoved,
			[=] (ModDockUi::index_t index)
			{
				dial->removeModunitUiForModArc(index);
				emit modUnitRemoved(index);
			});

	connect(dock, &ModDockUi::itemHovered,
			[=] (ModDockUi::index_t index)
			{ dial->showModArc(index); });

	// Adding the dock in a grid layout centers
	// it in the parent layout
	QGridLayout* dockLayout = new QGridLayout;

	dockLayout->addWidget(dock);

	layout->addLayout(dockLayout);

	setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}
