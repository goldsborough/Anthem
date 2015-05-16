#include "MessageBox.hpp"

#include <QGridLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QStyleOption>
#include <QPainter>
#include <QPushButton>
#include <QIcon>


#include <QDebug>



MessageBox::MessageBox(QWidget *parent)
: QDialog(parent)
{ }

MessageBox::MessageBox(const QString &title,
					   const QString &message,
					   QWidget *parent)
: MessageBox(title,
				   message,
				   QString(),
				   parent)
{ }

MessageBox::MessageBox(const QString& title,
					   const QString& message,
					   const QString& details,
					   QWidget* parent)
: QDialog(parent),
  layout_(new QGridLayout),
  lastButton_(nullptr),
  message_(new QLabel(message, this)),
  details_(new QLabel(details, this))
{
	QDialog::setWindowTitle(title);

	// Otherwise there'll be some extra padding
	if (details.isEmpty()) details_->hide();

	setupUi();
}

void MessageBox::setupUi()
{
	QVBoxLayout* main = new QVBoxLayout(this);


	message_->setWordWrap(true);

	message_->setObjectName("Message");

	main->addWidget(message_);


	details_->setWordWrap(true);

	details_->setObjectName("Details");

	main->addWidget(details_);


	main->addLayout(layout_);
}

QPushButton* MessageBox::ask()
{
	QDialog::exec();

	return lastButtonPressed();
}

void MessageBox::setTitle(const QString& title)
{
	QDialog::setWindowTitle(title);
}

QString MessageBox::getTitle() const
{
	return QDialog::windowTitle();
}


void MessageBox::setMessage(const QString& message)
{
	message_->setText(message);
}

QString MessageBox::getMessage() const
{
	return message_->text();
}


void MessageBox::setDetails(const QString& details)
{
	details_->setText(details);

	if (! details.isEmpty()) details_->show();
}

QString MessageBox::getDetails() const
{
	return details_->text();
}

QPushButton* MessageBox::lastButtonPressed() const
{
    return lastButton_;
}

void MessageBox::addButton(QPushButton* button)
{
    button->setCursor(Qt::PointingHandCursor);

    // All button clicks end up in the current window being closed
	connect(button,&QPushButton::clicked, this, &MessageBox::close);

    connect(button, &QPushButton::clicked,
			this, &MessageBox::update_);

    layout_->addWidget(button);
}

void MessageBox::update_()
{
    lastButton_ = dynamic_cast<QPushButton*>(QObject::sender());
}

void MessageBox::addButton(QPushButton* button,
                                 int row,
                                 int column,
                                 int rowStretch,
                                 int colStretch,
                                 QFlags<Qt::AlignmentFlag> align)
{
    button->setCursor(Qt::PointingHandCursor);

    // All button clicks end up in the current window being closed
	connect(button,&QPushButton::clicked, this, &MessageBox::close);

    connect(button, &QPushButton::clicked,
			this, &MessageBox::update_);

	// Message Details are 0 and 1
    layout_->addWidget(button,
					   row,
					   column,
					   rowStretch,
					   colStretch,
                       align);
}

void MessageBox::paintEvent(QPaintEvent*)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

