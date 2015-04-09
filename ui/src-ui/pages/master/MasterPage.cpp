#include "MasterPage.hpp"
#include "ModControl.hpp"
#include "RecordUi.hpp"
#include "PanUi.hpp"

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


	ModControl* volume = new ModControl("Volume", 2, 2, this);

	connect(volume, &ModControl::controlChanged,
			[=] (double value) { emit volumeChanged(value); });

	layout->addWidget(volume, 0, 0);


	PanUi* pan = new PanUi(this);

	layout->addWidget(pan, 0, 1);


	RecordUi* record = new RecordUi(this);

	layout->addWidget(record, 1, 0);
}

void MasterPage::paintEvent(QPaintEvent* )
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
