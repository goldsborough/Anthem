#ifndef OPERATORUI_HPP
#define OPERATORUI_HPP

#include <QWidget>

namespace OperatorUi
{

    class Container : public QWidget
    {
        Q_OBJECT

    public:

        Container(QWidget* parent = nullptr);

    private:

        void setupUi();

        void paintEvent(QPaintEvent* paintEvent);

    };

}

#endif // OPERATORUI_HPP
