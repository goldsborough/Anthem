#include "ModControl.hpp"
#include "ModDockUi.hpp"
#include "ModDial.hpp"
#include "ModUnitUi.hpp"

#include <QGridLayout>
#include <QStyleOption>
#include <QPainter>
#include <QVBoxLayout>
#include <QMouseEvent>

#include <QDebug>

ModControl::ModControl(const QString& title, QWidget* parent)
: QWidget(parent), title_(title.toUpper())
{
    setupUi();

    QWidget::setMouseTracking(true);

    QWidget::setFixedSize(100, 200);
}

ModControl::~ModControl()
{ }

void ModControl::setupUi()
{
    QVBoxLayout* layout = new QVBoxLayout(this);

    layout->setSpacing(0);

    layout->setMargin(0);

	ModDial* dial_ = new ModDial(title_, this);

	//dial_->addModArc({nullptr, "LFO-A", ModUnitUi::Range::PERIODIC});

	//dial_->setModArcValue(0, 0.25);

	dial_->addModArc({nullptr, "LFO-B", ModUnitUi::Range::LINEAR});

	dial_->setModArcValue(0, -0.5);

    layout->addWidget(dial_);

/*
	dock_ = new ModDockUi(4, 2, this);

    connect(dock_, &ModDockUi::depthChanged,
            [&] (ModDockUi::index_t index, double value)
            { emit depthChanged(index, value); });

	connect(dock_, &ModDockUi::modUnitInserted,
            [&] (ModDockUi::index_t index, const ModUnitUi& mod)
			{ emit modUnitInserted(index, mod); });

    connect(dock_, &ModDockUi::itemHovered,
            [&] (ModDockUi::index_t index)
            { qDebug() << index << endl; });

    layout->addWidget(dock_);
	*/

    QWidget::setLayout(layout);
}

void ModControl::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
