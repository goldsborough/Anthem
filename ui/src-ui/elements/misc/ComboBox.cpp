#include "ComboBox.hpp"
#include "Menu.hpp"

#include <QString>
#include <QPainter>
#include <QLineEdit>
#include <QListView>
#include <QPushButton>
#include <QStringList>
#include <QGridLayout>
#include <QStyleOption>
#include <QResizeEvent>
#include <QStandardItemModel>

ComboBox::ComboBox(QWidget* parent,
				   Direction direction,
				   Qt::Alignment alignment)
: QWidget(parent),
  widthOffset_(0),
  heightOffset_(0),
  current_(0),
  button_(new QPushButton(this)),
  view_(new QListView(this)),
  model_(new QStandardItemModel(this))
{
	button_->setCursor(Qt::PointingHandCursor);

	button_->setSizePolicy(QSizePolicy::Expanding,
						   QSizePolicy::Expanding);

	connect(button_, &QPushButton::clicked,
			this, &ComboBox::popup);

	auto layout = new QGridLayout(this);

	layout->setMargin(0);

	layout->addWidget(button_);

	setupView();

	setDirection(direction);

	setAlignment(alignment);
}

void ComboBox::popup()
{
	auto pos = QWidget::mapToGlobal({0, 0});

	switch(direction_)
	{
		case Direction::UP:
		{
			pos.ry() -= (view_->sizeHintForRow(0) * model_->rowCount());

			pos.rx() -= widthOffset_;

			break;
		}

		case Direction::DOWN:
		{
			pos.ry() += button_->height();

			pos.rx() -= widthOffset_;

			break;
		}

		case Direction::RIGHT:
		{
			pos.rx() += button_->width();

			pos.ry() -= heightOffset_;

			break;
		}

		case Direction::LEFT:
		{
			pos.rx() -= (view_->sizeHintForColumn(0) * model_->rowCount());

			pos.ry() -= heightOffset_;

			break;
		}
	}

	// Please don't ask why. Please don't.
	view_->move(0, 0);

	view_->show();

	view_->move(pos);
}

ComboBox::index_t ComboBox::addItem(const QString& text)
{	
	auto item = new QStandardItem(text);

	item->setData(Qt::AlignCenter, Qt::TextAlignmentRole);

	int count = model_->rowCount();

	// Set first item
	if (! count++) button_->setText(text);

	model_->appendRow(item);

	switch(direction_)
	{
		case Direction::UP:
		case Direction::DOWN:
		{
			view_->setFixedHeight(view_->sizeHintForRow(0) * count);

			break;
		}

		case Direction::LEFT:
		case Direction::RIGHT:
		{
			view_->setFixedWidth(view_->sizeHintForColumn(0) * count);

			break;
		}
	}

	return item->row();
}

void ComboBox::addItems(const QStringList &list)
{
	for (auto& i : list) addItem(i);
}

void ComboBox::setCurrent(index_t index)
{
	auto modelIndex = model_->index(index, 0);

	button_->setText(modelIndex.data().toString());

	current_ = modelIndex.row();
}

QString ComboBox::getCurrent() const
{
	return button_->text();
}

ComboBox::index_t ComboBox::getCurrentIndex() const
{
	return current_;
}

void ComboBox::setDirection(int direction)
{
	direction_ = direction;

	int count = model_->rowCount();

	switch(direction_)
	{
		case Direction::UP:
		case Direction::DOWN:
		{
			view_->setFlow(QListView::Flow::TopToBottom);

			view_->setFixedHeight(view_->sizeHintForRow(0) * count);

			break;
		}

		case Direction::RIGHT:
		case Direction::LEFT:
		{
			view_->setFlow(QListView::Flow::LeftToRight);

			view_->setFixedWidth(view_->sizeHintForColumn(0) * count);

			break;
		}
	}
}

int ComboBox::getDirection() const
{
	return direction_;
}

void ComboBox::setAlignment(Qt::Alignment alignment)
{
	alignment_ = alignment;

	for (index_t row = 0; row < model_->rowCount(); ++row)
	{
		model_->item(row)->setData({alignment}, Qt::TextAlignmentRole);
	}
}

Qt::Alignment ComboBox::getAlignment() const
{
	return alignment_;
}


void ComboBox::setupView()
{
	view_->setModel(model_);

	view_->setCursor(Qt::PointingHandCursor);

	view_->setWindowFlags(Qt::Popup);

	view_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	view_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	view_->hide();

	connect(view_, &QListView::clicked,
			[=] (const QModelIndex& index)
	{
		auto text = index.data().toString();

		button_->setText(text);

		current_ = index.row();

		view_->hide();

		emit currentChanged(text);

		emit currentIndexChanged(current_);

	});
}

void ComboBox::resizeEvent(QResizeEvent *event)
{
	QWidget::resizeEvent(event);

	switch(direction_)
	{
		case Direction::UP:
		case Direction::DOWN:
		{
			view_->setFixedWidth(event->size().width() + widthOffset_);

			break;
		}

		case Direction::RIGHT:
		case Direction::LEFT:
		{
			view_->setFixedHeight(event->size().height() + heightOffset_);

			break;
		}
	}
}

void ComboBox::setHeightOffset(int offset)
{
	heightOffset_ = offset;
}

int ComboBox::getHeightOffset() const
{
	return heightOffset_;
}


void ComboBox::setWidthOffset(int offset)
{
	widthOffset_ = offset;
}

int ComboBox::getWidthOffset() const
{
	return widthOffset_;
}

void ComboBox::paintEvent(QPaintEvent *)
{
	QStyleOption option;

	option.init(this);

	QPainter painter(this);

	style()->drawPrimitive(QStyle::PE_Widget, &option, &painter, this);
}
