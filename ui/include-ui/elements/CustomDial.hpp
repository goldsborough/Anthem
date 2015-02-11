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

    CustomDial(const QString& text, QWidget * parent = nullptr);

    ~CustomDial();

    void setArcColor(const QString& color);

    QString getArcColor() const;

    void setStartAngle(double angle);

    double getStartAngle() const;

    void setMaximumAngle(double angle);

    double getMaximumAngle() const;

    void setArcWidth(double px);

    double getArcWidth() const;

    void setText(const QString& text);

    QString getText() const;

    void setFixedSize(int w, int h);

private slots:

    void updateValue();

private:

    void paintEvent(QPaintEvent*);

    double maximumAngle_;

    double startAngle_;

    double arcWidth_;

    double angleSpan_;

    QString valueString_;

    QString text_;

    QSharedPointer<QRectF> arcRect_;

    QSharedPointer<QRectF> valueRect_;

    QSharedPointer<QRectF> textRect_;

    QSharedPointer<QColor> arcColor_;

    QSharedPointer<QPen> arcPen_;
};

#endif // CUSTOMDIAL_HPP
