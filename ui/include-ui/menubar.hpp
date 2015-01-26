#ifndef MENUBAR_H
#define MENUBAR_H

#include <QWidget>

class Projectbar;

class Menubar : public QWidget
{
    Q_OBJECT

public:

    explicit Menubar(QWidget *parent = 0);

private:

    void setupUi();

    Projectbar* projectBar_;
};

#endif // MENUBAR_H
