#ifndef CONTAINER_H
#define CONTAINER_H

#include <QTabWidget>

class Container : public QTabWidget
{
    Q_OBJECT

public:

    Container(QWidget* parent);

private:

    void setupUi();

};

#endif // CONTAINER_H
