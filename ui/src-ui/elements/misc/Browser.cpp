#include "Browser.hpp"
#include "MessageBox.hpp"
#include "PopupLine.hpp"

#include <QPainter>
#include <QStyleOption>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTreeView>
#include <QDir>
#include <QHeaderView>
#include <QPushButton>
#include <QMenu>
#include <QToolTip>


#include <QDebug>


QIcon IconProvider::icon(IconType type) const
{
	/*
	if (type == QFileIconProvider::File)
	{
		return QIcon();
	}

	else if (type == QFileIconProvider::Folder)
	{
		return  QIcon(":/icons/folder.png");
	}

	else*/ return QFileIconProvider::icon(type);
}

QIcon IconProvider::icon(const QFileInfo &item) const
{
	/*
	if (item.isFile())
	{
		return QIcon();
	}

	else if (item.isDir())
	{
		return  QIcon(":/icons/folder.png");
	}

	else*/ return QFileIconProvider::icon(item);
}

FileModel::FileModel(QWidget *parent)
: QFileSystemModel(parent)
{ }

QVariant FileModel::data(const QModelIndex &index, int role) const
{
	auto variant = QFileSystemModel::data(index, role);

	if (role == Qt::DisplayRole && ! QFileSystemModel::isDir(index))
	{
		// Get rid of the extension. Cannot use QFileSystemModel::filePath
		// because it seems to access data() itself so that would be a
		// recursive call. Just remove everything after and the last dot.
		auto file = variant.toString();

		int dot = file.lastIndexOf('.');

		file.remove(dot, file.size() - dot);

		return file;
	}

	else return variant;
}

Proxy::Proxy(QWidget *parent)
: QSortFilterProxyModel(parent)
{ }

bool Proxy::filterAcceptsRow(int source_row,
							 const QModelIndex &source_parent) const
{
	auto source = dynamic_cast<QFileSystemModel*>(QSortFilterProxyModel::sourceModel());

	// Only filter_ indices under the root path
	if (source_parent == source->index(source->rootPath()))
	{
		return QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
	}

	return true;
}

Browser::Browser(QWidget *parent)
: QWidget(parent),
  isExpanded_(false)
{
	setupUi();
}

Browser::Browser(bool isExpanded, QWidget *parent)
: QWidget(parent),
  isExpanded_(isExpanded)
{
	setupUi();
}

void Browser::setupUi()
{
	QWidget::setProperty("isExpanded", isExpanded_);

	QWidget::style()->unpolish(this);

	QWidget::style()->polish(this);

	QWidget::update();


	setupBar();

	setupModel();

	setupView();

	setupContextMenu();


	auto layout = new QVBoxLayout(this);

	layout->setMargin(0);

	layout->setSpacing(0);

	layout->setContentsMargins(0,0,0,0);


	layout->addLayout(barLayout_);

	layout->addWidget(view_);

}

void Browser::setupBar()
{
	barLayout_ = new QHBoxLayout;

	barLayout_->setMargin(0);

	barLayout_->setSpacing(0);

	barLayout_->setContentsMargins(0,0,0,0);


	setupFilter();


	if (! isExpanded_)
	{
		setupExpansion();

		barLayout_->addWidget(expandButton_);

		QWidget::setSizePolicy(QSizePolicy::Maximum,
							   QSizePolicy::Preferred);
	}
}

void Browser::setupExpansion()
{
	expandButton_ = new QPushButton("⌝", this);

	expandButton_->setSizePolicy(QSizePolicy::Fixed,
								 QSizePolicy::Fixed);

	expandButton_->setCursor(Qt::PointingHandCursor);


	expandedDialog_ = new QDialog(this);

	auto layout = new QGridLayout(expandedDialog_);

	layout->setSpacing(0);

	layout->setContentsMargins(0, 0, 0, 0);

	layout->setMargin(0);

	// Create new Browser with expansion disabled
	auto browser = new Browser(true, expandedDialog_);

	browser->filter_->setText(filter_->text());

	browser->setSizePolicy(QSizePolicy::Expanding,
						   QSizePolicy::Expanding);

	layout->addWidget(browser);

	connect(expandButton_, &QPushButton::clicked,
			[=] (bool) { expandedDialog_->show(); });
}

void Browser::setupFilter()
{
	filter_ = new QLineEdit(this);

	filter_->setSizePolicy(QSizePolicy::Expanding,
						   QSizePolicy::Maximum);

	filter_->setPlaceholderText("Filter");

	// Removes focus rectangle
	filter_->setAttribute(Qt::WA_MacShowFocusRect, false);


	// There's an overload so can't connect directly
	connect(filter_, &QLineEdit::textChanged,
			[=] (const QString& text)
			{ proxy_->setFilterRegExp(QRegExp(text, proxy_->filterCaseSensitivity())); });

	auto caseSensitivity = new QPushButton("Aa", this);

	caseSensitivity->setSizePolicy(QSizePolicy::Maximum,
								   QSizePolicy::Maximum);

	caseSensitivity->setObjectName("CaseSensitivityButton");

	caseSensitivity->setCursor(Qt::PointingHandCursor);

	caseSensitivity->setCheckable(true);

	caseSensitivity->setToolTip("Case Sensitvity");

	connect(caseSensitivity, &QPushButton::clicked,
			[=] (bool checked)
			{
				auto cs = checked ? Qt::CaseSensitive : Qt::CaseInsensitive;

				// Re-filter
				proxy_->setFilterRegExp(QRegExp(proxy_->filterRegExp().pattern(), cs));
			});

	barLayout_->addWidget(filter_);

	barLayout_->addWidget(caseSensitivity);

}

void Browser::setupModel()
{
	model_ = new FileModel(this);

	model_->setReadOnly(false);

	model_->setRootPath("/Users/petergoldsborough/Documents/Anthem/rsc/wavetables");

	model_->setNameFilters({"*.wavetable"});

	model_->setNameFilterDisables(false);

	model_->setIconProvider(new IconProvider);


	proxy_ = new Proxy(this);

	proxy_->setSourceModel(model_);

	proxy_->setFilterCaseSensitivity(Qt::CaseInsensitive);
}

void Browser::setupView()
{
	view_ = new QTreeView;

	view_->setSizePolicy(QSizePolicy::Expanding,
						 QSizePolicy::Expanding);

	view_->setCursor(Qt::PointingHandCursor);

	view_->setAttribute(Qt::WA_MacShowFocusRect, false);

	view_->setModel(proxy_);

	view_->hideColumn(1);

	view_->hideColumn(2);

	view_->hideColumn(3);

	view_->header()->hide();

	view_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	view_->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);


	auto index = model_->index("/Users/petergoldsborough/Documents/Anthem/rsc/wavetables");

	view_->setRootIndex(proxy_->mapFromSource(index));

	// Editable via context-menu, but no other edit triggers
	view_->setEditTriggers(QTreeView::NoEditTriggers);

	view_->setRootIsDecorated(false);

	view_->setSortingEnabled(true);

	// Expands directories and selects Wavetables
	connect(view_, &QTreeView::clicked,
			this, &Browser::itemClicked);
}

void Browser::itemClicked(const QModelIndex &index)
{
	static QFlags<QDir::Filter> filter(QDir::AllEntries | QDir::NoDotAndDotDot);

	auto mapped = proxy_->mapToSource(index);

	if (model_->isDir(mapped))
	{
		if (QDir(model_->filePath(mapped)).entryList(filter).count())
		{
			view_->setExpanded(index, ! view_->isExpanded(index));
		}

		else QToolTip::showText(QWidget::cursor().pos(), "Empty", nullptr, { }, 400);
	}

	else emit wavetableSelected(index.data().toString());
}

void Browser::setupContextMenu()
{
	QWidget::setContextMenuPolicy(Qt::CustomContextMenu);

	context_ = new QMenu(this);

	connect(this, &QWidget::customContextMenuRequested,
			[=] (const QPoint& pos)
	{ context_->popup(QWidget::mapToGlobal(pos)); });


	connect(context_->addAction("Rename"), &QAction::triggered,
			this, &Browser::renameAction_);

	connect(context_->addAction("Delete"), &QAction::triggered,
			this, &Browser::deleteAction_);


	context_->addSeparator();

	connect(context_->addAction("New folder"), &QAction::triggered,
		   this, &Browser::newFolderAction_);
}

void Browser::renameAction_()
{
	auto index = proxy_->mapToSource(view_->indexAt(view_->mapFromGlobal(context_->pos())));

	auto file = model_->fileInfo(index);

	PopupLine line(this);

	line.setText(file.baseName());

	for( /* inf */ ; /* in */ ; /* ity */ )
	{
		auto name = line.ask();

		if (name.isEmpty() || name == file.baseName()) break;

		auto entries = file.dir().entryList(QDir::Dirs | QDir::Files);

		if (file.isFile()) name += ".wavetable";

		if (! entries.contains(name))
		{
			file.dir().rename(file.fileName(), name);

			break;
		}

		MessageBox error("Duplicate",
							   "A file or folder with that\n name already exists!",
							   this);

		auto again = new QPushButton("Try again", &error);

		again->setShortcut(Qt::Key_Enter);

		error.addButton(again, 0, 0);

		// No shortcut needed here
		error.addButton(new QPushButton("Give up", &error), 0, 1);

		error.setFixedSize(error.sizeHint());

		if (error.ask() != again) break;
	}
}

void Browser::deleteAction_()
{
	MessageBox confirm("Confirm permanent obliteration",
							 "Like, for ever and stuff.",
							 this);

	auto yes = new QPushButton("Yes", &confirm);

	yes->setShortcut(Qt::Key_Enter);

	confirm.addButton(yes, 0, 0);

	confirm.addButton(new QPushButton("No", &confirm), 0, 1);

	auto index = proxy_->mapToSource(view_->indexAt(view_->mapFromGlobal(context_->pos())));

	auto name = model_->fileName(index);

	if (model_->isDir(index))
	{
		confirm.setMessage("Do you really want to delete all\n"
							"Wavetables in the folder "
							+ name + "?");
	}

	else
	{
		confirm.setMessage("Do you really want to\n"
							"delete the Wavetable "
							+ name + "?");
	}

	confirm.setFixedSize(confirm.sizeHint());

	auto button = confirm.ask();

	if(button && button->text() == "Yes")
	{
		if (model_->isDir(index)) model_->rmdir(index);

		else model_->remove(index);
	}
}

void Browser::newFolderAction_()
{
	PopupLine line("Enter a name for the new folder", this);

	auto index = proxy_->mapToSource(view_->indexAt(view_->mapFromGlobal(context_->pos())));

	auto dir = model_->fileInfo(index).dir();

	for( /* inf */ ; /* in */ ; /* ity */ )
	{
		auto name = line.ask();

		if (name.isEmpty()) break;

		if (! dir.entryList().contains(name))
		{
			// Get index for directory and mkdir there
			model_->mkdir(model_->index(dir.path()), name);

			break;
		}

		MessageBox error("Duplicate",
							   "A folder with that name already\n"
							   "exists at this location!",
							   this);

		auto again = new QPushButton("Try again", &error);

		again->setShortcut(Qt::Key_Enter);

		error.addButton(again, 0, 0);

		// No shortcut needed here
		error.addButton(new QPushButton("Give up", &error), 0, 1);

		error.setFixedSize(error.sizeHint());

		if (error.ask() != again) break;
	}
}

void Browser::paintEvent(QPaintEvent*)
{
	QStyleOption option;

	option.init(this);

	QPainter painter(this);

	style()->drawPrimitive(QStyle::PE_Widget, &option, &painter, this);
}
