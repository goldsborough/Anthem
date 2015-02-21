#ifndef OPERATORUI_HPP
#define OPERATORUI_HPP

#include <QWidget>

class Operator;

class OperatorUi : public QWidget
{
    Q_OBJECT

public:

    OperatorUi(QWidget* parent = nullptr);

private:

    void setupUi();

    void paintEvent(QPaintEvent* paintEvent);

};

#endif // OPERATORUI_HPP
