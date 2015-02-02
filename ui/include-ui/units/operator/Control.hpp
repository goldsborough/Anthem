#ifndef CONTROL_HPP
#define CONTROL_HPP

#include <QWidget>

namespace OperatorUi
{
    class Control : public QWidget
    {
        Q_OBJECT

    public:

        Control(QWidget* parent = nullptr);

    private:

        void setupUi();
    };
}

#endif // CONTROL_HPP
