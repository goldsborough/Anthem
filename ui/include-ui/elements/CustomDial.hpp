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

	Q_PROPERTY(QColor arcColor READ getArcColor WRITE setArcColor)

	Q_PROPERTY(double arcWidth READ getArcWidth WRITE setArcWidth)

public:

    explicit CustomDial(QWidget* parent = nullptr);

    CustomDial(const QString& text,
			   QWidget * parent = nullptr,
			   double factor = 0.001,
			   int minimum = 0,
			   int maximum = 999);


    virtual ~CustomDial();


	virtual void setArcColor(const QColor& color);

	virtual QColor getArcColor() const;


    virtual void setStartAngle(double angle);

    virtual double getStartAngle() const;


    virtual void setMaximumAngle(double angle);

    virtual double getMaximumAngle() const;


    virtual void setArcWidth(double px);

    virtual double getArcWidth() const;


    virtual void setText(const QString& text);

    virtual QString getText() const;


    virtual void setFactor(double factor);

    virtual double getFactor() const;


	virtual double getScaledValue() const;

signals:

	void scaledValueChanged(double value);

protected slots:

    virtual void updateValue();

protected:

    virtual void paintEvent(QPaintEvent*) override;

	virtual void resizeEvent(QResizeEvent* event) override;

	double maximumAngleSpan_;

	double endAngle_;

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
