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

	Q_PROPERTY(bool tooltipEnabled READ tooltipIsEnabled WRITE setTooltipEnabled)

	Q_PROPERTY(int step READ getStep WRITE setStep)

public:

    enum Side { LEFT, RIGHT, TOP, BOTTOM };

	ModItemUi(QWidget* parent = nullptr,
			  bool tooltipEnabled = true,
			  double factor = 0.001,
			  int minimum = -999,
			  int maximum = 999,
			  int step = 20);


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


	void setStep(int step);

	int getStep() const;


	void setTooltipEnabled(bool enabled);

	bool tooltipIsEnabled() const;

signals:

	void sidechainEvent(QObject* slave, bool enable) const;

	void clearSlavesEvent() const;

    void depthChanged(double value) const;

	void modUnitInserted(const ModUnitUi& mod) const;

	void modUnitRemoved() const;

    void itemHovered() const;

private:

	virtual void mouseDoubleClickEvent(QMouseEvent* event) override;

    virtual void mouseMoveEvent(QMouseEvent* event) override;

	virtual void mousePressEvent(QMouseEvent* event) override;

    virtual void resizeEvent(QResizeEvent* event) override;

    virtual void paintEvent(QPaintEvent*) override;

	virtual void dragEnterEvent(QDragEnterEvent* event) override;

	virtual void dragMoveEvent(QDragMoveEvent* event) override;

	virtual void dropEvent(QDropEvent* event) override;


	void showContextMenu() const;


	QVector<QSharedPointer<QLineF>> borders_;

	QSharedPointer<QPoint> lastPosition_;

    QSharedPointer<ModUnitUi> mod_;

    QSharedPointer<QPen> borderPen_;

	QVector<QObject*> slaves_;

    QVector<double> ratios_;

	bool tooltipEnabled_;

    double borderWidth_;

	double factor_;

	int step_;
};

#endif /* MODDOCKITEM_HPP */


