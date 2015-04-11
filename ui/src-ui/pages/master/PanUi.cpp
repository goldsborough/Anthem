#include "PanUi.hpp"
#include "ModControl.hpp"

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
	QVBoxLayout* layout = new QVBoxLayout(this);

	layout->setSpacing(0);

	layout->setMargin(0);

	layout->setContentsMargins(0, 0, 0, 0);


	QPushButton* settings = new QPushButton(this);

	ModControl* control = new ModControl("PAN", 2, 2, this);


	QWidget* type = new QWidget(this);

	QVBoxLayout* typeLayout = new QVBoxLayout(type);

	typeLayout->setSpacing(0);

	typeLayout->setMargin(0);

	typeLayout->setContentsMargins(0, 0, 0, 0);


	QButtonGroup* group = new QButtonGroup(type);

	QPushButton* linear = new QPushButton("LINEAR", type);

	linear->setSizePolicy(QSizePolicy::Expanding,
						  QSizePolicy::Expanding);

	linear->setCursor(Qt::PointingHandCursor);

	linear->setCheckable(true);

	typeLayout->addWidget(linear);

	group->addButton(linear);


	QPushButton* sine = new QPushButton("SINE", type);

	sine->setSizePolicy(QSizePolicy::Expanding,
						QSizePolicy::Expanding);

	sine->setCursor(Qt::PointingHandCursor);

	sine->setCheckable(true);

	typeLayout->addWidget(sine);

	group->addButton(sine);


	QPushButton* radical = new QPushButton("RADICAL", type);

	radical->setSizePolicy(QSizePolicy::Expanding,
						   QSizePolicy::Expanding);

	radical->setCursor(Qt::PointingHandCursor);

	radical->setCheckable(true);

	typeLayout->addWidget(radical);

	group->addButton(radical);

	connect(group, static_cast<void (QButtonGroup::*)
				   (QAbstractButton*)>(&QButtonGroup::buttonClicked),
			[=] (QAbstractButton* button)
			{
				settings->setText(button->text());
				settings->click();
			});

	linear->click();


	connect(settings, &QPushButton::clicked,
			[=] (bool)
			{
				if (control->isHidden())
				{
					type->hide();
					control->show();
				}

				else
				{
					control->hide();
					type->show();
				}
			});

	settings->setCursor(Qt::PointingHandCursor);


	layout->addWidget(settings);

	layout->addWidget(type);

	type->hide();

	layout->addWidget(control);
}

void PanUi::paintEvent(QPaintEvent*)
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
