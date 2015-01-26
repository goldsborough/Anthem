#include "CustomMenu.hpp"
#include "IconButton.hpp"

#include <QPoint>
#include <QDebug>

CustomMenu::CustomMenu(IconButton *icon,
                       QWidget* parent,
                       const Position& pos)
: QMenu(parent), icon_(icon),
  pos_(pos), point_(new QPoint)
{

    QWidget::setCursor(Qt::PointingHandCursor);

    connect(icon, &IconButton::clicked,
            this, &CustomMenu::popup_);

   updatePoint();
}

CustomMenu::~CustomMenu()
{ }

void CustomMenu::updatePoint()
{
    *point_ = icon_->pos();

    switch(pos_)
    {
        case Position::TOP:
        {
             point_->rx() -= (QMenu::width() + icon_->width()) / 2;

             point_->ry() -= QMenu::height() + 5;

             break;
        }

        case Position::BOTTOM:
        {
            point_->rx() -= (QMenu::width() + icon_->width()) / 2;

            point_->ry() += icon_->height() + 5;

            break;
        }

        case Position::LEFT:
            point_->rx() -= icon_->width() + QMenu::width() + 50;
            break;

        case Position::RIGHT:
            point_->rx() += icon_->width() + 10;
            break;
    }
}

void CustomMenu::setPosition(const Position& pos)
{
    pos_ = pos;
}

CustomMenu::Position CustomMenu::getPosition() const
{
    return pos_;
}

void CustomMenu::setIconButton(IconButton* icon)
{
    icon_ = icon;
}

IconButton* CustomMenu::getIconButton() const
{
    return icon_;
}


void CustomMenu::popup_()
{
    QMenu::popup(QMenu::parentWidget()->mapToGlobal(*point_));
}
