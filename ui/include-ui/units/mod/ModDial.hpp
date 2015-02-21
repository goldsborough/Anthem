#ifndef MODDIAL_HPP
#define MODDIAL_HPP

#include "CustomDial.hpp"

#include <QString>
#include <QVector>
#include <QSharedPointer>

class QRectF;

class ModDial : public CustomDial
{

public:

    typedef unsigned short index_t;

    ModDial(QWidget* parent = nullptr);

    ModDial(const QString& text,
            double factor,
            QWidget* parent = nullptr);

    void showModValue(index_t index);

    void showControlValue();

private:

    struct Mod
    {
        enum Direction
        {
            LEFT,
            RIGHT,
            BOTH

        } direction;

        QSharedPointer<QRectF> arcRect;

        double displayedValue;

        double value;

        QString text;
    };

    virtual void mouseMoveEvent(QMouseEvent* event) override;

    bool controlValueShown_;

};

#endif // MODDIAL_HPP
