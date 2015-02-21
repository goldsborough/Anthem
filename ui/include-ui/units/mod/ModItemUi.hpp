#ifndef MODDOCKITEM_HPP
#define MODDOCKITEM_HPP

#include <QAbstractSlider>
#include <QSharedPointer>
#include <QVector>

class QColor;
class QLineF;
class QPen;
struct ModUnitUi;

class ModItemUi : public QAbstractSlider
{
    Q_OBJECT

    Q_PROPERTY(QColor borderColor READ getBorderColor WRITE setBorderColor)

    Q_PROPERTY(double borderWidth READ getBorderWidth WRITE setBorderWidth)

public:

    enum Side { LEFT, RIGHT, TOP, BOTTOM };

    ModItemUi(QWidget* parent = nullptr);

    ModItemUi(const ModUnitUi& mod,
              QWidget* parent = nullptr);


    void setModUnitUi(const ModUnitUi& mod);

    ModUnitUi getModUnitUi() const;


    void setBorderColor(const QColor& color);

    QColor getBorderColor() const;


    void setBorderWidth(double width);

    double getBorderWidth() const;


    void setBorderRatios(double left,
                         double right,
                         double top,
                         double bottom);

    void setBorderRatio(Side side, double ratio);

    double getBorderRatio(Side side) const;

signals:

    void depthChanged(double value) const;

    void modUnitChanged(const ModUnitUi& mod) const;

    void itemHovered() const;

private:

    virtual void mouseMoveEvent(QMouseEvent* event) override;

    virtual void resizeEvent(QResizeEvent* event) override;

    virtual void paintEvent(QPaintEvent*) override;

    QSharedPointer<ModUnitUi> mod_;

    QSharedPointer<QPen> borderPen_;

    QSharedPointer<QLineF> borders_;

    QVector<double> ratios_;

    double borderWidth_;
};

#endif /* MODDOCKITEM_HPP */


