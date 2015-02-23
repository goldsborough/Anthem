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

    enum class Range { SINGLE, DOUBLE };

    ModDial(QWidget* parent = nullptr);

    ModDial(const QString& text,
            QWidget * parent = nullptr,
            double factor = 0.001,
            int minimum = 0,
            int maximum = 999);

    void addModArc(Range range, const QString& text);

    void removeModArc(index_t index);

    void removeModArc(const QString& text);


    index_t getIndexForModArcText(const QString& text) const;

    void setModArcText(const QString& text);

    QString getModArcText() const;


    void setModArcRange(Range range);

    Range getModArcRange() const;


    void setModArcValue(double value);

    double getScaledModArcValue() const;

    double getDisplayedModArcValue() const;


    void showModArcValue(index_t index);

    void showControlValue();

private:

    struct ModArc
    {
        Range range_;

        QSharedPointer<QRectF> arcRect;

        double displayedValue;

        double scaledValue;

        QString text;
    };

    virtual void paintEvent(QPaintEvent* event) override;

    virtual void mouseMoveEvent(QMouseEvent* event) override;

    bool controlValueShown_;

    QVector<ModArc> mods_;

};

#endif // MODDIAL_HPP
