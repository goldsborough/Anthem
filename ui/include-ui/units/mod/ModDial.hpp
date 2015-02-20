#ifndef MODDIAL_HPP
#define MODDIAL_HPP

#include "CustomDial.hpp"

#include <QString>
#include <QVector>

class ModDial : public CustomDial
{

public:

    typedef unsigned short index_t;

    ModDial(QWidget* parent = nullptr);

    ModDial(const QString& text,
            double factor,
            QWidget* parent = nullptr);

private:

};

#endif // MODDIAL_HPP
