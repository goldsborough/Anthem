#ifndef CUSTOMMENU_HPP
#define CUSTOMMENU_HPP

#include <QMenu>

#include <memory>

class QPushButton;
class QPoint;
class QAction;

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

    void updatePoint();

private slots:

    void popup_();

private:

    QPushButton* button_;

    Position pos_;

    std::unique_ptr<QPoint> point_;

};

#endif // CUSTOMMENU_HPP
