#include "ModDockUi.hpp"
#include "ModItemUi.hpp"

#include <QGridLayout>
#include <algorithm>

ModDockUi::ModDockUi(index_t dockSize,                   
                     index_t wrap,
                     QWidget* parent)
: QWidget(parent),
  wrap_(wrap),
  units_(dockSize)
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

    for (short i = 0, row = -1; i < units_.size(); ++i)
    {
        units_[i] = new ModItemUi(QString('A' + i), this);

        connect(units_[i], &ModItemUi::scaledValueChanged,
                this, &ModDockUi::emitItemValueChanged);

        // Halve the top for the first row
        double top = (i < wrap_) ?  1 : 0.5;

        // Left is 1 at wraps else 0.5 (see below)
        double left = 1;

        // Halve the bottom for the last row
        double bottom = (i < (units_.size() - wrap_)) ? 0.5 : 1;

        // Halve the right side if the next columnwill wrap
        double right = ((i + 1) % wrap_ > 0) ? 0.5 : 1;

        if (i % wrap_ == 0) ++row;

        else left = 0.5; // First column of a row is halved

        units_[i]->setBorderRatios(left, right, top, bottom);

        layout->addWidget(units_[i], row, i % wrap_);
    }
}

void ModDockUi::emitItemValueChanged(double value)
{
    ModItemUi* senderItem = dynamic_cast<ModItemUi*>(QWidget::sender());

    for (index_t i = 0; i < units_.size(); ++i)
    {
        if (senderItem == units_[i])
        {
            emit itemValueChanged(i, value);
        }
    }
}

void ModDockUi::addModUnit(ModItemUi* unit)
{
    units_.push_back(unit);
}

ModItemUi* ModDockUi::getModUnit(index_t index) const
{
    return units_[index];
}

ModItemUi* ModDockUi::getModUnit(const QString& text) const
{
    QVector<ModItemUi*>::const_iterator itr;

    itr = std::find(units_.begin(),  units_.end(),
                    [&] (ModItemUi* unit) {unit->getText() == text});

    return (itr == units_.end()) ? 0 : *itr;
}


void ModDockUi::removeModUnit(index_t index)
{
    units_.remove(index);
}

void ModDockUi::removeModUnit(const QString& text)
{
    QVector<ModItemUi*>::iterator itr;

    itr = std::find(units_.begin(),  units_.end(),
                    [&] (ModItemUi* unit) {unit->getText() == text});

    units_.erase(itr);
}


ModDockUi::index_t ModDockUi::size() const
{
    return units_.size();
}

void ModDockUi::clear()
{
    units_.clear();
}


void ModDockUi::setWrap(index_t wrap)
{
    wrap_ = wrap;
}

ModDockUi::index_t ModDockUi::getWrap() const
{
    return wrap_;
}
