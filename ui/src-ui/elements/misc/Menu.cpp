#include "Menu.hpp"

#include <QPushButton>
#include <QPoint>
#include <QDebug>

Menu::Menu(QWidget* parent,
		   Position pos)
: QMenu(parent),
  pos_(pos)
{
	QMenu::setCursor(Qt::PointingHandCursor);
}

Menu::~Menu() = default;

void Menu::setPosition(Position pos)
{
    pos_ = pos;
}

Menu::Position Menu::getPosition() const
{
    return pos_;
}

void Menu::popup()
{
	QPoint pos = QMenu::parentWidget()->pos();

	switch(pos_)
	{
		case Position::TOP:
		{
			pos.ry() -= QMenu::actions().size() *
						QMenu::actionGeometry(QMenu::actions()[0]).height();

			pos.ry() -= QMenu::parentWidget()->height();

			 break;
		}

		case Position::BOTTOM:
		{
			pos.ry() += QMenu::parentWidget()->height();

			break;
		}

		case Position::LEFT:
		{
			pos.rx() -= QMenu::actionGeometry(QMenu::actions()[0]).width();

			pos.rx() -= QMenu::parentWidget()->width();

			break;
		}

		case Position::RIGHT:
		{
			pos.rx() += QMenu::parentWidget()->width();

			break;
		}
	}

	QMenu::popup(QMenu::parentWidget()->parentWidget()->mapToGlobal(pos));
}
