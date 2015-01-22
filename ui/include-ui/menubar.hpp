#ifndef MENUBAR_H
#define MENUBAR_H

#include <QWidget>

class Filebar;

class Menubar : public QWidget
{
    Q_OBJECT

public:

    explicit Menubar(QWidget *parent = 0);

private:

    void initUI_();

    Filebar* filebar_;
};

#endif // MENUBAR_H
