#ifndef MODDOCKITEM_HPP
#define MODDOCKITEM_HPP

#include <QAbstractSlider>
#include <QSharedPointer>
#include <QVector>
#include <QString>

class QColor;
class QLineF;
class QPen;

class ModItemUi : public QAbstractSlider
{
    Q_OBJECT

    Q_PROPERTY(QColor borderColor READ getBorderColor WRITE setBorderColor)

    Q_PROPERTY(double borderWidth READ getBorderWidth WRITE setBorderWidth)

public:

    enum Side { LEFT, RIGHT, TOP, BOTTOM };

    ModItemUi(const QString& text = QString(),
               QWidget* parent = nullptr);


    void setText(const QString& text);

    QString getText() const;


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

    void scaledValueChanged(double value) const;

private:

    virtual void resizeEvent(QResizeEvent* event) override;

    virtual void paintEvent(QPaintEvent*) override;


    QString text_;

    QSharedPointer<QPen> borderPen_;

    QSharedPointer<QLineF> borders_;

    QVector<double> ratios_;

    double borderWidth_;
};

#endif /* MODDOCKITEM_HPP */


