#ifndef OPERATORPAGE_HPP
#define OPERATORPAGE_HPP

#include <QWidget>
#include <QTabWidget>

namespace OperatorPage
{
    class ContainerPage : public QTabWidget
    {
        Q_OBJECT

    public:

        ContainerPage(QWidget* parent = nullptr);

    private:

        void paintEvent(QPaintEvent* paintEvent);

        void setupUi();

    };
}

#endif // OPERATORPAGE_HPP
