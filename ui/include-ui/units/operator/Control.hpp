#ifndef CONTROL_HPP
#define CONTROL_HPP

#include <QWidget>
#include <QString>

namespace OperatorUi
{
    class Control : public QWidget
    {
        Q_OBJECT

    public:

        Control(const QString& title, QWidget* parent = nullptr);

    private:

        void setupUi();

        void paintEvent(QPaintEvent *);

        QString title_;
    };
}

#endif // CONTROL_HPP
