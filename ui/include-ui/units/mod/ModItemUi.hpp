#ifndef MODDOCKITEM_HPP
#define MODDOCKITEM_HPP

#include <QAbstractSlider>
#include <QSharedPointer>
#include <QVector>

class QColor;
class QLineF;
class QPen;
class QMenu;

struct ModUnitUi;

class ModItemUi : public QAbstractSlider
{
    Q_OBJECT

    Q_PROPERTY(QColor borderColor READ getBorderColor WRITE setBorderColor)

    Q_PROPERTY(double borderWidth READ getBorderWidth WRITE setBorderWidth)

public:

    enum Side { LEFT, RIGHT, TOP, BOTTOM };

	ModItemUi(QWidget* parent = nullptr,
			  double factor = 0.001,
			  int minimum = 0,
			  int maximum = 999,
			  int dialSpeed = 20);


	void insertModUnitUi(const ModUnitUi& mod);

    ModUnitUi getModUnitUi() const;


	void removeModUnitUi();


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


	void setDialSpeed(int speed);

	int getDialSpeed() const;

signals:

    void depthChanged(double value) const;

	void modUnitInserted(const ModUnitUi& mod) const;

	void modUnitRemoved() const;

    void itemHovered() const;

private:

    virtual void mouseMoveEvent(QMouseEvent* event) override;

	virtual void mousePressEvent(QMouseEvent* event) override;

    virtual void resizeEvent(QResizeEvent* event) override;

    virtual void paintEvent(QPaintEvent*) override;

	void setupUi();

	void showContextMenu() const;


	QSharedPointer<QMenu> contextMenu_;

    QSharedPointer<ModUnitUi> mod_;

    QSharedPointer<QPen> borderPen_;

	QSharedPointer<QPoint> lastPosition_;

	QVector<QSharedPointer<QLineF>> borders_;

    QVector<double> ratios_;

    double borderWidth_;

	double factor_;

	int speed_;
};

#endif /* MODDOCKITEM_HPP */


