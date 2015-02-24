#include "ModDockUi.hpp"
#include "ModItemUi.hpp"
#include "ModUnitUi.hpp"

#include <QGridLayout>
#include <algorithm>

ModDockUi::ModDockUi(QWidget* parent)
: QWidget(parent)
{ }

ModDockUi::ModDockUi(index_t dockSize,
					 QWidget* parent)
: ModDockUi(dockSize, dockSize, parent)
{ }

ModDockUi::ModDockUi(index_t dockSize,                   
                     index_t wrap,
                     QWidget* parent)
: QWidget(parent), wrap_(wrap),
  layout_(nullptr)
{
	setDockSize(dockSize);

    setupUi();
}

void ModDockUi::setDockSize(index_t size)
{
	index_t old = items_.size();

	if (old != size)
	{
		items_.resize(size);

		for (; old < size; ++old)
		{
			ModItemUi* temp = new ModItemUi(this);

			connect(temp, &ModItemUi::depthChanged,
					this, &ModDockUi::emitDepthChanged);

			connect(temp, &ModItemUi::modUnitInserted,
					this, &ModDockUi::emitModUnitInserted);

			connect(temp, &ModItemUi::modUnitRemoved,
					this, &ModDockUi::emitModUnitRemoved);

			connect(temp, &ModItemUi::itemHovered,
					this, &ModDockUi::emitItemHovered);

			items_[old] = temp;
		}

		setupUi();
	}
}

ModDockUi::index_t ModDockUi::getDockSize() const
{
	return items_.size();
}

void ModDockUi::addSpot()
{
	setDockSize(items_.size() + 1);
}

void ModDockUi::removeSpot()
{
	if (! items_.empty())
	{
		setDockSize(items_.size() - 1);
	}
}

void ModDockUi::setupUi()
{
	double top, left, bottom, right;

	// No other way to clear the layout
	delete layout_;

	layout_ = new QGridLayout(this);

	layout_->setHorizontalSpacing(0);

	layout_->setVerticalSpacing(0);

	for (short i = 0, row = -1; i < items_.size(); ++i)
	{
		// Halve the top for the first row
		top = (i < wrap_) ?  1 : 0.5;

		// Left is 1 at wraps else 0.5 (see below)
		left = 1;

		// Halve the bottom for the last row
		bottom = (i < (items_.size() - wrap_)) ? 0.5 : 1;

		// Halve the right side if the next column will wrap
		right = ((i + 1) % wrap_ > 0) ? 0.5 : 1;

		// Wrap to next row at wrap point
		if (i % wrap_ == 0) ++row;

		else left = 0.5; // First column of a row is halved

		items_[i]->setBorderRatios(left, right, top, bottom);

		layout_->addWidget(items_[i], row, i % wrap_);
	}

	if (! items_.empty())
	{
		QSize size = items_[0]->minimumSize();

		QWidget::setMinimumSize(size.height() * layout_->rowCount(),
								size.width() * layout_->columnCount());
	}
}

void ModDockUi::emitDepthChanged(double value) const
{
    ModItemUi* senderItem = dynamic_cast<ModItemUi*>(QWidget::sender());

    for (index_t i = 0; i < items_.size(); ++i)
    {
        if (senderItem == items_[i])
        {
            emit depthChanged(i, value);
        }
    }
}

void ModDockUi::emitModUnitInserted(const ModUnitUi& mod) const
{
    ModItemUi* senderItem = dynamic_cast<ModItemUi*>(QWidget::sender());

    for (index_t i = 0; i < items_.size(); ++i)
    {
        if (senderItem == items_[i])
        {
			emit modUnitInserted(i, mod);
        }
    }
}

void ModDockUi::emitModUnitRemoved() const
{
	ModItemUi* senderItem = dynamic_cast<ModItemUi*>(QWidget::sender());

	for (index_t i = 0; i < items_.size(); ++i)
	{
		if (senderItem == items_[i])
		{
			emit modUnitRemoved(i);
		}
	}
}


void ModDockUi::emitItemHovered() const
{
    ModItemUi* senderItem = dynamic_cast<ModItemUi*>(QWidget::sender());

    for (index_t i = 0; i < items_.size(); ++i)
    {
        if (senderItem == items_[i])
        {
            emit itemHovered(i);
        }
    }
}

void ModDockUi::setWrap(index_t wrap)
{
    wrap_ = wrap;
}

ModDockUi::index_t ModDockUi::getWrap() const
{
    return wrap_;
}

