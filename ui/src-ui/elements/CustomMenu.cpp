#include "CustomMenu.hpp"

#include <QPushButton>
#include <QPoint>
#include <QDebug>

CustomMenu::CustomMenu(QPushButton *icon,
                       QWidget* parent,
                       const Position& pos)
: QMenu(parent), button_(icon),
  pos_(pos), point_(new QPoint)
{

    QWidget::setCursor(Qt::PointingHandCursor);

    connect(icon, &QPushButton::clicked,
            this, &CustomMenu::popup_);

   updatePoint();
}

CustomMenu::~CustomMenu()
{ }

void CustomMenu::updatePoint()
{
    *point_ = button_->pos();

    switch(pos_)
    {
        case Position::TOP:
        {
             //point_->rx() -= (QMenu::width() + button_->width()) / 2;

             point_->ry() -= QMenu::height() + 5;

             break;
        }

        case Position::BOTTOM:
        {
            //point_->rx() -= (QMenu::width() + button_->width()) / 2;

            point_->ry() += button_->height() + 5;

            break;
        }

        case Position::LEFT:
            point_->rx() -= button_->width() + QMenu::width() + 50;
            break;

        case Position::RIGHT:
            point_->rx() += button_->width() + 10;
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

void CustomMenu::setButton(QPushButton* icon)
{
    button_ = icon;
}

QPushButton* CustomMenu::getButton() const
{
    return button_;
}


void CustomMenu::popup_()
{
    QMenu::popup(QMenu::parentWidget()->mapToGlobal(*point_));
}
