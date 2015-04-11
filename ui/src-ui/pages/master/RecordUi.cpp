#include "RecordUi.hpp"
#include "IconButton.hpp"

#include <QPainter>
#include <QStyleOption>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>

RecordUi::RecordUi(QWidget* parent)
: QWidget(parent)
{
	setupUi();
}

void RecordUi::setupUi()
{
	QGridLayout* layout = new QGridLayout(this);

	layout->setSpacing(0);

	layout->setMargin(0);

	layout->setContentsMargins(0, 0, 0, 0);


	QLineEdit* filename = new QLineEdit(this);

	filename->setSizePolicy(QSizePolicy::Minimum,
							QSizePolicy::Minimum);

	filename->setPlaceholderText("Filename");

	// Removes focus rectangle around line edit
	filename->setAttribute(Qt::WA_MacShowFocusRect, false);

	layout->addWidget(filename, 0, 0, 1, 2);


	IconButton* folder = new IconButton(QIcon(":/icons/folder.png"),
										QIcon(":/icons/folder-active.png"),
										this);

	layout->addWidget(folder, 0, 2);


	QPushButton* record = new QPushButton("●", this);

	record->setSizePolicy(QSizePolicy::Expanding,
						  QSizePolicy::Fixed);

	record->setObjectName("Record");

	record->setCheckable(true);

	record->setCursor(Qt::PointingHandCursor);

	connect(record, &QPushButton::clicked,
			[=] (bool checked)
			{
				record->setText(checked ? "❚❚" : "●");
			});

	layout->addWidget(record, 1, 0);


	QPushButton* stop = new QPushButton("■", this);

	stop->setSizePolicy(QSizePolicy::Expanding,
						QSizePolicy::Fixed);

	stop->setObjectName("Stop");

	stop->setCursor(Qt::PointingHandCursor);

	connect(stop, &QPushButton::clicked,
			[=] () { });

	layout->addWidget(stop, 1, 2);


	QPushButton* save = new QPushButton("▼", this);

	save->setSizePolicy(QSizePolicy::Expanding,
						QSizePolicy::Fixed);

	save->setObjectName("Save");

	save->setCursor(Qt::PointingHandCursor);

	connect(save, &QPushButton::clicked,
			[=] () { });

	layout->addWidget(save, 1, 3);
}

void RecordUi::paintEvent(QPaintEvent*)
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
