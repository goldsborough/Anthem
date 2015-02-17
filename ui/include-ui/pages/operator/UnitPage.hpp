#ifndef UNITPAGE_HPP
#define UNITPAGE_HPP

#include <QWidget>
#include <QVector>

class OperatorUi;

namespace OperatorPage
{
    class UnitPage : public QWidget
    {
        Q_OBJECT

    public:

        enum { A, B, C, D };

        UnitPage(QWidget* parent = nullptr);

    private:

        typedef QList<OperatorUi*>::size_type size_t;

        void setupUi();

        QVector<OperatorUi*> operators_;
    };
}

#endif // UNITPAGE_HPP
