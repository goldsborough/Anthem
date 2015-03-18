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
