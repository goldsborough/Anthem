#ifndef CUSTOMMENU_HPP
#define CUSTOMMENU_HPP

#include <QMenu>

#include <memory>

class IconButton;
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

    CustomMenu(IconButton* icon,
               QWidget* parent,
               const Position& pos = Position::BOTTOM);

    ~CustomMenu();

    void setIconButton(IconButton* icon);

    IconButton* getIconButton() const;

    void setPosition(const Position& pos);

    Position getPosition() const;

    void updatePoint();

private slots:

    void popup_();

private:

    IconButton* icon_;

    Position pos_;

    std::unique_ptr<QPoint> point_;

};

#endif // CUSTOMMENU_HPP
