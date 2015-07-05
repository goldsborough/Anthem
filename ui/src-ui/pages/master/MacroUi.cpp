#include "MacroUi.hpp"
#include "ModControl.hpp"
#include "ModUnitUi.hpp"

#include <QString>
#include <QPushButton>
#include <QPainter>
#include <QStyleOption>
#include <QSizePolicy>
#include <QVBoxLayout>

class ModUnit { };

class Macro : public ModUnit
{ public: };

MacroUi::MacroUi(QWidget *parent)
: QWidget(parent)
{ }

MacroUi::MacroUi(const QString &id,
				 QWidget *parent)
: QWidget(parent),
  macro_(nullptr),
  mod_(new ModUnitUi(&(*macro_),
					 id,
					 ModUnitUi::Range::LINEAR))
{
	setupUi();
}

void MacroUi::setupUi()
{
	QWidget::setSizePolicy(QSizePolicy::Maximum,
						   QSizePolicy::Minimum);

	auto layout = new QGridLayout(this);

	layout->setMargin(0);

	layout->setSpacing(0);


	auto activity = new QPushButton(QString(mod_->id[6]), this);

	activity->setCheckable(true);

	activity->setCursor(Qt::PointingHandCursor);

	activity->setSizePolicy(QSizePolicy::Expanding,
							QSizePolicy::Expanding);

	layout->addWidget(activity);


	auto control = new ModControl(mod_->id, 2, 2, this);

	layout->addWidget(control);
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
