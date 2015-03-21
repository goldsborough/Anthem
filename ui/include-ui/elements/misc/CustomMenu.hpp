#ifndef CUSTOMMENU_HPP
#define CUSTOMMENU_HPP

#include <QMenu>

class QWidget;

class CustomMenu : public QMenu
{
    Q_OBJECT

public:

    enum class Position
    {
        TOP,
        BOTTOM,
        RIGHT,
        LEFT
    };

	CustomMenu(QWidget* parent,
			   const Position& pos = Position::BOTTOM);

    ~CustomMenu();

    void setPosition(const Position& pos);

	Position getPosition() const;

public slots:

	void popup();

private:

	Position pos_;

};

#endif // CUSTOMMENU_HPP
