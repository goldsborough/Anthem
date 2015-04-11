#include "MacroUi.hpp"
#include "ModControl.hpp"
#include "ModUnitUi.hpp"

#include <QString>
#include <QPushButton>
#include <QPainter>
#include <QStyleOption>
#include <QSizePolicy>
#include <QGridLayout>

class ModUnit { };

class Macro : public ModUnit
{
public:
};

MacroUi::MacroUi(QWidget *parent)
: QWidget(parent)
{ }

MacroUi::MacroUi(const QString &id,
				 QWidget *parent)
: QWidget(parent),
  macro_(nullptr),
  mod_(new ModUnitUi(&(*macro_), id,
					 ModUnitUi::Range::LINEAR))
{
	setupUi();
}

void MacroUi::setupUi()
{
	QWidget::setSizePolicy(QSizePolicy::Maximum,
						   QSizePolicy::Minimum);

	QGridLayout* layout = new QGridLayout(this);

	layout->setMargin(0);

	layout->setSpacing(0);

	layout->setContentsMargins(0, 0, 0, 0);


	QPushButton* activity = new QPushButton(QString(mod_->id[6]), this);

	activity->setCheckable(true);

	activity->setChecked(true);

	activity->setCursor(Qt::PointingHandCursor);

	layout->addWidget(activity, 0, 0);


	QPushButton* copy = new QPushButton("Copy", this);

	copy->setCursor(Qt::PointingHandCursor);

	layout->addWidget(copy, 0, 1);


	ModControl* control = new ModControl(mod_->id, 2, 2, this);

	layout->addWidget(control, 1, 0, 1, 2);
}

void MacroUi::paintEvent(QPaintEvent* )
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void MacroUi::setId(const QString &id)
{
	mod_->id = id;
}

QString MacroUi::getId() const
{
	return mod_->id;
}
