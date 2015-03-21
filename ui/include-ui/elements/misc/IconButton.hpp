#ifndef ICONBUTTON_HPP
#define ICONBUTTON_HPP

#include <QPushButton>
#include <QSharedPointer>

class QIcon;
class QSize;

class IconButton : public QPushButton
{

    Q_OBJECT

    Q_PROPERTY(int iconWidth READ getIconWidth WRITE setIconWidth)

    Q_PROPERTY(int iconHeight READ getIconHeight WRITE setIconHeight)

    Q_PROPERTY(double iconFactor READ getIconFactor WRITE setIconFactor)

public:

    IconButton(const QIcon& icon,
               QWidget* parent = nullptr);

    IconButton(const QIcon& standard,
               const QIcon& active,
               QWidget* parent = nullptr);


    void setStandardIcon(const QIcon& icon);

    QIcon getStandardIcon() const;


    void setActiveIcon(const QIcon& icon);

    QIcon getActiveIcon() const;


    void setIconFactor(double factor);

    double getIconFactor() const;


    void setIconWidth(int width);

    int getIconWidth() const;


    void setIconHeight(int height);

    int getIconHeight() const;


    virtual void setIconSize(const QSize& size);

 private:

    QSharedPointer<QIcon> standard_;

    QSharedPointer<QIcon> active_;

    QSharedPointer<QSize> size_;

    double factor_;
};

#endif // ICONBUTTON_HPP

