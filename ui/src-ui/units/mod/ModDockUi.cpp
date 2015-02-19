#include "ModDockUi.hpp"
#include "ModDockItem.hpp"

#include <QGridLayout>

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
        items_[i] = new ModDockItem(QString('A' + i), this);

        connect(items_[i], &ModDockItem::scaledValueChanged,
                this, &ModDockUi::emitItemValueChanged);

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

void ModDockUi::emitItemValueChanged(double value)
{
    ModDockItem* senderItem = dynamic_cast<ModDockItem*>(QWidget::sender());

    for (index_t i = 0; i < items_.size(); ++i)
    {
        if (senderItem == items_[i])
        {
            emit itemValueChanged(i, value);
        }
    }
}

void ModDockUi::paintEvent(QPaintEvent* pe)
{
    QWidget::paintEvent(pe);
}

void ModDockUi::setWrap(index_t wrap)
{
    wrap_ = wrap;
}

unsigned short ModDockUi::getWrap() const
{
    return wrap_;
}
