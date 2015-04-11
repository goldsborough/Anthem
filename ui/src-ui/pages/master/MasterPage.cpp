#include "MasterPage.hpp"
#include "ModControl.hpp"
#include "RecordUi.hpp"
#include "PanUi.hpp"
#include "MacroUi.hpp"

#include <QGridLayout>
#include <QStyleOption>
#include <QPainter>

MasterPage::MasterPage(QWidget* parent)
: QWidget(parent)
{
	setupUi();
}

void MasterPage::setupUi()
{
	QGridLayout* layout = new QGridLayout(this);


	ModControl* volume = new ModControl("VOLUME", 2, 2, this);

	volume->setObjectName("VolumeUi");

	connect(volume, &ModControl::controlChanged,
			[=] (double value) { emit volumeChanged(value); });

	layout->addWidget(volume, 0, 0);


	PanUi* pan = new PanUi(this);

	layout->addWidget(pan, 0, 1);


	RecordUi* record = new RecordUi(this);

	layout->addWidget(record, 0, 2, 1, 2);


	for (int i = 0; i < 4; ++i)
	{
		MacroUi* macro = new MacroUi("MACRO " + QString('A' + i), this);

		layout->addWidget(macro, 1, i);
	}
}

void MasterPage::paintEvent(QPaintEvent* )
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
