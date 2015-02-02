#ifndef ICONBUTTON_HPP
#define ICONBUTTON_HPP

#include <QPushButton>
#include <QString>

class QIcon;
class QSize;

class IconButton : public QPushButton
{

    Q_OBJECT

public:

    IconButton(const QString& path,
               QSize* size,
               QWidget* parent = nullptr);

    IconButton(const QString& standardPath,
               const QString& activePath,
               QSize* standardSize,
               QSize* activeSize,
               QWidget* parent = nullptr);

    QIcon* getStandardIcon() const;

    QIcon* getActiveIcon() const;

    QSize* getStandardSize() const;

    QSize* getActiveSize() const;

    ~IconButton();

 private slots:

    void setActiveIcon();

    void setStandardIcon();

 private:

    QIcon* standard_;

    QIcon* active_;

    QSize* standardSize_;

    QSize* activeSize_;

};

#endif // ICONBUTTON_HPP

