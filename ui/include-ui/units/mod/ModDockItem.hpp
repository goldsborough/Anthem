#ifndef MODDOCKITEM_HPP
#define MODDOCKITEM_HPP

#include <QAbstractSlider>
#include <QString>
#include <QSharedPointer>

class QColor;
class QLineF;
class QPen;

class ModDockItem : public QAbstractSlider
{
    Q_OBJECT

    Q_PROPERTY(QColor borderColor READ getBorderColor WRITE setBorderColor)

    Q_PROPERTY(double borderWidth READ getBorderWidth WRITE setBorderWidth)

public:

    enum Side { LEFT, RIGHT, TOP, BOTTOM };

    ModDockItem(const QString& text = QString(),
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

    void setFixedSize(int w, int h);

    void setFixedSize(const QSize& size);

signals:

    void scaledValueChanged(double value) const;

private:

    void paintEvent(QPaintEvent* pe);

    QString text_;

    QSharedPointer<QPen> borderPen_;

    QSharedPointer<QLineF> borders_;

    double ratios_ [4];

    double borderWidth_;
};

#endif /* MODDOCKITEM_HPP */


