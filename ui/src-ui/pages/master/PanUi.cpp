#include "PanUi.hpp"
#include "ModControl.hpp"
#include "ComboBox.hpp"

#include <QLabel>
#include <QPushButton>
#include <QPainter>
#include <QStyleOption>
#include <QVBoxLayout>
#include <QButtonGroup>


#include <QDebug>


PanUi::PanUi(QWidget* parent)
: QWidget(parent)
{
	setupUi();
}

void PanUi::setupUi()
{
	auto layout = new QVBoxLayout(this);

	layout->setSpacing(0);

	layout->setMargin(0);


	auto control = new ModControl("PAN", 2, 2, this);


	auto box = new ComboBox(this);

	box->setSizePolicy(QSizePolicy::Expanding,
					   QSizePolicy::Expanding);

	box->addItems({"Linear", "Sine", "Radical"});


	layout->addWidget(box);

	layout->addWidget(control);
}

void PanUi::paintEvent(QPaintEvent*)
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
