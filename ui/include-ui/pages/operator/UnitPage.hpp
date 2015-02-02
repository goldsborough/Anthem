#ifndef UNITPAGE_HPP
#define UNITPAGE_HPP

#include <QWidget>
#include <QList>

namespace OperatorUi { class Container; }

namespace OperatorPage
{
    class UnitPage : public QWidget
    {
        Q_OBJECT

    public:

        enum { A, B, C, D };

        UnitPage(QWidget* parent = nullptr);

    private:

        typedef QList<OperatorUi::Container*>::size_type size_t;

        void setupUi();

        QList<OperatorUi::Container*> operators_;
    };
}

#endif // UNITPAGE_HPP
