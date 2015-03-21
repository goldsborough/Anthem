#include "MasterPage.hpp"
#include "VolumeUi.hpp"
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

	layout->addWidget(new VolumeUi(this), 0, 0);

	layout->addWidget(new PanUi(this), 0, 1);

	layout->addWidget(new RecordUi(this), 1, 0);
}

void MasterPage::paintEvent(QPaintEvent* )
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
