#ifndef MODDIAL_HPP
#define MODDIAL_HPP

#include "Dial.hpp"

#include <QVector>
#include <QSharedPointer>

struct ModUnitUi;
class QRectF;
class QPoint;

class ModDial : public Dial
{

	Q_OBJECT

	Q_PROPERTY(double arcPadding READ getArcPadding WRITE setArcPadding)

	Q_PROPERTY(QColor displayedArcColor READ getDisplayedArcColor WRITE setDisplayedArcColor)

public:

	typedef int index_t;

    ModDial(QWidget* parent = nullptr);

	ModDial(const QString& text,
			QWidget * parent = nullptr,
			index_t numberOfArcs = 0,
			double factor = 0.001,
			double modFactor = 0.001,
			int minimum = 0,
			int maximum = 999);


	void addModArc();

	void addModArc(const ModUnitUi& mod);


	void insertModArc(index_t index);

	void insertModArc(index_t index, const ModUnitUi& mod);


	void removeLastModArc();

	void removeModArc(index_t index);


	void setModUnitUiForModArc(index_t index, const ModUnitUi& mod);

	void removeModUnitUiForModArc(index_t index);

	ModUnitUi getModUnitUiFromModArc(index_t index);


	index_t getModArcIndexFromModUnitUiId(const QString& id) const;

	index_t activeModArcs() const;


	void setModArcValue(index_t index, double value);

	void resetModArcValues();

	double getModArcValue(index_t index) const;

	int getDisplayedModArcValue(index_t index) const;


	virtual void setArcWidth(double px) override;


	void setModArcFactor(double factor);

	double getModArcFactor() const;


	void setArcPadding(double padding);

	double getArcPadding() const;


	void setDisplayedArcColor(const QColor& color);

	QColor getDisplayedArcColor() const;


	void showModArc(index_t index);

	void showControl();

private:

	struct ModArc
    {
		ModArc();

		ModArc(const ModUnitUi& modUnit);

		QSharedPointer<ModUnitUi> mod;

        QSharedPointer<QRectF> arcRect;

		double angleSpan;

		int displayedValue;

		double value;
    };


	void setupUi();

	virtual void paintEvent(QPaintEvent* event) override;

	virtual void mouseMoveEvent(QMouseEvent* event) override;

	virtual void resizeEvent(QResizeEvent* event) override;

	void updateArcRects_();

	void updateContents_();


	index_t activeModArcs_;

	double arcPadding_;

	double modFactor_;

	ModArc* displayedModArc_;

	QVector<ModArc> mods_;

	QSharedPointer<QRectF> contentsRect_;

	QSharedPointer<QColor> displayedArcColor_;
};

#endif // MODDIAL_HPP
