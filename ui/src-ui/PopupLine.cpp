#include "PopupLine.hpp"
#include "IconButton.hpp"

#include <QLineEdit>
#include <QKeyEvent>
#include <QStyleOption>
#include <QPainter>

PopupLine::PopupLine(QWidget* parent,
                     IconButton* icon,
                     const QString& placeholderText,
                     const QSize& size)
: QDialog(parent), line_(new QLineEdit(this))
{
    QDialog::setFixedSize(size);

    line_->setFixedSize(QSize(size.width() * 0.9, size.height()));

    line_->move(size.width() * 0.05,0);

    // Removes focus rectangle around line edit
    line_->setAttribute(Qt::WA_MacShowFocusRect, false);

    line_->setPlaceholderText(placeholderText);

    if (icon)
    {
        icon->setParent(this);

        setIconButton(icon);
    }
}

void PopupLine::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Escape)
    {
        text_ = QString();

        QDialog::close();
    }

    else if (event->key() == Qt::Key_Return ||
             event->key() == Qt::Key_Enter)
    {
        text_ = line_->text();

        if (! text_.isEmpty()) QDialog::close();
    }

    else event->ignore();
}

void PopupLine::setIconButton(IconButton *icon)
{
    QSize size = QDialog::size();

    QDialog::setFixedWidth(size.width() * 1.1);

    icon->move(size.width() * 0.97,
               (size.height() - icon->getStandardSize()->height())/2);

    icon->show();
}

void PopupLine::paintEvent(QPaintEvent*)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

QString PopupLine::getInput() const
{
    return text_;
}
