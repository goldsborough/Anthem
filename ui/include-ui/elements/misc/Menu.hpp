#ifndef CUSTOMMENU_HPP
#define CUSTOMMENU_HPP

#include <QMenu>

class Menu : public QMenu
{
    Q_OBJECT

public:

	enum class Position { TOP, BOTTOM, RIGHT, LEFT };

	Menu(QWidget* parent = nullptr,
		 Position pos = Position::BOTTOM);

	~Menu();

	void setPosition(Position pos);

	Position getPosition() const;

public slots:

	void popup();

private:

	Position pos_;

};

#endif // CUSTOMMENU_HPP
