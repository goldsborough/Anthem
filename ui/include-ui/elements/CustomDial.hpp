#ifndef CUSTOMDIAL_HPP
#define CUSTOMDIAL_HPP

#include <QDial>
#include <QString>
#include <QSharedPointer>

class QColor;
class QRectF;
class QPen;

class CustomDial : public QDial
{
    Q_OBJECT

    Q_PROPERTY(QString arc READ getArcColor WRITE setArcColor)

public:

    explicit CustomDial(QWidget* parent = nullptr);

    CustomDial(const QString& text,
               double factor,
               QWidget * parent = nullptr);

    ~CustomDial();

    virtual void setArcColor(const QString& color);

    virtual QString getArcColor() const;


    virtual void setStartAngle(double angle);

    virtual double getStartAngle() const;


    virtual void setMaximumAngle(double angle);

    virtual double getMaximumAngle() const;


    virtual void setArcWidth(double px);

    virtual double getArcWidth() const;


    virtual void setText(const QString& text);

    virtual QString getText() const;


    virtual void resizeEvent(QResizeEvent* event) override;


    virtual void setFactor(double factor);

    virtual double getFactor() const;

signals:

    virtual void scaledValueChanged(double value);

protected slots:

    virtual void updateValue();

protected:

    virtual void paintEvent(QPaintEvent*) override;

    double maximumAngle_;

    double startAngle_;

    double arcWidth_;

    double angleSpan_;

    double factor_;

    QString valueString_;

    QString text_;

    QSharedPointer<QRectF> arcRect_;

    QSharedPointer<QRectF> valueRect_;

    QSharedPointer<QRectF> textRect_;

    QSharedPointer<QColor> arcColor_;

    QSharedPointer<QPen> arcPen_;
};

#endif // CUSTOMDIAL_HPP
