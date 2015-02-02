#include "CustomComboBox.hpp"

#include <QMenu>

#include <QDebug>

CustomComboBox::CustomComboBox(QWidget* parent)
: QPushButton(parent)
{
    QWidget::setCursor(Qt::PointingHandCursor);

    menu_ = new QMenu(this);

    connect(this, &QPushButton::clicked,
            [=] ()
            {
                menu_->popup(mapToGlobal(QPoint(0, height())));
            });
}

void CustomComboBox::addItem(const QString& item)
{
    QAction* action = new QAction(item, this);

    connect(action, &QAction::triggered,
            this, &CustomComboBox::setCurrentItem_);

    bool empty = menu_->isEmpty();

    menu_->addAction(action);

    // To have the first item as default
    if (empty) action->trigger();

    menu_->setStyleSheet("");
}

void CustomComboBox::addItems(const QStringList &list)
{
    for(QStringList::const_iterator itr = list.begin(), end = list.end();
        itr != end;
        ++itr)
    {
        addItem(*itr);
    }
}

void CustomComboBox::setCurrentItem_()
{
    QPushButton::setText(dynamic_cast<QAction*>(QWidget::sender())->text());
}

QString CustomComboBox::getCurrentItem() const
{
    return QPushButton::text();
}
