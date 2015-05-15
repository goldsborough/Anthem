#include "PopupLine.hpp"
#include "IconButton.hpp"

#include <QLineEdit>
#include <QKeyEvent>
#include <QStyleOption>
#include <QPainter>
#include <QHBoxLayout>

PopupLine::PopupLine(QWidget *parent)
: PopupLine(QString(), parent)
{ }

PopupLine::PopupLine(const QString &placeholderText,
					 QWidget *parent)
: PopupLine(nullptr,
			placeholderText,
			parent)
{ }

PopupLine::PopupLine(IconButton* icon,
                     const QString& placeholderText,
					 QWidget* parent)

: QDialog(parent),
  icon_(icon),
  line_(new QLineEdit(this))
{
	QDialog::setSizePolicy(QSizePolicy::Fixed,
						   QSizePolicy::Fixed);

	QHBoxLayout* layout = new QHBoxLayout(this);

	layout->setSpacing(0);

	layout->setMargin(0);

	layout->setContentsMargins(0, 0, 0, 0);

	// Removes focus rectangle around line edit
	line_->setAttribute(Qt::WA_MacShowFocusRect, false);

	line_->setPlaceholderText(placeholderText);

	layout->addWidget(line_);

	if (icon)
	{
		icon = icon_;

		icon_->setParent(this);

		layout->addWidget(icon_);
	}
}

QString PopupLine::ask()
{
	QDialog::exec();

	return line_->text();
}

void PopupLine::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Escape)
    {
        QDialog::close();
    }

    else if (event->key() == Qt::Key_Return ||
             event->key() == Qt::Key_Enter)
    {
        QString text = line_->text();

        if (! text.isEmpty())
        {
            emit receivedInput(text);

            QDialog::close();
        }
    }

    else event->ignore();
}

void PopupLine::setIconButton(IconButton *icon)
{
	icon_ = icon;
}

IconButton* PopupLine::getIconButton() const
{
    return icon_;
}

void PopupLine::setText(const QString& text)
{
	line_->setText(text);
}

QString PopupLine::getText() const
{
	return line_->text();
}

void PopupLine::setPlaceholderText(const QString& text)
{
	line_->setPlaceholderText(text);
}

QString PopupLine::getPlaceholderText() const
{
	return line_->placeholderText();
}

void PopupLine::reset()
{
	line_->clear();
}

void PopupLine::paintEvent(QPaintEvent*)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
