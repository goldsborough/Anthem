#ifndef CONTAINER_H
#define CONTAINER_H

#include <QWidget>

class Container : public QWidget
{
    Q_OBJECT

public:

    explicit Container(QWidget* parent);

private:

    void initUI_();

};

#endif // CONTAINER_H
