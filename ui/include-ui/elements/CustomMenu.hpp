#ifndef CUSTOMMENU_HPP
#define CUSTOMMENU_HPP

#include <QMenu>

class QPushButton;

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

    CustomMenu(QPushButton* button,
               QWidget* parent,
			   const Position& pos = Position::BOTTOM);

    ~CustomMenu();

    void setButton(QPushButton* icon);

    QPushButton* getButton() const;

    void setPosition(const Position& pos);

	Position getPosition() const;

private slots:

    void popup_();

private:

    QPushButton* button_;

	Position pos_;

};

#endif // CUSTOMMENU_HPP
