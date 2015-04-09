#include "PanUi.hpp"
#include "ModControl.hpp"

#include <QLabel>
#include <QPushButton>
#include <QPainter>
#include <QStyleOption>
#include <QVBoxLayout>

PanUi::PanUi(QWidget* parent)
: QWidget(parent)
{
	setupUi();
}

void PanUi::setupUi()
{
	QVBoxLayout* layout = new QVBoxLayout(this);

	layout->setSpacing(0);

	layout->setMargin(0);

	layout->setContentsMargins(0, 0, 0, 0);


	QPushButton* type = new QPushButton("Radical", this);

	type->setSizePolicy(QSizePolicy::Minimum,
						QSizePolicy::Minimum);

	layout->addWidget(type);


	ModControl* control = new ModControl("Pan", 2, 2, this);

	control->setSizePolicy(QSizePolicy::Expanding,
						   QSizePolicy::Expanding);


	layout->addWidget(control);

	QWidget::setSizePolicy(QSizePolicy::Maximum,
						   QSizePolicy::Maximum);
}

void PanUi::paintEvent(QPaintEvent*)
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
