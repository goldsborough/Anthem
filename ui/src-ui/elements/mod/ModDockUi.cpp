#include "ModDockUi.hpp"
#include "ModItemUi.hpp"
#include "ModUnitUi.hpp"

#include <QGridLayout>
#include <QPaintEvent>
#include <QStyleOption>
#include <QPainter>
#include <algorithm>

ModDockUi::ModDockUi(index_t dockSize,
					 QWidget* parent)
: ModDockUi(dockSize, dockSize, parent)
{ }

ModDockUi::ModDockUi(index_t dockSize,                   
                     index_t wrap,
                     QWidget* parent)
: QWidget(parent),
  wrap_(wrap),
  items_(dockSize)
{
    setupUi();
}

void ModDockUi::setupUi()
{
	double top, left, bottom, right;

	QGridLayout* layout = new QGridLayout();

	for (short i = 0, row = -1; i < items_.size(); ++i)
	{
		items_[i] = new ModItemUi(this);

		connect(items_[i], &ModItemUi::depthChanged,
				[=] (double value) { emit depthChanged(i, value); });

		connect(items_[i], &ModItemUi::modUnitInserted,
				[=] (const ModUnitUi& mod) { emit modUnitInserted(i, mod); });

		connect(items_[i], &ModItemUi::modUnitRemoved,
				[=] () { emit modUnitRemoved(i); });

		connect(items_[i], &ModItemUi::itemHovered,
				[=] () { emit itemHovered(i); });

		// Halve the top for all but the first row
		top = (i < wrap_) ?  1 : 0.5;

		// Left is 1 for all but first, else 0.5 (see below)
		left = 1;

		// Halve the bottom for all but the last row
		bottom = (i < (items_.size() - wrap_)) ? 0.5 : 1;

		// Halve the right side if the next column will wrap
		right = ((i + 1) % wrap_ > 0) ? 0.5 : 1;

		// Wrap to next row at wrap point
		// This is true for i = 0, that's why
		// row is equal to -1 initially
		if (i % wrap_ == 0) ++row;

		else left = 0.5; // First column of a row is halved

		items_[i]->setBorderRatios(left, right, top, bottom);

		layout->addWidget(items_[i], row, i % wrap_);
	}

	layout->setSpacing(0);

	layout->setContentsMargins(0,0,0,0);

	setLayout(layout);

	setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
}

void ModDockUi::paintEvent(QPaintEvent*)
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);

	QPen temp = p.pen();

	p.setPen({QColor("blue")});

	p.drawRect(0, 0, width() - 2, height() - 2);

	p.setPen(temp);

	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

