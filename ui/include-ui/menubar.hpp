#ifndef MENUBAR_H
#define MENUBAR_H

#include <QWidget>

class Projectbar;
class QLabel;

class Menubar : public QWidget
{
    Q_OBJECT

public:

    explicit Menubar(QWidget *parent = 0);

    void setVolume(double volume);

    double getVolume() const;

private:

    void setupUi();

    Projectbar* projectBar_;

    QLabel* volumeLabel_;

    double volume_;
};

#endif // MENUBAR_H
