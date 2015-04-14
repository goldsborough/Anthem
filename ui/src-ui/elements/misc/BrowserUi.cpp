#include "BrowserUi.hpp"

#include <QPainter>
#include <QStyleOption>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QTreeView>
#include <QFileSystemModel>
#include <QDir>
#include <QHeaderView>


#include <QDebug>



BrowserUi::BrowserUi(QWidget* parent)
: QWidget(parent)
{
	setupUi();
}

void BrowserUi::setupUi()
{
	QVBoxLayout* layout = new QVBoxLayout(this);

	layout->setMargin(0);

	layout->setSpacing(0);

	layout->setContentsMargins(0,0,0,0);


	QLineEdit* search = new QLineEdit(this);

	search->setPlaceholderText("...");

	// Removes focus rectangle
	search->setAttribute(Qt::WA_MacShowFocusRect, false);

	layout->addWidget(search);


	QFileSystemModel* model = new QFileSystemModel(this);

	model->setReadOnly(true);

	model->setRootPath("/Users/petergoldsborough/Documents");

	model->setNameFilters({".wavetable"});


	QTreeView* view = new QTreeView(this);

	QHeaderView* header = view->header();

	header->setSectionResizeMode(QHeaderView::ResizeToContents);

	header->hideSection(1);

	header->hideSection(2);

	header->hideSection(3);

	//header->hide();

	view->setAttribute(Qt::WA_MacShowFocusRect, false);

	view->setModel(model);

	layout->addWidget(view);

/*

	const QList<QString> names = {"Sine",
								  "Ramp",
								  "Square",
								  "Sawtooth",
								  "Triangle"};

	for (const auto& wave : names)
	{
		for (int bits = 2; bits <= 64; bits *= 2)
		{
			QListWidgetItem* item = new QListWidgetItem(wave + QString::number(bits));

			item->setTextAlignment(Qt::AlignCenter);

			browser->addItem(item);
		}
	}

	browser->setCursor(Qt::PointingHandCursor);

	browser->setCurrentRow(0);*/

	//connect(browser, &QListWidget::currentTextChanged,
		//	[=] (const QString& text) { settings->setText(text); });
}

void BrowserUi::paintEvent(QPaintEvent*)
{
	QStyleOption option;

	option.init(this);

	QPainter painter(this);

	style()->drawPrimitive(QStyle::PE_Widget, &option, &painter, this);
}
