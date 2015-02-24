#ifndef MODDIAL_HPP
#define MODDIAL_HPP

#include "CustomDial.hpp"

#include <QVector>
#include <QSharedPointer>

struct ModUnitUi;
class QRectF;

class ModDial : public CustomDial
{

public:

	typedef int index_t;

    ModDial(QWidget* parent = nullptr);

	ModDial(const QString& text,
			QWidget * parent = nullptr,
			double factor = 0.001,
			int minimum = 0,
			int maximum = 999);


	void addModArc();

	void addModArc(const ModUnitUi& mod);


	void insertModArc(index_t index);

	void insertModArc(index_t index, const ModUnitUi& mod);


	void removeLastModArc();

	void removeModArc(index_t index);


	void setModUnitUiForModArc(index_t index, const ModUnitUi& mod);

	ModUnitUi getModUnitUiFromModArc(index_t index);


	index_t getModArcIndexFromModUnitUiText(const QString& text) const;


	void setModArcValue(index_t index, double value);

	double getScaledModArcValue(index_t index) const;

	double getDisplayedModArcValue(index_t index) const;


	void showModArc(index_t index);

	void showControl();

private:

	struct ModArc
    {
		ModArc();

		ModArc(const ModUnitUi& modUnit);

		QSharedPointer<ModUnitUi> mod;

        QSharedPointer<QRectF> arcRect;

		int displayedValue;

		double scaledValue;
    };


	virtual void paintEvent(QPaintEvent* event) override;

    virtual void mouseMoveEvent(QMouseEvent* event) override;

	virtual void updateValue() override;


	ModArc* displayedModArc_;

	QVector<ModArc> mods_;
};

#endif // MODDIAL_HPP
