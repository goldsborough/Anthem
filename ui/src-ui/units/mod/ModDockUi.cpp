#include "ModDockUi.hpp"
#include "ModItemUi.hpp"
#include "ModUnitUi.hpp"

#include <QGridLayout>
#include <algorithm>

ModDockUi::ModDockUi(QWidget* parent)
: QWidget(parent)
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

ModDockUi::~ModDockUi()
{ }

void ModDockUi::setupUi()
{
    QGridLayout* layout = new QGridLayout(this);

    layout->setHorizontalSpacing(0);

    layout->setVerticalSpacing(0);

    for (short i = 0, row = -1; i < items_.size(); ++i)
    {
        items_[i] = new ModItemUi(ModUnitUi(nullptr, "-"), this);

        connect(items_[i], &ModItemUi::depthChanged,
                this, &ModDockUi::emitDepthChanged);

        connect(items_[i], &ModItemUi::modUnitChanged,
                this, &ModDockUi::emitModUnitChanged);

        connect(items_[i], &ModItemUi::itemHovered,
                this, &ModDockUi::emitItemHovered);

        // Halve the top for the first row
        double top = (i < wrap_) ?  1 : 0.5;

        // Left is 1 at wraps else 0.5 (see below)
        double left = 1;

        // Halve the bottom for the last row
        double bottom = (i < (items_.size() - wrap_)) ? 0.5 : 1;

        // Halve the right side if the next columnwill wrap
        double right = ((i + 1) % wrap_ > 0) ? 0.5 : 1;

        if (i % wrap_ == 0) ++row;

        else left = 0.5; // First column of a row is halved

        items_[i]->setBorderRatios(left, right, top, bottom);

        layout->addWidget(items_[i], row, i % wrap_);
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

void ModDockUi::emitModUnitChanged(const ModUnitUi& mod) const
{
    ModItemUi* senderItem = dynamic_cast<ModItemUi*>(QWidget::sender());

    for (index_t i = 0; i < items_.size(); ++i)
    {
        if (senderItem == items_[i])
        {
            emit modUnitChanged(i, mod);
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

