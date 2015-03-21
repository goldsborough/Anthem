#include "PanUi.hpp"
#include "ModControl.hpp"

#include <QLabel>
#include <QComboBox>
#include <QPainter>
#include <QStyleOption>
#include <QGridLayout>

PanUi::PanUi(QWidget* parent)
: QWidget(parent)
{
	setupUi();
}

void PanUi::setupUi()
{
	QGridLayout* layout = new QGridLayout(this);


	ModControl* control = new ModControl("Pan", 2, 2, this);

	layout->addWidget(control, 0, 0, 2, 1);


	QLabel* label = new QLabel("Type", this);

	layout->addWidget(label, 0, 1);


	QComboBox* box = new QComboBox(this);

	layout->addWidget(box, 1, 1);

}

void PanUi::paintEvent(QPaintEvent*)
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
