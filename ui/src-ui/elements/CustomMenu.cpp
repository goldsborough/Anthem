#include "CustomMenu.hpp"

#include <QPushButton>
#include <QPoint>
#include <QDebug>

CustomMenu::CustomMenu(QPushButton *icon,
                       QWidget* parent,
                       const Position& pos)
: QMenu(parent),
  button_(icon),
  pos_(pos)
{
	QMenu::setCursor(Qt::PointingHandCursor);

    connect(icon, &QPushButton::clicked,
			this, &CustomMenu::popup_);
}

CustomMenu::~CustomMenu() = default;

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
	QPoint pos = button_->pos();

	switch(pos_)
	{
		case Position::TOP:
		{
			pos.ry() -= QMenu::actions().size() * QMenu::actionGeometry(QMenu::actions()[0]).height();

			pos.ry() -= button_->height();

			 break;
		}

		case Position::BOTTOM:
		{
			pos.ry() += button_->height();

			break;
		}

		case Position::LEFT:
		{
			pos.rx() -= QMenu::actionGeometry(QMenu::actions()[0]).width();

			pos.rx() -= button_->width();

			break;
		}

		case Position::RIGHT:
		{
			pos.rx() += button_->width();

			break;
		}
	}

	QMenu::popup(button_->parentWidget()->mapToGlobal(pos));
}
